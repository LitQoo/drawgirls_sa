//
//  ContinueContent.h
//  DGproto
//
//  Created by 사원3 on 2014. 2. 3..
//
//

#ifndef __DGproto__ContinueContent__
#define __DGproto__ContinueContent__

#include "cocos2d.h"
#include "cocos-ext.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum ContinueContentMenuTag{
	kContinueContentMenuTag_end = 0,
	kContinueContentMenuTag_continue
};
class ContinueContent : public CCNode
{
public:
	static ContinueContent* create(int t_touch_priority, function<void(void)> t_end, function<void(void)> t_continue)
	{
		ContinueContent* t_ctc = new ContinueContent();
		t_ctc->myInit(t_touch_priority, t_end, t_continue);
		t_ctc->autorelease();
		return t_ctc;
	}
	
	void startShow()
	{
		setScale(0);
		
		CCScaleTo* t_scale = CCScaleTo::create(0.3f, 1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::endShow));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		runAction(t_seq);
	}
	
private:
	int touch_priority;
	function<void(void)> end_selector;
	function<void(void)> continue_selector;
	
	bool is_menu_enable;
	bool is_continue;
	
	void menuAction(CCObject* sender);
	
	void endShow()
	{
		is_menu_enable = true;
	}
	void startHide()
	{
		CCScaleTo* t_scale = CCScaleTo::create(0.3f, 0.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::endHide));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		runAction(t_seq);
	}
	void endHide()
	{
		if(is_continue)
			continue_selector();
		else
			end_selector();
	}
	
	void myInit(int t_touch_priority, function<void(void)> t_end, function<void(void)> t_continue);
};

#endif /* defined(__DGproto__ContinueContent__) */
