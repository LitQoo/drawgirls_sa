//
//  StageImgLoader.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 10..
//
//

#include "StageImgLoader.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import <Foundation/Foundation.h>
#endif

CCSprite* StageImgLoader::getLoadedImg(string filename)
{
	CCTexture2D* texture = addImage(filename.c_str());
	return CCSprite::createWithTexture(texture);
}
CCSprite* StageImgLoader::getLoadedImg(string filename, CCRect t_rect)
{
	CCTexture2D* texture = addImage(filename.c_str());
	return CCSprite::createWithTexture(texture, t_rect);
}

CCTexture2D * StageImgLoader::addImage(const char * path)
{
	return CCTextureCache::sharedTextureCache()->addImage(path, true, getDocumentPath());
}

void StageImgLoader::addImageAsync(const char *path, cocos2d::CCObject *target, SEL_CallFuncO selector)
{
	CCTextureCache::sharedTextureCache()->addImageAsync(path, target, selector, true, getDocumentPath());
}

size_t StageImgLoader::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size*nmemb;
	LMemoryStruct *out=(LMemoryStruct *)userp;
    if(out && !out->stream) {
        /* open file for writing */
        out->stream = fopen(out->filename.c_str(), "wb");
        if(!out->stream)
            return -1; /* failure, can't open file to write */
    }
	out->size += realsize;
    return fwrite(contents, size, nmemb, out->stream);
}

string StageImgLoader::getDocumentPath()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return CCFileUtils::sharedFileUtils()->getWritablePath();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    std::string strRet = [documentsDirectory UTF8String];
    strRet.append("/");
    return strRet;
#endif
}

LMemoryStruct lchunk;
void* StageImgLoader::t_function(void *data)
{
	string document_path = StageImgLoader::sharedInstance()->writeable_path;
	lchunk.size = 0;
	lchunk.filename = (document_path+StageImgLoader::sharedInstance()->down_filename).c_str();
	lchunk.stream = NULL;
	
	do{
		CURL *curl_handle;
		CURLcode retCode = curl_global_init(CURL_GLOBAL_ALL); // _ALL -> _DEFAULT
		if(retCode != CURLE_OK)
		{
			StageImgLoader::sharedInstance()->isFail = true;
			break;
		}
		curl_handle = curl_easy_init();
		curl_easy_setopt(curl_handle, CURLOPT_URL, StageImgLoader::sharedInstance()->p_url.c_str());
		curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, true);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&lchunk);
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
//		curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
		
		if(curl_easy_perform(curl_handle) != CURLE_OK)
		{
			StageImgLoader::sharedInstance()->isFail = true;
			break;
		}
		curl_easy_cleanup(curl_handle);
		
		if(lchunk.stream)
			fclose(lchunk.stream);
		
		curl_global_cleanup();
		
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(StageImgLoader::successAction), StageImgLoader::sharedInstance(), 0, 0, 0, false);
		
//		while (lchunk.size!=0) {
//			usleep(10000);
//		}
	}while (0);
	
	if(StageImgLoader::sharedInstance()->isFail)
	{
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(StageImgLoader::failAction), StageImgLoader::sharedInstance(), 0, 0, 0, false);
	}
	
	return NULL;
}

float StageImgLoader::getDownloadPercentage()
{
	if(lchunk.size != 0)
	{
		float return_value = 1.f*lchunk.size/total_size;
		if(return_value < 0.f)
			return 0.f;
		else if(return_value > 1.f)
			return 1.f;
		else
			return return_value;
	}
	else
		return 0;
}

void StageImgLoader::successAction()
{
	unschedule(schedule_selector(StageImgLoader::successAction));
	
	lchunk.size = 0;
	
	if(target_success)
	{
//		my_savedata->setKeyValue(kSDF_downloadedInfo, down_filename, 1);
		(target_success->*delegate_success)();
	}
}

void StageImgLoader::failAction()
{
	unschedule(schedule_selector(StageImgLoader::failAction));
	lchunk.size = 0;
	
	if(target_fail)
		(target_fail->*delegate_fail)();
}

StageImgLoader* StageImgLoader::sharedInstance()
{
	static StageImgLoader* t_loader = NULL;
	if(t_loader == NULL)
	{
		t_loader = new StageImgLoader();
		t_loader->myInit();
	}
	return t_loader;
}

void StageImgLoader::downloadImg( string t_url, int t_size, string t_down_filename, CCObject* t_success, SEL_CallFunc d_success, CCObject* t_fail, SEL_CallFunc d_fail )
{
	target_success = t_success;
	delegate_success = d_success;
	target_fail = t_fail;
	delegate_fail = d_fail;
	down_filename = t_down_filename;

	startDownload(t_url, t_size);
}

void StageImgLoader::removeTD()
{
	isFail = true;
	target_success = NULL;
	target_fail = NULL;
}

bool StageImgLoader::isLoadedImg( string filename )
{
	//		return my_savedata->getValue(kSDF_downloadedInfo, filename, 0) == 1;
	return false;
}

void StageImgLoader::startDownload( string t_url, int t_size )
{
	p_url = t_url;
	total_size = t_size*1024;

	isFail = false;
	pthread_t p_thread;
	int thr_id;
	// 쓰레드 생성 아규먼트로 1 을 넘긴다.
	thr_id = pthread_create(&p_thread, NULL, t_function, NULL);
	if (thr_id < 0)
	{
		perror("thread create error : ");
		exit(0);
	}
}

void StageImgLoader::myInit()
{
	my_savedata = SaveData::sharedObject();
	my_savedata->createJSON(kSDF_downloadedInfo);
	writeable_path = getDocumentPath();
}
