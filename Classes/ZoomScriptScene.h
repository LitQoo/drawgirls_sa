//
//  ZoomScriptScene.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 6..
//
//

#ifndef __DGproto__ZoomScriptScene__
#define __DGproto__ZoomScriptScene__

#include "cocos2d.h"
#include "DataStorageHub.h"
#include "SilhouetteData.h"
#include <map>

USING_NS_CC;
using namespace std;

class CommonButton;
class ZoomScript : public CCLayer
{
public:
	virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(ZoomScript);
	
	virtual void onEnterTransitionDidFinish();
	
private:
	CCNode* game_node;
	int silType;
	bool is_showtime;
	bool is_exchanged;
	
	int text_length;
	string save_text;
	int typing_frame;
	
	int ing_animation_frame;
	
//	bool is_touched_menu;
	bool is_actioned;
	
	CCSprite* first_img;
	CCLabelTTF* script_label;
	CCSprite* script_case;
	
	CommonButton* next_button;
	CCSprite* showtime_back;
	CCSprite* second_img;
	CCSprite* white_paper;
	
	CCSprite* zoom_img;
	
	
	float minimum_scale;
	CCSize screen_size;
	
	void menuAction(CCObject* sender);
	
	void startScript();
	
	SEL_CallFunc delegate_typing_after;
	void typingAnimation();
	
	void startTouchAction();
	
	void showtimeFirstAction();
	void showtimeSecondAction();
	void showtimeThirdAction();
	void showtimeForthAction();
	
	bool isAnimated;
	CCPoint touch_p;
    long touchStartTime;
	CCPoint touchStart_p;
	CCPoint moveSpeed_p;
	float zoom_base_distance;
	
	map<int, CCPoint> multiTouchData;
	
	CCSize eye_ani_size;
	int loop_length;
	deque<int> animation_frame;
	
	void startStageAnimation();
	
	void frameAnimation();
	
	
	void moveListXY(CCPoint t_p);
	void moveAnimation();
	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher(void);
	
};

#endif /* defined(__DGproto__ZoomScriptScene__) */
