//
//  FriendData.h
//  DGproto
//
//  Created by ksoo k on 2013. 12. 5..
//
//

#pragma once

#include <string>
#include "jsoncpp/json.h"
#include <boost/format.hpp>
#include <sstream>

struct FriendData
{
	std::string userId;
	std::string nick;
	
	int64_t joinDate;
	int64_t lastDate;
	int64_t lastTime; // 형식이 좀 다름.
	std::string profileUrl;
	bool messageBlocked;
	Json::Value userData;
	std::string hashedTalkUserId;
	bool unknownFriend;
	Json::Value extraData; // 여분의 공간, 필요하면 이 공간을 쓰면 됨.
	friend std::ostream &operator<<(std::ostream &output, const FriendData &a)
	{
		std::ostringstream oss;
		oss << a.userData;
		std::ostringstream oss2;
		oss2 << a.extraData;
		auto ta = boost::str(boost::format("\n\tuserId = %||\n\tnick = %||\n\tunknown = %||\n\tuserData = %||\n\t"
																			"joinDate = %||\n\tlastDate = %||\n\thashId = %||\n\tprofileUrl = %||\n\t"
																			"messageBlocked = %||\n\textraData = %||\n\t") % a.userId
												% a.nick % a.unknownFriend % oss.str() % a.joinDate  % a.lastDate %
												a.hashedTalkUserId % a.profileUrl.c_str() % a.messageBlocked % oss2.str().c_str());
		output << ta << std::endl;
		return output;
	}
	FriendData()
	{
		joinDate = lastDate = lastTime = 0;
		messageBlocked = false;
		unknownFriend = false;
	}
};


