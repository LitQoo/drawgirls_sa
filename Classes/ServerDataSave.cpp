// ServerDataSave.cpp
//

#include "ServerDataSave.h"
#define LZZ_INLINE inline
ServerDataSave * ServerDataSave::sharedInstance ()
{
	static ServerDataSave* t_sds = NULL;
	if(t_sds == NULL)
	{
		t_sds = new ServerDataSave();
		t_sds->myInit();
	}
	return t_sds;
}
bool ServerDataSave::getBoolForKey (SaveDataFile f_key, string r_key)
{
	string c_key = myDefault->getSyncKey(f_key)+r_key;
	
	iter_bool = sds_cache_bool.find(c_key);
	if(iter_bool != sds_cache_bool.end())
		return iter_bool->second;
	
	string bool_string = myDefault->getValue(f_key, r_key, "false");
	if(bool_string == "false")
	{
		sds_cache_bool[c_key] = false;
		return false;
	}
	else
	{
		sds_cache_bool[c_key] = true;
		return true;
	}
}
bool ServerDataSave::gbfk (SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getBoolForKey(key_set.f_key, key_set.r_key.c_str());
}
bool ServerDataSave::getBoolForKey (SaveDataFile f_key, string t_key, int key_val1)
{
	string c_key = myDefault->getSyncKey(f_key)+CCString::createWithFormat(t_key.c_str(), key_val1)->getCString();
	
	iter_bool = sds_cache_bool.find(c_key);
	if(iter_bool != sds_cache_bool.end())
		return iter_bool->second;
	
	string bool_string = myDefault->getValue(f_key, CCString::createWithFormat(t_key.c_str(), key_val1)->getCString(), "false");
	if(bool_string == "false")
	{
		sds_cache_bool[c_key] = false;
		return false;
	}
	else
	{
		sds_cache_bool[c_key] = true;
		return true;
	}
}
bool ServerDataSave::gbfk (SDS_KEY fr_key, int key_val1)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getBoolForKey(key_set.f_key, key_set.r_key.c_str(), key_val1);
}
bool ServerDataSave::getBoolForKey (SaveDataFile f_key, int i1, string r_key)
{
	string c_key = myDefault->getSyncKey(f_key, i1)+r_key;
	
	iter_bool = sds_cache_bool.find(c_key);
	if(iter_bool != sds_cache_bool.end())
		return iter_bool->second;
	
	string bool_string = myDefault->getValue(f_key, i1, r_key, "false");
	if(bool_string == "false")
	{
		sds_cache_bool[c_key] = false;
		return false;
	}
	else
	{
		sds_cache_bool[c_key] = true;
		return true;
	}
}
bool ServerDataSave::gbfk (int i1, SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getBoolForKey(key_set.f_key, i1, key_set.r_key);
}
bool ServerDataSave::getBoolForKey (SaveDataFile f_key, int i1, string t_key, int key_val1)
{
	string c_key = myDefault->getSyncKey(f_key, i1) + CCString::createWithFormat(t_key.c_str(), key_val1)->getCString();
	
	iter_bool = sds_cache_bool.find(c_key);
	if(iter_bool != sds_cache_bool.end())
		return iter_bool->second;
	
	string bool_string = myDefault->getValue(f_key, i1, CCString::createWithFormat(t_key.c_str(), key_val1)->getCString(), "false");
	if(bool_string == "false")
	{
		sds_cache_bool[c_key] = false;
		return false;
	}
	else
	{
		sds_cache_bool[c_key] = true;
		return true;
	}
}
bool ServerDataSave::gbfk (int i1, SDS_KEY fr_key, int key_val1)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getBoolForKey(key_set.f_key, i1, key_set.r_key, key_val1);
}
void ServerDataSave::setBoolForKey (SaveDataFile f_key, string r_key, bool t_b, bool diskWrite)
{
	string bool_string;
	if(t_b)			bool_string = "true";
	else			bool_string = "false";
	myDefault->setKeyValue(f_key, r_key, bool_string, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key)+r_key;
	sds_cache_bool[c_key] = t_b;
}
void ServerDataSave::sbfk (SDS_KEY fr_key, bool t_b, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setBoolForKey(key_set.f_key, key_set.r_key, t_b, diskWrite);
}
void ServerDataSave::setBoolForKey (SaveDataFile f_key, string r_key, int key_val1, bool t_b, bool diskWrite)
{
	string bool_string;
	if(t_b)			bool_string = "true";
	else			bool_string = "false";
	myDefault->setKeyValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), bool_string, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key)+CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	sds_cache_bool[c_key] = t_b;
}
void ServerDataSave::sbfk (SDS_KEY fr_key, int key_val1, bool t_b, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setBoolForKey(key_set.f_key, key_set.r_key, key_val1, t_b, diskWrite);
}
void ServerDataSave::setBoolForKey (SaveDataFile f_key, int i1, string r_key, bool t_b, bool diskWrite)
{
	string bool_string;
	if(t_b)			bool_string = "true";
	else			bool_string = "false";
	myDefault->setKeyValue(f_key, i1, r_key, bool_string, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key, i1)+r_key;
	sds_cache_bool[c_key] = t_b;
}
void ServerDataSave::sbfk (int i1, SDS_KEY fr_key, bool t_b, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setBoolForKey(key_set.f_key, i1, key_set.r_key, t_b, diskWrite);
}
void ServerDataSave::setBoolForKey (SaveDataFile f_key, int i1, string r_key, int key_val1, bool t_b, bool diskWrite)
{
	string bool_string;
	if(t_b)			bool_string = "true";
	else			bool_string = "false";
	myDefault->setKeyValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), bool_string, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key, i1) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	sds_cache_bool[c_key] = t_b;
}
void ServerDataSave::sbfk (int i1, SDS_KEY fr_key, int key_val1, bool t_b, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setBoolForKey(key_set.f_key, i1, key_set.r_key, key_val1, t_b, diskWrite);
}
int ServerDataSave::getIntegerForKey (SaveDataFile f_key, string r_key)
{
	string c_key = myDefault->getSyncKey(f_key) + r_key;
	
	iter_int = sds_cache_int.find(c_key);
	if(iter_int != sds_cache_int.end())
		return iter_int->second;
	
	int return_value = myDefault->getValue(f_key, r_key, 0);
	sds_cache_int[c_key] = return_value;
	return return_value;
}
int ServerDataSave::gifk (SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getIntegerForKey(key_set.f_key, key_set.r_key);
}
int ServerDataSave::getIntegerForKey (SaveDataFile f_key, string r_key, int key_val1)
{
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	
	iter_int = sds_cache_int.find(c_key);
	if(iter_int != sds_cache_int.end())
		return iter_int->second;
	
	int return_value = myDefault->getValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), 0);
	sds_cache_int[c_key] = return_value;
	return return_value;
}
int ServerDataSave::gifk (SDS_KEY fr_key, int key_val1)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getIntegerForKey(key_set.f_key, key_set.r_key, key_val1);
}
int ServerDataSave::getIntegerForKey (SaveDataFile f_key, string r_key, int key_val1, int key_val2)
{
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1, key_val2)->getCString();
	
	iter_int = sds_cache_int.find(c_key);
	if(iter_int != sds_cache_int.end())
		return iter_int->second;
	
	int return_value = myDefault->getValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1, key_val2)->getCString(), 0);
	sds_cache_int[c_key] = return_value;
	return return_value;
}
int ServerDataSave::gifk (SDS_KEY fr_key, int key_val1, int key_val2)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getIntegerForKey(key_set.f_key, key_set.r_key, key_val1, key_val2);
}
int ServerDataSave::getIntegerForKey (SaveDataFile f_key, int i1, string r_key)
{
	string c_key = myDefault->getSyncKey(f_key, i1) + r_key;
	
	iter_int = sds_cache_int.find(c_key);
	if(iter_int != sds_cache_int.end())
		return iter_int->second;
	
	int return_value = myDefault->getValue(f_key, i1, r_key, 0);
	sds_cache_int[c_key] = return_value;
	return return_value;
}
int ServerDataSave::gifk (int i1, SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getIntegerForKey(key_set.f_key, i1, key_set.r_key);
}
int ServerDataSave::getIntegerForKey (SaveDataFile f_key, int i1, string r_key, int key_val1)
{
	string c_key = myDefault->getSyncKey(f_key, i1) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	
	iter_int = sds_cache_int.find(c_key);
	if(iter_int != sds_cache_int.end())
		return iter_int->second;
	
	int return_value = myDefault->getValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), 0);
	sds_cache_int[c_key] = return_value;
	return return_value;
}
int ServerDataSave::gifk (int i1, SDS_KEY fr_key, int key_val1)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getIntegerForKey(key_set.f_key, i1, key_set.r_key, key_val1);
}
void ServerDataSave::setIntegerForKey (SaveDataFile f_key, string r_key, int val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, r_key, val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key) + r_key;
	sds_cache_int[c_key] = val1;
}
void ServerDataSave::sifk (SDS_KEY fr_key, int val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setIntegerForKey(key_set.f_key, key_set.r_key, val1, diskWrite);
}
void ServerDataSave::setIntegerForKey (SaveDataFile f_key, string r_key, int key_val1, int val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	sds_cache_int[c_key] = val1;
}
void ServerDataSave::sifk (SDS_KEY fr_key, int key_val1, int val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setIntegerForKey(key_set.f_key, key_set.r_key, key_val1, val1, diskWrite);
}
void ServerDataSave::setIntegerForKey (SaveDataFile f_key, string r_key, int key_val1, int key_val2, int val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1, key_val2)->getCString(), val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1, key_val2)->getCString();
	sds_cache_int[c_key] = val1;
}
void ServerDataSave::sifk (SDS_KEY fr_key, int key_val1, int key_val2, int val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setIntegerForKey(key_set.f_key, key_set.r_key, key_val1, key_val2, val1, diskWrite);
}
void ServerDataSave::setIntegerForKey (SaveDataFile f_key, int i1, string r_key, int val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, i1, r_key, val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key, i1) + r_key;
	sds_cache_int[c_key] = val1;
}
void ServerDataSave::sifk (int i1, SDS_KEY fr_key, int val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setIntegerForKey(key_set.f_key, i1, key_set.r_key, val1, diskWrite);
}
void ServerDataSave::setIntegerForKey (SaveDataFile f_key, int i1, string r_key, int key_val1, int val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key, i1) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	sds_cache_int[c_key] = val1;
}
void ServerDataSave::sifk (int i1, SDS_KEY fr_key, int key_val1, int val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setIntegerForKey(key_set.f_key, i1, key_set.r_key, key_val1, val1, diskWrite);
}
double ServerDataSave::getDoubleForKey (SaveDataFile f_key, string r_key)
{
	string c_key = myDefault->getSyncKey(f_key)+r_key;
	
	iter_double = sds_cache_double.find(c_key);
	if(iter_double != sds_cache_double.end())
		return iter_double->second;
	
	double return_value = myDefault->getValue(f_key, r_key, 0.0);
	sds_cache_double[c_key] = return_value;
	return return_value;
}
double ServerDataSave::gdfk (SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getDoubleForKey(key_set.f_key, key_set.r_key);
}
double ServerDataSave::getDoubleForKey (SaveDataFile f_key, string r_key, int key_val1)
{
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	
	iter_double = sds_cache_double.find(c_key);
	if(iter_double != sds_cache_double.end())
		return iter_double->second;
	
	double return_value = myDefault->getValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), 0.0);
	sds_cache_double[c_key] = return_value;
	return return_value;
}
double ServerDataSave::gdfk (SDS_KEY fr_key, int key_val1)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getDoubleForKey(key_set.f_key, key_set.r_key, key_val1);
}
double ServerDataSave::getDoubleForKey (SaveDataFile f_key, int i1, string r_key)
{
	string c_key = myDefault->getSyncKey(f_key, i1) + r_key;
	
	iter_double = sds_cache_double.find(c_key);
	if(iter_double != sds_cache_double.end())
		return iter_double->second;
	
	double return_value = myDefault->getValue(f_key, i1, r_key, 0.0);
	sds_cache_double[c_key] = return_value;
	return return_value;
}
double ServerDataSave::gdfk (int i1, SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getDoubleForKey(key_set.f_key, i1, key_set.r_key);
}
double ServerDataSave::getDoubleForKey (SaveDataFile f_key, int i1, string r_key, int key_val1)
{
	string c_key = myDefault->getSyncKey(f_key, i1) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	
	iter_double = sds_cache_double.find(c_key);
	if(iter_double != sds_cache_double.end())
		return iter_double->second;
	
	double return_value = myDefault->getValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), 0.0);
	sds_cache_double[c_key] = return_value;
	return return_value;
}
double ServerDataSave::gdfk (int i1, SDS_KEY fr_key, int key_val1)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getDoubleForKey(key_set.f_key, i1, key_set.r_key, key_val1);
}
void ServerDataSave::setDoubleForKey (SaveDataFile f_key, string r_key, double val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, r_key, val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key)+r_key;
	sds_cache_double[c_key] = val1;
}
void ServerDataSave::sdfk (SDS_KEY fr_key, double val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setDoubleForKey(key_set.f_key, key_set.r_key, val1, diskWrite);
}
void ServerDataSave::setDoubleForKey (SaveDataFile f_key, string r_key, int key_val1, double val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	sds_cache_double[c_key] = val1;
}
void ServerDataSave::sdfk (SDS_KEY fr_key, int key_val1, double val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setDoubleForKey(key_set.f_key, key_set.r_key, key_val1, val1, diskWrite);
}
void ServerDataSave::setDoubleForKey (SaveDataFile f_key, int i1, string r_key, double val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, i1, r_key, val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key, i1) + r_key;
	sds_cache_double[c_key] = val1;
}
void ServerDataSave::sdfk (int i1, SDS_KEY fr_key, double val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setDoubleForKey(key_set.f_key, i1, key_set.r_key, val1, diskWrite);
}
void ServerDataSave::setDoubleForKey (SaveDataFile f_key, int i1, string r_key, int key_val1, double val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1, diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key, i1) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	sds_cache_double[c_key] = val1;
}
void ServerDataSave::sdfk (int i1, SDS_KEY fr_key, int key_val1, double val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setDoubleForKey(key_set.f_key, i1, key_set.r_key, key_val1, val1, diskWrite);
}
string ServerDataSave::getStringForKey (SaveDataFile f_key, string r_key)
{
	string c_key = myDefault->getSyncKey(f_key) + r_key;
	
	iter_string = sds_cache_string.find(c_key);
	if(iter_string != sds_cache_string.end())
		return iter_string->second;
	
	string return_value = myDefault->getValue(f_key, r_key, "");
	sds_cache_string[c_key] = return_value;
	return return_value;
}
string ServerDataSave::gsfk (SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getStringForKey(key_set.f_key, key_set.r_key);
}
string ServerDataSave::getStringForKey (SaveDataFile f_key, string r_key, int key_val1)
{
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	
	iter_string = sds_cache_string.find(c_key);
	if(iter_string != sds_cache_string.end())
		return iter_string->second;
	
	string return_value = myDefault->getValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), "");
	sds_cache_string[c_key] = return_value;
	return return_value;
}
string ServerDataSave::gsfk (SDS_KEY fr_key, int key_val1)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getStringForKey(key_set.f_key, key_set.r_key, key_val1);
}
string ServerDataSave::getStringForKey (SaveDataFile f_key, int i1, string r_key)
{
	string c_key = myDefault->getSyncKey(f_key, i1) + r_key;
	
	iter_string = sds_cache_string.find(c_key);
	if(iter_string != sds_cache_string.end())
		return iter_string->second;
	
	string return_value = myDefault->getValue(f_key, i1, r_key, "");
	sds_cache_string[c_key] = return_value;
	return return_value;
}
string ServerDataSave::gsfk (int i1, SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getStringForKey(key_set.f_key, i1, key_set.r_key);
}
string ServerDataSave::getStringForKey (SaveDataFile f_key, int i1, string r_key, int key_val1)
{
	string c_key = myDefault->getSyncKey(f_key, i1) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	
	iter_string = sds_cache_string.find(c_key);
	if(iter_string != sds_cache_string.end())
		return iter_string->second;
	
	string return_value = myDefault->getValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), "");
	sds_cache_string[c_key] = return_value;
	return return_value;
}
string ServerDataSave::gsfk (int i1, SDS_KEY fr_key, int key_val1)
{
	SDS_SET key_set = getKeySet(fr_key);
	return getStringForKey(key_set.f_key, i1, key_set.r_key, key_val1);
}
void ServerDataSave::setStringForKey (SaveDataFile f_key, string r_key, string val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, r_key, val1.c_str(), diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key) + r_key;
	sds_cache_string[c_key] = val1;
}
void ServerDataSave::ssfk (SDS_KEY fr_key, string val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setStringForKey(key_set.f_key, key_set.r_key, val1.c_str(), diskWrite);
}
void ServerDataSave::setStringForKey (SaveDataFile f_key, string r_key, int key_val1, string val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1.c_str(), diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	sds_cache_string[c_key] = val1;
}
void ServerDataSave::ssfk (SDS_KEY fr_key, int key_val1, string val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setStringForKey(key_set.f_key, key_set.r_key, key_val1, val1.c_str(), diskWrite);
}
void ServerDataSave::setStringForKey (SaveDataFile f_key, int i1, string r_key, string val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, i1, r_key, val1.c_str(), diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key, i1) + r_key;
	sds_cache_string[c_key] = val1;
}
void ServerDataSave::ssfk (int i1, SDS_KEY fr_key, string val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setStringForKey(key_set.f_key, i1, key_set.r_key, val1.c_str(), diskWrite);
}
void ServerDataSave::setStringForKey (SaveDataFile f_key, int i1, string r_key, int key_val1, string val1, bool diskWrite)
{
	myDefault->setKeyValue(f_key, i1, CCString::createWithFormat(r_key.c_str(), key_val1)->getCString(), val1.c_str(), diskWrite);
	
	string c_key = myDefault->getSyncKey(f_key, i1) + CCString::createWithFormat(r_key.c_str(), key_val1)->getCString();
	sds_cache_string[c_key] = val1;
}
void ServerDataSave::ssfk (int i1, SDS_KEY fr_key, int key_val1, string val1, bool diskWrite)
{
	SDS_SET key_set = getKeySet(fr_key);
	setStringForKey(key_set.f_key, i1, key_set.r_key, key_val1, val1.c_str(), diskWrite);
}
SDS_SET ServerDataSave::getKeySet (SDS_KEY t_key)
{
	SDS_SET rv;
	rv.f_key = getSDF(t_key);
	rv.r_key = getRKey(t_key).c_str();
	return rv;
}
string ServerDataSave::getRKey (SDS_KEY t_key)
{
	string rv;
	
	if(t_key == kSDS_PZ_version_i)						rv = "version";
	else if(t_key == kSDS_PZ_title_s)					rv = "title";
	else if(t_key == kSDS_PZ_ticket_i)					rv = "ticket";
	else if(t_key == kSDS_PZ_point_i)					rv = "point";
	else if(t_key == kSDS_PZ_center_s)					rv = "center";
	else if(t_key == kSDS_PZ_original_s)				rv = "original";
	else if(t_key == kSDS_PZ_face_s)					rv = "face";
	else if(t_key == kSDS_PZ_map_s)						rv = "map";
	else if(t_key == kSDS_PZ_bottom_s)					rv = "bottom";
	else if(t_key == kSDS_PZ_top_s)						rv = "top";
	else if(t_key == kSDS_PZ_left_s)					rv = "left";
	else if(t_key == kSDS_PZ_right_s)					rv = "right";
	else if(t_key == kSDS_PZ_startStage_i)				rv = "startStage";
	else if(t_key == kSDS_PZ_stageCount_i)				rv = "stageCount";
	else if(t_key == kSDS_PZ_stage_int1_level_i)		rv = "stage%d_level";
	else if(t_key == kSDS_PZ_stage_int1_pieceType_s)	rv = "stage%d_pieceType";
	else if(t_key == kSDS_PZ_stage_int1_pieceNo_i)		rv = "stage%d_pieceNo";
	else if(t_key == kSDS_PZ_stage_int1_x_d)			rv = "stage%d_x";
	else if(t_key == kSDS_PZ_stage_int1_y_d)			rv = "stage%d_y";
	else if(t_key == kSDS_PZ_stage_int1_piece_s)		rv = "stage%d_piece";
	else if(t_key == kSDS_PZ_stage_int1_thumbnail_s)	rv = "stage%d_thumbnail";
	else if(t_key == kSDS_PZ_stage_int1_condition_stage_i)	rv = "stage%d_condition_stage";
	else if(t_key == kSDS_PZ_stage_int1_condition_gold_i)	rv = "stage%d_condition_gold";
	else if(t_key == kSDS_PZ_startWarp_x_d)				rv = "swarp_x";
	else if(t_key == kSDS_PZ_startWarp_y_d)				rv = "swarp_y";
	else if(t_key == kSDS_PZ_lastWarp_x_d)				rv = "lwarp_x";
	else if(t_key == kSDS_PZ_lastWarp_y_d)				rv = "lwarp_y";
	
	else if(t_key == kSDS_GI_puzzleListVersion_i)		rv = "puzzleListVersion";
	else if(t_key == kSDS_GI_puzzleListCount_i)			rv = "puzzleListCount";
	else if(t_key == kSDS_GI_puzzleList_int1_no_i)		rv = "puzzleList%d_no";
	else if(t_key == kSDS_GI_puzzleList_int1_title_s)	rv = "puzzleList%d_title";
	else if(t_key == kSDS_GI_puzzleList_int1_version_i)	rv = "puzzleList%d_version";
	else if(t_key == kSDS_GI_puzzleList_int1_thumbnail_s)			rv = "puzzleList%d_thumbnail";
	
	else if(t_key == kSDS_GI_eventListVersion_i)		rv = "eventListVersion";
	else if(t_key == kSDS_GI_eventListCount_i)			rv = "eventListCount";
	else if(t_key == kSDS_GI_eventList_int1_no_i)		rv = "eventList%d_no";
	else if(t_key == kSDS_GI_eventList_int1_title_s)	rv = "eventList%d_title";
	else if(t_key == kSDS_GI_eventList_int1_version_i)	rv = "eventList%d_version";
	else if(t_key == kSDS_GI_eventList_int1_thumbnail_s)			rv = "eventList%d_thumbnail";
	
	else if(t_key == kSDS_GI_characterVersion_i)		rv = "characterVersion";
	else if(t_key == kSDS_GI_characterCount_i)			rv = "characterCount";
	else if(t_key == kSDS_GI_characterInfo_int1_no_i)		rv = "ci%d_no";
	else if(t_key == kSDS_GI_characterInfo_int1_name_s)		rv = "ci%d_name";
	else if(t_key == kSDS_GI_characterInfo_int1_purchaseInfo_type_s)	rv = "ci%d_pi_type";
	else if(t_key == kSDS_GI_characterInfo_int1_purchaseInfo_value_i)	rv = "ci%d_pi_value";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_gold_d)		rv = "ci%d_si_gold";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_percent_d)		rv = "ci%d_si_percent";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_feverTime_i)	rv = "ci%d_si_feverTime";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_speed_d)		rv = "ci%d_si_speed";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_life_i)		rv = "ci%d_si_life";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_lineColor_i)	rv = "ci%d_si_lineColor";
	else if(t_key == kSDS_GI_characterInfo_int1_statInfo_slotCnt_i)		rv = "ci%d_si_slotCnt";
	else if(t_key == kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s)	rv = "ci%d_ri_ccbiID";
	else if(t_key == kSDS_GI_characterInfo_int1_resourceInfo_ccbi_s)	rv = "ci%d_ri_ccbi";
	else if(t_key == kSDS_GI_characterInfo_int1_resourceInfo_imageID_s)	rv = "ci%d_ri_imageID";
	else if(t_key == kSDS_GI_characterInfo_int1_resourceInfo_plist_s)	rv = "ci%d_ri_plist";
	else if(t_key == kSDS_GI_characterInfo_int1_resourceInfo_pvrccz_s)	rv = "ci%d_ri_pvrccz";
	else if(t_key == kSDS_GI_characterInfo_int1_resourceInfo_size_i)	rv = "ci%d_ri_size";
	else if(t_key == kSDS_GI_characterInfo_int1_comment_s)				rv = "ci%d_comment";
	
	else if(t_key == kSDS_GI_monsterVersion_i)			rv = "monsterVersion";
	else if(t_key == kSDS_GI_monsterCount_i)			rv = "monsterCount";
	else if(t_key == kSDS_GI_monsterInfo_int1_no_i)			rv = "mi%d_no";
	else if(t_key == kSDS_GI_monsterInfo_int1_name_s)		rv = "mi%d_name";
	else if(t_key == kSDS_GI_monsterInfo_int1_isBoss_b)		rv = "mi%d_isBoss";
	else if(t_key == kSDS_GI_monsterInfo_int1_resourceInfo_ccbiID_s)	rv = "mi%d_ri_ccbiID";
	else if(t_key == kSDS_GI_monsterInfo_int1_resourceInfo_ccbi_s)		rv = "mi%d_ri_ccbi";
	else if(t_key == kSDS_GI_monsterInfo_int1_resourceInfo_imageID_s)	rv = "mi%d_ri_imageID";
	else if(t_key == kSDS_GI_monsterInfo_int1_resourceInfo_plist_s)		rv = "mi%d_ri_plist";
	else if(t_key == kSDS_GI_monsterInfo_int1_resourceInfo_pvrccz_s)	rv = "mi%d_ri_pvrccz";
	else if(t_key == kSDS_GI_monsterInfo_int1_resourceInfo_size_i)		rv = "mi%d_ri_size";
	
	else if(t_key == kSDS_SI_version_i)					rv = "version";
	else if(t_key == kSDS_SI_puzzle_i)					rv = "puzzle";
	else if(t_key == kSDS_SI_playtime_i)				rv = "playtime";
	else if(t_key == kSDS_SI_scoreRate_d)				rv = "scoreRate";
	else if(t_key == kSDS_SI_scale_d)					rv = "scale";
	else if(t_key == kSDS_SI_missionType_i)				rv = "mission_type";
	else if(t_key == kSDS_SI_missionOptionEnergy_i)		rv = "mission_option_energy";
	else if(t_key == kSDS_SI_missionOptionCount_i)		rv = "mission_option_count";
	else if(t_key == kSDS_SI_missionOptionPercent_i)	rv = "mission_option_percent";
	else if(t_key == kSDS_SI_missionOptionSec_i)		rv = "mission_option_sec";
	else if(t_key == kSDS_SI_shopItemsCnt_i)			rv = "shopItems_cnt";
	else if(t_key == kSDS_SI_shopItems_int1_currency_s)	rv = "shopItems_%d_currency";
	else if(t_key == kSDS_SI_shopItems_int1_type_i)		rv = "shopItems_%d_type";
	else if(t_key == kSDS_SI_shopItems_int1_price_i)	rv = "shopItems_%d_price";
	else if(t_key == kSDS_SI_defItemsCnt_i)				rv = "defItems_cnt";
	else if(t_key == kSDS_SI_defItems_int1_type_i)		rv = "defItems_%d_type";
	else if(t_key == kSDS_SI_itemOptionAttackPower_i)	rv = "itemOption_attack_power";
	else if(t_key == kSDS_SI_itemOptionAddTimeSec_i)	rv = "itemOption_addTime_sec";
	else if(t_key == kSDS_SI_itemOptionFastSec_i)		rv = "itemOption_fast_sec";
	else if(t_key == kSDS_SI_itemOptionSilenceSec_i)	rv = "itemOption_silence_sec";
	else if(t_key == kSDS_SI_itemOptionDoubleItemPercent_i)			rv = "itemOption_doubleItem_percent";
	else if(t_key == kSDS_SI_itemOptionLongTimeSec_i)	rv = "itemOption_longTime_sec";
	else if(t_key == kSDS_SI_itemOptionBossLittleEnergyPercent_i)	rv = "itemOption_bossLittleEnergy_percent";
	else if(t_key == kSDS_SI_itemOptionSubSmallSizePercent_i)		rv = "itemOption_subSmallSize_percent";
	else if(t_key == kSDS_SI_itemOptionSmallAreaPercent_i)			rv = "itemOption_smallArea_percent";
	else if(t_key == kSDS_SI_itemOptionWidePerfectPercent_i)		rv = "itemOption_widePerfect_percent";
	else if(t_key == kSDS_SI_level_int1_card_i)			rv = "level%d_card";
	else if(t_key == kSDS_SI_level_i)					rv = "level";
	else if(t_key == kSDS_SI_boss_s)					rv = "boss";
	else if(t_key == kSDS_SI_junior_s)					rv = "junior";
	else if(t_key == kSDS_SI_autoBalanceTry_i)			rv = "autoBalanceTry";
	
	else if(t_key == kSDS_CI_int1_rank_i)						rv = "%d_grade";
	else if(t_key == kSDS_CI_int1_grade_i)						rv = "%d_rank";
	else if(t_key == kSDS_CI_int1_durability_i)					rv = "%d_durability";
	else if(t_key == kSDS_CI_int1_theme_i)						rv = "%d_theme";
	else if(t_key == kSDS_CI_int1_stage_i)						rv = "%d_stage";
	else if(t_key == kSDS_CI_int1_reward_i)						rv = "%d_reward";
	else if(t_key == kSDS_CI_int1_missile_type_s)				rv = "%d_missile_type";
	else if(t_key == kSDS_CI_int1_missile_power_i)				rv = "%d_missile_power";
	else if(t_key == kSDS_CI_int1_missile_dex_i)				rv = "%d_missile_dex";
	else if(t_key == kSDS_CI_int1_missile_speed_d)				rv = "%d_missile_speed";
	else if(t_key == kSDS_CI_int1_passive_s)					rv = "%d_passive";
	else if(t_key == kSDS_CI_int1_abilityCnt_i)					rv = "%d_ability_cnt";
	else if(t_key == kSDS_CI_int1_ability_int2_type_i)			rv = "%d_ability_%d_type";
	else if(t_key == kSDS_CI_int1_abilityAttackOptionPower_i)	rv = "%d_ability_attack_option_power";
	else if(t_key == kSDS_CI_int1_abilityAddTimeOptionSec_i)	rv = "%d_ability_addTime_option_sec";
	else if(t_key == kSDS_CI_int1_abilityFastOptionSec_i)		rv = "%d_ability_fast_option_sec";
	else if(t_key == kSDS_CI_int1_abilitySilenceOptionSec_i)	rv = "%d_ability_silence_option_sec";
	else if(t_key == kSDS_CI_int1_abilityDoubleItemOptionPercent_i)			rv = "%d_ability_doubleItem_option_percent";
	else if(t_key == kSDS_CI_int1_abilityLongTimeOptionSec_i)				rv = "%d_ability_longTime_option_sec";
	else if(t_key == kSDS_CI_int1_abilityBossLittleEnergyOptionPercent_i)	rv = "%d_ability_bossLittleEnergy_option_percent";
	else if(t_key == kSDS_CI_int1_abilitySubSmallSizeOptionPercent_i)		rv = "%d_ability_subSmallSize_option_percent";
	else if(t_key == kSDS_CI_int1_abilitySmallAreaOptionPercent_i)			rv = "%d_ability_smallArea_option_percent";
	else if(t_key == kSDS_CI_int1_abilityWidePerfectOptionPercent_i)		rv = "%d_ability_widePerfect_option_percent";
	else if(t_key == kSDS_CI_int1_imgInfo_s)					rv = "%d_imgInfo";
	else if(t_key == kSDS_CI_int1_thumbnailInfo_s)				rv = "%d_thumbnailInfo";
	else if(t_key == kSDS_CI_int1_aniInfoIsAni_b)				rv = "%d_aniInfo_isAni";
	else if(t_key == kSDS_CI_int1_aniInfoDetailLoopLength_i)	rv = "%d_aniInfo_detail_loopLength";
	else if(t_key == kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i)	rv = "%d_aniInfo_detail_loopSeq";
	else if(t_key == kSDS_CI_int1_aniInfoDetailCutWidth_i)		rv = "%d_aniInfo_detail_cutWidth";
	else if(t_key == kSDS_CI_int1_aniInfoDetailCutHeight_i)		rv = "%d_aniInfo_detail_cutHeight";
	else if(t_key == kSDS_CI_int1_aniInfoDetailCutLength_i)		rv = "%d_aniInfo_detail_cutLength";
	else if(t_key == kSDS_CI_int1_aniInfoDetailPositionX_i)		rv = "%d_aniInfo_detail_positionX";
	else if(t_key == kSDS_CI_int1_aniInfoDetailPositionY_i)		rv = "%d_aniInfo_detail_positionY";
	else if(t_key == kSDS_CI_int1_aniInfoDetailImg_s)			rv = "%d_aniInfo_detail_img";
	else if(t_key == kSDS_CI_int1_script_s)						rv = "%d_script";
	else if(t_key == kSDS_CI_int1_silImgInfoIsSil_b)			rv = "%d_silImgInfo_isSil";
	else if(t_key == kSDS_CI_int1_silImgInfoImg_s)				rv = "%d_silImgInfo_img";
	//		else if(t_key == kSDS_CI_int1_silImgInfoSilData_s)			rv = "%d_silImgInfo_silData";
	
	return rv.c_str();
}

void ServerDataSave::fFlush(SaveDataFile f_key){			myDefault->fFlush(f_key);		}
void ServerDataSave::fFlush(SaveDataFile f_key, int i1){	myDefault->fFlush(f_key, i1);	}
void ServerDataSave::fFlush(SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	fFlush(key_set.f_key);
}
void ServerDataSave::fFlush(int i1, SDS_KEY fr_key)
{
	SDS_SET key_set = getKeySet(fr_key);
	fFlush(key_set.f_key, i1);
}

SaveDataFile ServerDataSave::getSDF (SDS_KEY t_key)
{
	SaveDataFile rv;
	if(t_key >= kSDS_PZ_base && t_key <= kSDS_PZ_end)					rv = kSDF_puzzleInfo;
	else if(t_key >= kSDS_GI_base && t_key <= kSDS_GI_end)				rv = kSDF_gameInfo;
	else if(t_key >= kSDS_SI_base && t_key <= kSDS_SI_end)				rv = kSDF_stageInfo;
	else if(t_key >= kSDS_CI_base && t_key <= kSDS_CI_end)				rv = kSDF_cardInfo;
	return rv;
}

void ServerDataSave::removeCache()
{
	sds_cache_bool.clear();
	sds_cache_int.clear();
	sds_cache_float.clear();
	sds_cache_double.clear();
	sds_cache_string.clear();
}

void ServerDataSave::myInit ()
{
	removeCache();
	myDefault = SaveData::sharedObject();
}
#undef LZZ_INLINE
