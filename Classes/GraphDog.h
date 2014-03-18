//
//  GraphDog.h
//  archervszombie
//
//  Created by LitQoo on 13. 2. 14..
//
//

#ifndef __litqoo__GraphDog__
#define __litqoo__GraphDog__

#include <iostream>
#include "GDLib.h"
#include "curl/curl.h"
#include <stdlib.h>
#include <queue>
#include <list>
#define GRAPHDOG_VERSION    "2"
#include "jsoncpp/json.h"

enum GDRESULT{
	GDUNKNOWNRESULT = 0,
	GDSUCCESS = 1,
	GDDONTMAKETHREAD = 2,
	GDSECURITY = 3,
	GDCHECKNETWORK = 4,
	GDMYSQLQUERY = 1001,
	GDMYSQLCONNECTION = 1002,
	GDMYSQLFETCH = 1003,
	GDSAMEVERSION = 2001,
	GDPARAMETER = 2002,
	GDDONTFIND = 2003,
	GDFRIENDMAX = 2004,
	GDDONTFINDUSER = 2005,
	GDFAILTOSAVEUSERDATA = 2006
};

struct GDStruct {
    char *memory;
    size_t size;
	CURLcode resultCode;
	GDStruct(){}
	GDStruct(char *t_memory, size_t t_size, CURLcode t_resultCode)
	{
		memory = t_memory;
		size = t_size;
		resultCode = t_resultCode;
	}
};


struct AutoIncrease
{
	static int cnt;
	static int get(){return cnt++;}
};

struct CommandParam
{
	string action;
	//@ JsonBox::Object param;
	Json::Value param;
    //@@const CCObject* target;
	//@@GDSelType selector;
    function<void(Json::Value)> func;
    
    //@ CommandParam(const string& _action, const JsonBox::Object& _param, const CCObject* _target, GDSelType _selector)
	//@@CommandParam(const string& _action, const Json::Value _param, const CCObject* _target, GDSelType _selector, )
	CommandParam(const string& _action, const Json::Value _param, function<void(Json::Value)> _func)
    {
		action = _action;
		param = _param;
//@@		target = _target;
//@@		selector = _selector;
        func = _func;
	}
	//@@CommandParam(const string& _action, int _param, const CCObject* _target, GDSelType _selector)
    CommandParam(const string& _action, int _param, function<void(Json::Value)> _func)
	{
		CCAssert(_param == 0, "_param == 0");
		action = _action;
		//@ JsonBox::Object _p;
        Json::Value _p;
        param = _p;
        //@@target = _target;
		//@@selector = _selector;
        func=_func;
	}
	CommandParam(){}
//	CommandParam(string a, const JsonBox::Object* p, const CCObject* t, GDSelType& s) : action(a),
//		param(p), target(t), selector(s) {}
};

class GraphDog: public CCObject{
public:
    //시작설정
    void setup(string secretKey,string _appVersion); //nhn용 새로추가
    void setup(string appID,string secretKey,string _packageName,string _appVersion);
    //명령날리기 - 이 함수로 모든 통신을 할수있다. 쓰레드생성 실패시 false 그외 true
    
    bool command(const std::vector<CommandParam>& params);
    
//@    bool command(string action, const JsonBox::Object* const param,CCObject *target,GDSelType selector);
    
//@    bool test(string action, const JsonBox::Object* const param,CCObject *target, GDSelType selector, JsonBox::Object result);
//@    bool test(string action, const JsonBox::Object* const param,CCObject *target, GDSelType selector, string result);
    
	//@@    bool command(string action, const Json::Value param,CCObject *target,GDSelType selector);

    bool command(string action, const Json::Value param,function<void(Json::Value)> func);
    bool test(string action, const Json::Value param,CCObject *target, GDSelType selector, Json::Value result);
    bool test(string action, const Json::Value param,CCObject *target, GDSelType selector, string result);
  
	
	
	
	

	
	std::vector<CCObject*> deleList;
	
	void addTarget(CCObject *obj){
		CCLog("checkDelegator addTarget");
		vector<CCObject*>::iterator it;
		for (it=deleList.begin();it!=deleList.end();it++) {
			if((*it)==obj)return;
		}
		
		CCLog("checkDelegator addTarget ok");
		deleList.push_back(obj);
	}
	
	bool cehckTarget(CCObject *obj){
		CCLog("checkDelegator");
		if(obj==NULL)return false;
		vector<CCObject*>::iterator it;
		for (it=deleList.begin();it!=deleList.end();it++) {
			if((*it)==obj){
				CCLog("checkDelegator find ok");
				return true;
			}
		}
		CCLog("checkDelegator don't find");
		return false;
	}
	
	void removeTarget(CCObject *obj){
		CCLog("checkDelegator removeTarget");
		vector<CCObject*>::iterator it;

		for (it=deleList.begin();it!=deleList.end();it++) {
			if((*it)==obj){
					deleList.erase(it);
					CCLog("checkDelegator remove Target success");
					return;
			}
		}
	}
	
	long long int getTime(){
		return (long long int)time(NULL);
	}
	
	bool command(string action, const Json::Value param,CCObject *target,function<void(Json::Value)> func){
		addTarget(target);
		function<void(Json::Value)> sFunc = [=](Json::Value value){
			CCLog("checkDelegator sFunc call");
			if(GraphDog::get()->cehckTarget(target))func(value);
		};
		
		return command(action,param,sFunc);
	}
    //닉네임저장
    void setNick(string nick);
    //플레그저장
    void setFlag(string flag);
    //이메일저장
    void setEmail(string email);
    //언어저장
    void setLanguage(string lang);
    //hspmemberno저장
	void setHSPMemberNo(long long int _hspMemberNo);
	
	//kakaomemberno저장
	void setKakaoMemberID(string _kakaomemberID);
	
    //graphdog 버전설정 - 평소엔 필요없음
    void setGraphDogVersion(int version);
    
    bool isLogin;
    
    long long int timestamp;
		long long int localTimestamp;
	
    string getNick();
    string getFlag();
    string getAuID();
    string getLanguage();
    string getPlatform();
    string getEmail();
    int getAppVersion();
    string getAppVersionString();
	string getGraphDogVersion();
	string getDeviceID();
    string getDeviceInfo();
    long long int getHSPMemberNo();
	string getKakaoMemberID();
	
    static GraphDog* get()
	{
		static GraphDog* _ins = 0;
		if(_ins == 0)
			_ins = new GraphDog();
		return _ins;
	}
    
	
    struct CommandType
	{
        function<void(Json::Value)> func;
		//@@ const CCObject* target;
		//@@ GDSelType selector;
		string paramStr;
		string action;
	};
	struct CommandsType
	{
		std::map<string, CommandType> commands; //gid, CommandType
		string commandStr;
		GraphDog* caller;
		GDStruct chunk;
		//@ JsonBox::Object result;
        Json::Value result;
	};
	std::map<int, CommandsType> commandQueue;
    void removeCommand(cocos2d::CCObject *target);

	//@ JsonBox::Object dictParam;
    Json::Value dictParam;
private:
//    GDStruct gdchunk;
	pthread_mutex_t t_functionMutex;
//	pthread_mutex_t cmdsMutex;
//	pthread_mutex_t authMutex;
    string aID;
    string sKey;
    string udid;
	string packageName;
    string appVersion;
	string deviceInfo;
	long long int hspMemberNo;
	string kakaoMemberID;
    CURL* getCURL();
    string getToken();
    string getUdid();
    string getCTime();
    int errorCount;
    void setAuID(string appuserID);
    void setUdid(string _id);
    void setCTime(string cTime);
    string gdVersion;
    CURL *curl_handle;
    static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
    static void* t_function(void *data);
    void receivedCommand(float dt);

    GraphDog(){
			
		pthread_mutex_init(&t_functionMutex, NULL);
//		pthread_mutex_init(&cmdsMutex, NULL);
		//pthread_mutex_lock(&authMutex);
        curl_global_init(CURL_GLOBAL_ALL);
        curl_handle = curl_easy_init();
        curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, true);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "GraphDog-agent/1.0");
        curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, true);
        curl_easy_setopt(curl_handle, CURLOPT_COOKIEJAR,"sessid");
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl_handle, CURLOPT_POST, true);
		curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 11);
		
        isLogin=false;
        errorCount=0;
        this->gdVersion = GRAPHDOG_VERSION;
		this->deviceInfo = "";
			this->timestamp = 9;
			this->localTimestamp = 0;
    }
  
    ~GraphDog(){
        curl_easy_cleanup(curl_handle);
		curl_global_cleanup();
    }
};

extern GraphDog* graphdog;
#endif /* defined(__litqoo__GraphDog__) */
