// LogData.cpp
//

#include "LogData.h"
#define LZZ_INLINE inline
LogData * LogData::sharedInstance ()
        {
		static LogData* t_log = NULL;
		if(t_log == NULL)
		{
			t_log = new LogData();
			t_log->myInit();
		}
		return t_log;
	}
int LogData::getLogCount ()
        {
		return myDefault->getValue(kSDF_log, getRKey(kLOG_count), 0);
	}
void LogData::addLog (LOG_KEY t_key, int t_time)
        {
		int log_count = getLogCount();
		log_count++;
		
		myDefault->addKeyValue(kSDF_log, CCString::createWithFormat(getRKey(kLOG_data).c_str(), log_count)->getCString(), CCString::createWithFormat(getRKey(t_key).c_str(), t_time)->getCString());
		myDefault->setKeyValue(kSDF_log, getRKey(kLOG_count), log_count);
	}
void LogData::addLog (LOG_KEY t_key, int t_time, float t_percent)
        {
		int log_count = getLogCount();
		log_count++;
		
		myDefault->addKeyValue(kSDF_log, CCString::createWithFormat(getRKey(kLOG_data).c_str(), log_count)->getCString(), CCString::createWithFormat(getRKey(t_key).c_str(), t_time, t_percent)->getCString());
		myDefault->setKeyValue(kSDF_log, getRKey(kLOG_count), log_count);
	}
void LogData::addLog (LOG_KEY t_key, int t_time, int t_i1)
        {
		int log_count = getLogCount();
		log_count++;
		
		myDefault->addKeyValue(kSDF_log, CCString::createWithFormat(getRKey(kLOG_data).c_str(), log_count)->getCString(), CCString::createWithFormat(getRKey(t_key).c_str(), t_time, t_i1)->getCString());
		myDefault->setKeyValue(kSDF_log, getRKey(kLOG_count), log_count);
	}
void LogData::addLog (LOG_KEY t_key, int t_time, string t_str1)
        {
		int log_count = getLogCount();
		log_count++;
		
		myDefault->addKeyValue(kSDF_log, CCString::createWithFormat(getRKey(kLOG_data).c_str(), log_count)->getCString(), CCString::createWithFormat(getRKey(t_key).c_str(), t_time, t_str1.c_str())->getCString());
		myDefault->setKeyValue(kSDF_log, getRKey(kLOG_count), log_count);
	}
void LogData::sendLog (string t_category)
        {
		Json::Value param;
		
		param["memberID"] = hspConnector::get()->getKakaoID();
		
		Json::Value contents;
		int loop_cnt = getLogCount();
		for(int i=1;i<=loop_cnt;i++)
		{
			contents[i-1] = getLog(kLOG_data, i);
		}
		
		param["content"] = contents;
		param["category"] = t_category.c_str();
		
		hspConnector::get()->command("writelog", param, NULL);
		myDefault->resetData(kSDF_log);
	}
string LogData::getLog (LOG_KEY t_key, int t_i)
        {
		return myDefault->getValue(kSDF_log, CCString::createWithFormat(getRKey(kLOG_data).c_str(), t_i)->getCString(), "");
	}
string LogData::getRKey (LOG_KEY t_key)
        {
		string rv;
		
		if(t_key == kLOG_count)						rv = "count";
		else if(t_key == kLOG_data)					rv = "data%d";
		else if(t_key == kLOG_getPercent_f)			rv = "time_%d_getPercent_%.3f";
		else if(t_key == kLOG_die_missileToLine)	rv = "time_%d_die_missileToLine";
		else if(t_key == kLOG_die_shockwave)		rv = "time_%d_die_shockwave";
		else if(t_key == kLOG_die_other)			rv = "time_%d_die_other";
		else if(t_key == kLOG_show_fever)			rv = "time_%d_show_fever";
		else if(t_key == kLOG_endCombo_i)			rv = "time_%d_endCombo_%d";
		else if(t_key == kLOG_attackPattern_i)		rv = "time_%d_attackPattern_%d";
		else if(t_key == kLOG_getCoin_i)			rv = "time_%d_getCoin_%d";
		else if(t_key == kLOG_getItem_s)			rv = "time_%d_getItem_%s";
		else if(t_key == kLOG_buyItem_s)			rv = "time_%d_buyItem_%s";
		else if(t_key == kLOG_useItem_s)			rv = "time_%d_useItem_%s";
		else if(t_key == kLOG_remainHeart_i)		rv = "time_%d_remainHeart_%d";
		else if(t_key == kLOG_gacha_startMap)		rv = "time_%d_gacha_startMap";
		else if(t_key == kLOG_gacha_onePercent)		rv = "time_%d_gacha_onePercent";
		else if(t_key == kLOG_puzzleAchievementMinimum_i)	rv = "time_%d_puzzleAchievementMinimum_%d";
		else if(t_key == kLOG_puzzleAchievementMaximum_i)	rv = "time_%d_puzzleAchievementMaximum_%d";
		else if(t_key == kLOG_typing_cardComment)			rv = "time_%d_typing_cardComment";
		else if(t_key == kLOG_action_continue)				rv = "time_%d_action_continue";
		else if(t_key == kLOG_network_getUserData)			rv = "time_%d_network_getUserData";
		else if(t_key == kLOG_network_loadCardData)			rv = "time_%d_network_loadCardData";
		else if(t_key == kLOG_network_downloadCardImg)		rv = "time_%d_network_downloadCardImg";
		else if(t_key == kLOG_network_getPuzzleEventList)	rv = "time_%d_network_getPuzzleEventList";
		else if(t_key == kLOG_network_downloadPuzzleEventImg)	rv = "time_%d_network_downloadPuzzleEventImg";
		else if(t_key == kLOG_network_setUserData)			rv = "time_%d_network_setUserData";
		else if(t_key == kLOG_getStageInfo_i)				rv = "time_%d_getStageInfo_%d";
		else if(t_key == kLog_getPuzzleInfo_i)				rv = "time_%d_getPuzzleInfo_%d";
		
		return rv.c_str();
	}
void LogData::myInit ()
        {
		myDefault = SaveData::sharedObject();
	}
#undef LZZ_INLINE
