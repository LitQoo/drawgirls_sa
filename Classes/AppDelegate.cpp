#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"
#include "Lua_extensions_CCB.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "Lua_web_socket.h"
#endif

#include "GameData.h"
#include "TitleRenewal.h"

using namespace CocosDenshion;

USING_NS_CC;

#include <functional>
#include <map>
#include "KSUtil.h"
#include "Well512.h"
#include "Dodge.h"
#include "RandomDistribution.h"
#include "CountingGame.h"


#include "SlidingPuzzle.h"
#include "CardMatching.h"
#include "KSProtect.h"
#include "ks19937.h"

//#include <boost/graph/graphviz.hpp>
/*
 메테오 : AP_Missile16
 AP_Missile15 불지르기
 AP_Missile34 인비지블
 
 AP_Missile12 위성빔
 AP_Missile6  레이저 스캔
 AP_Missile22 방사능.
 
 */
AppDelegate::AppDelegate()
{
	KSProtectVar<float> testVar(15.f);
	KSProtectVar<float> testVar2(20.f);
	testVar += 3.f;
	testVar -= 5.f;
	
	CCLog("%f", testVar.getV()); // 값 얻기.
	CCLog("%f", testVar + testVar2); // 보호된 변수끼리 연산가능.
	CCLog("%d", testVar < testVar2); // 비교가능.
	CCLog("%d", testVar < 3.f); // 상수값 비교 가능.
	
	KSProtectStr testStr("testtest");
	CCLog("%s", testStr.getV().c_str());
	//	std::random_device rd;
	//	std::mt19937 rEngine(rd());
	//	uniform_int_distribution<long long> dist(1000000000000000000L, 2000000000000000000L);
	//
	//	for(int i=0; i<20; i++)
	//	{
	//		long long number = dist(rEngine);
	//		CCLog("%lld", number);
	//		CCLog("%s", KS::longLongToStr(number).c_str());
	//		CCLog("\t%lld", KS::strToLongLong(KS::longLongToStr(number)));
	//		CCLog("%s", KS::longLongToStr(number).c_str());
	//		CCLog("\t%lld", KS::strToLongLong(KS::longLongToStr(number)));
	//		CCLog("");
	//	}
	//	CCLog("f");
}

AppDelegate::~AppDelegate()
{
	SimpleAudioEngine::end();
}

//static char sharedResourceDirectory[100];
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(480, 320);

struct AA
{
	int a;
	int b;
};
bool AppDelegate::applicationDidFinishLaunching()
{
	
	ks19937::seed(std::random_device()());
	// initialize director
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
	//	pDirector->setProjection(kCCDirectorProjection2D);
	
	CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	//	pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);
	pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionFixedWidth);// kResolutionNoBorder);
	//	CCFileUtils::sharedFileUtils()->setResourceDirectory(sharedResourceDirectory);
	
	CCSize screen_size = pEGLView->getFrameSize();
	myDSH->ui_top = 480.f*screen_size.height/screen_size.width;
	myDSH->ui_bottom = 0;
	myDSH->ui_center_y = myDSH->ui_top/2.f;
	myDSH->puzzle_ui_top = 320.f;
	
	float screen_rate = screen_size.width/screen_size.height;
	if(screen_rate > 1.5f)
	{
		myDSH->screen_convert_rate = (designResolutionSize.width/designResolutionSize.height)/screen_rate;
		myDSH->ui_zero_point = CCPointZero;
		
		float t_value = screen_size.height/designResolutionSize.height;
		myDSH->mult_point.x = screen_size.width/t_value/designResolutionSize.width;
		myDSH->add_point.x = -(screen_size.width-designResolutionSize.width*t_value)/2.f/t_value;
		
		myDSH->mult_point.y = screen_size.width/designResolutionSize.width/t_value;
		myDSH->add_point.y = 0.f;
	}
	else if(screen_rate < 1.5f)
	{
		myDSH->screen_convert_rate = 1.f;
		myDSH->ui_zero_point = ccp(0,(screen_size.height*(designResolutionSize.width/screen_size.width) - designResolutionSize.height)/2.f);
		myDSH->puzzle_ui_top = myDSH->ui_top;
		
		myDSH->mult_point.x = 1.f;
		myDSH->add_point.x = 0.f;
		
		myDSH->mult_point.y = 1.f;
		myDSH->add_point.y = -(screen_size.height*(designResolutionSize.width/screen_size.width) - designResolutionSize.height)/2.f;
	}
	else
	{
		myDSH->screen_convert_rate = 1.f;
		myDSH->ui_zero_point = CCPointZero;
		
		myDSH->mult_point.x = 1.f;
		myDSH->add_point.x = 0.f;
		
		myDSH->mult_point.y = 1.f;
		myDSH->add_point.y = 0.f;
	}
	
	CCPoint ui_touch_convert = ccp(0,screen_size.height*(designResolutionSize.width/screen_size.width) - designResolutionSize.height);
	myDSH->ui_touch_convert = ui_touch_convert;
	
	
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ccb");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ccb/ccbResources");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ksoo");
	//	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ccb_test");
	//	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ccb_test/resources-iphonehd");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ccb/resources-iphonehd");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_flow");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_tutorial");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_jack");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_jmissile");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_pattern");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_pause");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_skill");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_stage");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_img/img_ingame/ingame_ui");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_sound/sound_bgm");
	CCFileUtils::sharedFileUtils()->addSearchPath("res_sound/sound_effect");
	CCFileUtils::sharedFileUtils()->addSearchPath("fonts");
	
	//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	//	CCSize frameSize = pEGLView->getFrameSize();
	//
	//	float basicRate = 1.5; // iphone, ipod lower than version 5
	//	//	float upperThan5 = 1.775;
	//	//	float ipadRate = 1024.f/768.f;
	//	float frameRate = frameSize.width/frameSize.height;
	//
	//	CCSize calcFrameSize;
	//
	//	if(frameRate == basicRate) // iphone, ipod lower than version 5
	//	{
	//		calcFrameSize = designResolutionSize;
	//		DataStorageHub::sharedInstance()->device_rate = 1.f;
	//	}
	//	else if(frameRate > basicRate) // iphone, ipod upper than version 5 // upperThan5
	//	{
	//		float screen_rate = frameSize.height/designResolutionSize.height;
	//		calcFrameSize = CCSizeMake(frameSize.width/screen_rate, frameSize.height/screen_rate);
	//		DataStorageHub::sharedInstance()->device_rate = 1.f;
	//		DataStorageHub::sharedInstance()->device_margine = CCSizeMake(fabsf(frameSize.width-(960.f*DataStorageHub::sharedInstance()->device_rate))/2.f, 0);
	//	}
	//	else if(frameRate < basicRate) // ipad // ipadRate
	//	{
	//		calcFrameSize = CCSizeMake(1024, 768);
	//		DataStorageHub::sharedInstance()->device_rate = frameSize.width/960.f;
	//		DataStorageHub::sharedInstance()->device_margine = CCSizeMake(0, fabsf(frameSize.height-(640.f*DataStorageHub::sharedInstance()->device_rate))/2.f);
	//		if(frameSize.height == calcFrameSize.height)
	//		{
	//			DataStorageHub::sharedInstance()->setIsNotRetinaIpad(true);
	//		}
	//	}
	//	else
	//	{
	//		calcFrameSize = designResolutionSize;
	//		DataStorageHub::sharedInstance()->device_rate = 1.f;
	//	}
	//
	//	CCSize imgSize = CCSizeMake(960, 640);
	//
	//	if(basicRate < frameRate) // height long, calc : width
	//		pDirector->setContentScaleFactor(imgSize.height/calcFrameSize.height);
	//	else if(basicRate > frameRate) // width long, calc : height
	//		pDirector->setContentScaleFactor(2.0); // only ipad
	//	else
	//		pDirector->setContentScaleFactor(imgSize.height/calcFrameSize.height);
	//
	//
	//
	//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	//	pDirector->setContentScaleFactor(2.0);
	//
	//	CCSize frameSize = pEGLView->getFrameSize();
	//
	//	float basicRate = 1.5; // iphone, ipod lower than version 5
	//	float frameRate = frameSize.width/frameSize.height;
	//
	//	CCSize imgSize = CCSizeMake(960, 640);
	//
	//	if(basicRate < frameRate) // height long, calc : width
	//	{
	//		myDSH->device_rate = frameSize.height/imgSize.height;
	//		myDSH->device_margine = CCSizeMake(fabsf(frameSize.width-(imgSize.width*myDSH->device_rate))/2.f, 0);
	//	}
	//	else if(basicRate > frameRate) // width long, calc : height
	//	{
	//		myDSH->device_rate = frameSize.width/imgSize.width;
	//		myDSH->device_margine = CCSizeMake(0, fabsf(frameSize.height-(imgSize.height*myDSH->device_rate))/2.f);
	//	}
	//	else
	//	{
	//		myDSH->device_rate = frameSize.width/imgSize.width;
	//		myDSH->device_margine = CCSizeMake(0, 0);
	//	}
	//
	//#endif
	
	pDirector->setContentScaleFactor(2.0);
	
	// turn on display FPS
	pDirector->setDisplayStats(false);
	
	// set FPS. the default value is 1.0/60 if you don't call this
	pDirector->setAnimationInterval(1.0 / 60);
	
	// register lua engine
	//    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
	//    CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
	//
	//    CCLuaStack *pStack = pEngine->getLuaStack();
	//    lua_State *tolua_s = pStack->getLuaState();
	//    tolua_extensions_ccb_open(tolua_s);
	//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//    pStack = pEngine->getLuaStack();
	//    tolua_s = pStack->getLuaState();
	//    tolua_web_socket_open(tolua_s);
	//#endif
	//
	//    std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("hello.lua");
	//    pEngine->executeScriptFile(path.c_str());
	
	
	//	CCScene* pScene = TitleScene::scene();
		CCScene* pScene = TitleRenewalScene::scene();
	//	CCScene* pScene = Dodge::scene();
	//	CCScene* pScene = CountingGame::scene();
	//	CCScene* pScene = SlidingPuzzle::scene();
//	CCScene* pScene = CardMatching::scene();
	//	pEGLView->setDesignResolutionSize(480, 480, kResolutionFixedWidth);// kResolutionNoBorder);
	//	CCScene* pScene = RandomDistribution::scene();
	pDirector->runWithScene(pScene);
	
	
	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
	CCDirector::sharedDirector()->stopAnimation();
	
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	CCDirector::sharedDirector()->startAnimation();
	
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
