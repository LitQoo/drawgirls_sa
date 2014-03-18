//
//  MiniGamePopup.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 30..
//
//

#ifndef __DGproto__MiniGamePopup__
#define __DGproto__MiniGamePopup__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum MiniGameCode{
	kMiniGameCode_touchtouch = 0
};

class CountingBMLabel;
class CCMenuLambda;
class MiniGamePopup : public CCLayer
{
public:
    // preprocessor macro for "static create()" constructor ( node() deprecated )
	
	static MiniGamePopup* create(MiniGameCode t_code, function<void(void)> t_end_func);
	//    CREATE_FUNC(GachaPurchase);
	
	virtual void onEnter();
	
	virtual bool init(MiniGameCode t_code, function<void(void)> t_end_func);
	
private:
	std::function<void(void)> startMiniGame;
	bool is_menu_enable;
	void menuAction(CCObject* pSender);
	
	std::function<void(void)> end_function;
	std::function<void(void)> miniGameStart;
	MiniGameCode game_code;
	
	CCSprite* left_curtain;
	CCSprite* right_curtain;
	CCSprite* title_bonusgame;
	CCSprite* content_back;
	CCMenu* start_menu;
	
	void showPopup();
	void endShowPopup();
	
	void hidePopup(CCObject* pSelectorTarget, SEL_CallFunc selector, int ruby, int gold, int social);
	void endHidePopup();
	
	void startGame();
	
	int keep_gold;
	int keep_ruby;
	int keep_social;
	
	CCSprite* reward_title;
	CCSprite* reward_case;
	CCMenuLambda* ok_menu;
	
	CountingBMLabel* reward_gold;
	CountingBMLabel* reward_ruby;
	CountingBMLabel* reward_social;
	
	void startCalcAction();
	void setCalcInfo();
	void startOutAction();
	
	virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
	virtual void registerWithTouchDispatcher ();
};

#endif /* defined(__DGproto__MiniGamePopup__) */
