//
//  TakeCardAnimation.h
//  DGproto
//
//  Created by LitQoo on 2014. 1. 29..
//
//

#ifndef __DGproto__TakeCardAnimation__
#define __DGproto__TakeCardAnimation__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "KSUtil.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "GDWebSprite.h"
#include "CommonButton.h"

using namespace cocos2d::extension;
using namespace std;

class TakeCardAnimation : public CCLayer{
	CCSprite* gray;
	CCLayer* roundBig;
	CCLayer* roundSmall;
	CCSprite* card;
	CCSprite* cardLight;
	CCSprite* title;
	int touch_priority;
	bool isOpening;
	float kScale;
public:
//	CREATE_FUNC(TakeCardAnimation);
	//CC_SYNTHESIZE(CCNode*, m_contentNode, ContentNode);
	
	static TakeCardAnimation* create(CCSprite* _card, int _touch_priority){
		TakeCardAnimation* ret = new TakeCardAnimation();
		ret->init(_card,_touch_priority);
		ret->autorelease();
		return ret;
	}
	
	bool init(CCSprite* _card,int _touch_priority){
		if(CCLayer::init()==false)return false;
		
		isOpening=true;
		touch_priority=_touch_priority;
		setTouchEnabled(true);
		
		kScale = 141/_card->getContentSize().width;
		
		
		card=_card;
		card->setPosition(ccp(240,155));
		card->setVisible(false);
		card->setScale(kScale*0.7);
		
		this->addChild(card,3);
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		title = CCSprite::create("ani_title_takecard.png");
		title->setPosition(ccp(240,276));
		title->setScale(0);
		this->addChild(title,2);
		
		gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(240,160));
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		addChild(gray, 1);
		
		auto ret = KS::loadCCBI<CCLayer*>(this, "roundBlue1.ccbi");
		roundBig = CCLayer::create();
		roundBig->addChild(ret.first);
		roundBig->setPosition(ccp(0,-95));
		roundBig->setScale(0);
		this->addChild(roundBig,2);
		
		auto ret2 = KS::loadCCBI<CCLayer*>(this, "roundBlue2.ccbi");
		roundSmall = CCLayer::create();
		roundSmall->addChild(ret2.first);
		roundSmall->setPosition(ccp(0,-95));
		roundSmall->setScale(0);
		this->addChild(roundSmall,2);
		
		cardLight = CCSprite::create("cardLightCoverBlue.png");
		cardLight->setScale(0.7);
		cardLight->setPosition(ccp(240,500));
		
		this->addChild(cardLight,4);

		return true;
	}
	std::function<void(void)> closeFunc;
	
	void setCloseFunc(std::function<void(void)> func){
		closeFunc = func;
	}
	
	void start(){
		isOpening=true;
		gray->runAction(CCSequence::create(
										   CCFadeIn::create(0.5f),
										   CCCallFunc::create(this, callfunc_selector(TakeCardAnimation::step2))
										   ,NULL
										   )
						);
		
	}
	
	void step2(){
		
		roundBig->runAction(
							CCSequence::create(
											   CCScaleTo::create(8/30.f, 1, 0.2f),
											   CCDelayTime::create(0.2f),
											   CCCallFunc::create(this, callfunc_selector(TakeCardAnimation::step3))
											   ,NULL
											   )
							);
		
		roundSmall->runAction(CCScaleTo::create(8/60.f, 1, 0.2f));
			
	}
	
	void step3(){
		
		cardLight->runAction(
							 CCSequence::create(
												CCMoveTo::create(7/30.f,ccp(240,150)),
												CCMoveTo::create(7/30.f,ccp(240,155)),
												CCCallFunc::create(this, callfunc_selector(TakeCardAnimation::step4)),
												NULL
												)
							 );
		

	}
	
	void step4(){
		title->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,1)));
		
		cardLight->runAction(
							 CCFadeOut::create(16/30.f)
							 );
		card->setVisible(true);
		card->runAction(
						CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f,ccp(0,5)),CCMoveBy::create(0.5f,ccp(0,-5)),NULL))
						);
		
		isOpening=false;
	}
	
	void close(){
		isOpening=true;
		
		title->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,0.f)));
		
		card->runAction(CCMoveBy::create(0.2f,ccp(0,300)));
		roundSmall->runAction(CCScaleTo::create(0.3f,0.f));
		
		roundBig->runAction(
							CCSequence::create(
											   CCScaleTo::create(0.4f,0),
											   CCCallFunc::create(this, callfunc_selector(TakeCardAnimation::closeStep2)),
											   NULL
											   )
							);
		
		
		
	}
							
	void closeStep2(){
		gray->runAction(
						CCSequence::create(
										   CCFadeOut::create(0.3f),
										   CCCallFunc::create(this, callfunc_selector(TakeCardAnimation::closeStep3)),
										   NULL
										   )
						);
	}
						
	void closeStep3(){
		isOpening=false;
		this->removeFromParent();
		if(closeFunc)closeFunc();
	}
	
	void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, touch_priority, true);
		
	}
	
	
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
		if(isOpening==false)this->close();
		return true;
	}
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DownCardAnimation : public CCLayer{
	CCSprite* gray;
	CCLayer* roundBig;
	CCLayer* roundSmall;
	CCSprite* card;
	CCSprite* cardLight;
	CCSprite* title;
	int touch_priority;
	bool isOpening;
	float kScale;
public:
	//	CREATE_FUNC(TakeCardAnimation);
	//CC_SYNTHESIZE(CCNode*, m_contentNode, ContentNode);
	
	static DownCardAnimation* create(CCSprite* _card, int _touch_priority){
		DownCardAnimation* ret = new DownCardAnimation();
		ret->init(_card,_touch_priority);
		ret->autorelease();
		return ret;
	}
	
	bool init(CCSprite* _card,int _touch_priority){
		
		if(CCLayer::init()==false)return false;
		isOpening=true;
		touch_priority=_touch_priority;
		setTouchEnabled(true);
		
		kScale = 141/_card->getContentSize().width;
		
		card=_card;
		
		card->setPosition(ccp(240,155));
		card->setVisible(false);
		card->setScale(kScale*0.7);
		this->addChild(card,3);
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		title = CCSprite::create("ani_title_downcard.png");
		title->setPosition(ccp(240,276));
		title->setScale(0);
		this->addChild(title,2);
		
		gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(240,160));
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		addChild(gray, 1);
		
		auto ret = KS::loadCCBI<CCLayer*>(this, "roundRed1.ccbi");
		roundBig = CCLayer::create();
		roundBig->addChild(ret.first);
		roundBig->setPosition(ccp(0,-95));
		roundBig->setScale(0);
		this->addChild(roundBig,2);
		
		auto ret2 = KS::loadCCBI<CCLayer*>(this, "roundRed2.ccbi");
		roundSmall = CCLayer::create();
		roundSmall->addChild(ret2.first);
		roundSmall->setPosition(ccp(0,-95));
		roundSmall->setScale(0);
		this->addChild(roundSmall,2);
		
		cardLight = CCSprite::create("cardLightCoverRed.png");
		cardLight->setScale(0.7);
		cardLight->setPosition(ccp(240,500));
		
		this->addChild(cardLight,4);
		
		return true;
	}
	std::function<void(void)> closeFunc;
	
	void setCloseFunc(std::function<void(void)> func){
		closeFunc = func;
	}
	void start(){
		isOpening=true;
		gray->runAction(CCSequence::create(
										   CCFadeIn::create(0.5f),
										   CCCallFunc::create(this, callfunc_selector(DownCardAnimation::step2)),
										   CCDelayTime::create(0.2f)
										   ,NULL
										   )
						);
		
		
		
		
		
		
	}
	
	void step2(){
		
		roundBig->runAction(
							CCSequence::create(
											   CCScaleTo::create(8/30.f, 1, 0.2f),
											   CCDelayTime::create(0.2f),
											   CCCallFunc::create(this, callfunc_selector(DownCardAnimation::step3))
											   ,NULL
											   )
							);
		
		roundSmall->runAction(CCScaleTo::create(8/30.f, 1, 0.2f));
		
		
	}
	
	void step3(){
		
		cardLight->runAction(
							 CCSequence::create(
												CCMoveTo::create(7/30.f,ccp(240,150)),
												CCMoveTo::create(7/30.f,ccp(240,155)),
												CCCallFunc::create(this, callfunc_selector(DownCardAnimation::step4)),
												NULL
												)
							 );
		
		
	}
	
	void step4(){
		title->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,1)));
		
		cardLight->runAction(
							 CCFadeOut::create(16/30.f)
							 );
		card->setVisible(true);
		card->runAction(
						CCRepeatForever::create(CCSequence::create(CCTintTo::create(5/30.f, 255, 100, 100),CCTintTo::create(5/30.f, 255, 255, 255),CCMoveBy::create(0.5f,ccp(0,5)),CCMoveBy::create(0.5f,ccp(0,-5)),NULL))
						);
		
		isOpening=false;
	}
	
	void close(){
		isOpening=true;
		
		title->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,0)));
		
		card->runAction(CCMoveBy::create(0.2f,ccp(0,300)));
		
		roundSmall->runAction(CCScaleTo::create(0.3f,0));
		
		roundBig->runAction(
							CCSequence::create(
											   CCScaleTo::create(0.4f,0),
											   CCCallFunc::create(this, callfunc_selector(DownCardAnimation::closeStep2)),
											   NULL
											   )
							);
		
	}
	
	void closeStep2(){
		gray->runAction(
						CCSequence::create(
										   CCFadeOut::create(0.3f),
										   CCCallFunc::create(this, callfunc_selector(DownCardAnimation::closeStep3)),
										   NULL
										   )
						);
	}
	
	void closeStep3(){
		isOpening=false;
		this->removeFromParent();
		if(closeFunc)closeFunc();
	}
	
	void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, touch_priority, true);
		
	}
	
	
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
		if(isOpening==false)this->close();
		return true;
	}
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RemoveCardAnimation : public CCLayer{
	CCSprite* gray;
	CCLayer* roundBig;
	CCLayer* roundSmall;
	CCSprite* card;
	CCSprite* cardLight;
	CCSprite* title;
	CCSprite* stencil;
	int touch_priority;
	bool isOpening;
	CCParticleSystemQuad* particle;
	float removeSpeed;
	float kScale;
	CommonButton* skipBtn;
	CommonButton* repairBtn;
	
public:
	//	CREATE_FUNC(TakeCardAnimation);
	//CC_SYNTHESIZE(CCNode*, m_contentNode, ContentNode);
	
	static RemoveCardAnimation* create(CCSprite* _card, int _touch_priority){
		RemoveCardAnimation* ret = new RemoveCardAnimation();
		ret->init(_card,_touch_priority);
		ret->autorelease();
		return ret;
	}
	
	bool init(CCSprite* _card,int _touch_priority){
		if(CCLayer::init()==false)return false;
		isOpening=true;
		touch_priority=_touch_priority;
		setTouchEnabled(true);
		kScale = 141/_card->getContentSize().width;
		
		
		removeSpeed=0.2f;
		card = _card;
		card->setPosition(ccp(240,175));
		card->setScale(kScale*0.7);
		card->setVisible(false);
		//this->addChild(card,3);
		
		
		stencil = CCSprite::create("back_gray.png");
		stencil->setAnchorPoint(ccp(0.5,0));
		stencil->setPosition(ccp(240,320));
		
		CCClippingNode *cliper = CCClippingNode::create(stencil);
		//you want to hide intersection so we setInverted to true
		cliper->setInverted(true);
		cliper->addChild(card);
		addChild(cliper,3);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		title = CCSprite::create("ani_title_removecard.png");
		title->setPosition(ccp(240,276));
		title->setScale(0);
		this->addChild(title,2);
		
		gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(240,160));
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		addChild(gray, 1);
		
		auto ret = KS::loadCCBI<CCLayer*>(this, "roundRed1.ccbi");
		roundBig = CCLayer::create();
		roundBig->addChild(ret.first);
		roundBig->setPosition(ccp(0,-75));
		roundBig->setScale(0);
		this->addChild(roundBig,2);
		
		auto ret2 = KS::loadCCBI<CCLayer*>(this, "roundRed2.ccbi");
		roundSmall = CCLayer::create();
		roundSmall->addChild(ret2.first);
		roundSmall->setPosition(ccp(0,-75));
		roundSmall->setScale(0);
		this->addChild(roundSmall,2);
		
		cardLight = CCSprite::create("cardLightCoverRed.png");
		cardLight->setScale(0.7);
		cardLight->setPosition(ccp(240,500));
		
		this->addChild(cardLight,4);
		
		
		particle = CCParticleSystemQuad::createWithTotalParticles(150);
		particle->setPositionType(kCCPositionTypeRelative);
		//particle->setAutoRemoveOnFinish(true);
		particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("fever_particle.png"));
		particle->setEmissionRate(5000);
		particle->setAngle(0.0);
		particle->setAngleVar(0.0);
		ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
		particle->setBlendFunc(blendFunc);
		particle->setDuration(-1);
		particle->setEmitterMode(kCCParticleModeGravity);
		particle->setStartColor(ccc4f(1.f, 1.f, 1.f, 0.f));
		particle->setStartColorVar(ccc4f(0,0,0,1.f));
		particle->setEndColor(ccc4f(1.f,0.f,0.f,0.f));
		particle->setEndColorVar(ccc4f(0, 0, 0, 1.f));
		particle->setStartSize(8.0);
		particle->setStartSizeVar(5.0);
		particle->setEndSize(20.0);
		particle->setEndSizeVar(10.0);
		particle->setGravity(ccp(240,160));
		particle->setRadialAccel(-671.0);
		particle->setRadialAccelVar(0.0);
		particle->setSpeed(51);
		particle->setSpeedVar(300.0);
		particle->setTangentialAccel(0);
		particle->setTangentialAccelVar(0);
		particle->setTotalParticles(150);
		particle->setLife(0.03);
		particle->setLifeVar(0.1);
		particle->setStartSpin(0.0);
		particle->setStartSpinVar(0.f);
		particle->setEndSpin(360.0);
		particle->setEndSpinVar(0.f);
		particle->setPosVar(ccp(50,0));
		particle->setPosition(ccp(240,240));		//
		particle->setVisible(false);
		addChild(particle, 100);
		
		repairBtn = CommonButton::create("구원", 15, CCSizeMake(90,45), CommonButtonBlue, touch_priority-1);
		repairBtn->setFunction([=](CCObject *){this->stopRemove();});
		repairBtn->setPrice(PriceTypeRuby, 1);
		repairBtn->setTitleColor(ccc3(0,0,0));
//		repairBtn = CCControlButton::create("고치기", mySGD->getFont().c_str(), 20);
//		repairBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(RemoveCardAnimation::stopRemove), CCControlEventTouchUpInside);
//		
		repairBtn->setPosition(ccp(310,35));
		repairBtn->setScale(0);
		addChild(repairBtn,9);
		
		skipBtn = CommonButton::create("버리기", 15, CCSizeMake(90,45), CommonButtonYellow, touch_priority-1);
		skipBtn->setFunction([=](CCObject *){this->skip();});
		skipBtn->setPosition(ccp(170,35));
		skipBtn->setTouchPriority(touch_priority-1);
		skipBtn->setTitleColor(ccc3(0,0,0));
		skipBtn->setScale(0);
		addChild(skipBtn,9);
		
		skipFunc = NULL;
		repairFunc = NULL;
		closeFunc = NULL;
		
		return true;
	}
	
	std::function<void(void)> skipFunc;
	std::function<void(void)> repairFunc;
	std::function<void(void)> closeFunc;
	
	void setSkipFunc(std::function<void(void)> func){
		skipFunc = func;
	}
	
	void setRepairFunc(std::function<void(void)> func){
		repairFunc = func;
	}
	
	void setCloseFunc(std::function<void(void)> func){
		closeFunc = func;
	}
	
	void repair(){
		
		particle->setVisible(true);
		this->schedule(schedule_selector(RemoveCardAnimation::repairSchedule));
		
	}
	
	void skip(){
		removeSpeed+=4;
		
		
		repairBtn->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,0)));
		skipBtn->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,0)));
		
		if(skipFunc)skipFunc();
	}
	
	
	void stopRemove(){
		this->unschedule(schedule_selector(RemoveCardAnimation::removeSchedule));
		
		
		repairBtn->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,0)));
		skipBtn->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,0)));
		
		
		if(repairFunc)repairFunc();
	}
	
	
	void start(){
		isOpening=true;
		
		gray->runAction(CCSequence::create(
										   CCFadeIn::create(0.5f),
										   CCCallFunc::create(this, callfunc_selector(RemoveCardAnimation::step2)),
										   CCDelayTime::create(0.2f)
										   ,NULL
										   )
						);
		
		
		
		

		
		
	}
	
	void step2(){
		
		roundBig->runAction(
							CCSequence::create(
											   CCScaleTo::create(8/30.f, 1, 0.2f),
											   CCDelayTime::create(0.2f),
											   CCCallFunc::create(this, callfunc_selector(RemoveCardAnimation::step3))
											   ,NULL
											   )
							);
		
		roundSmall->runAction(CCScaleTo::create(8/30.f, 1, 0.2f));
		
		
	}
	
	void step3(){
		
		cardLight->runAction(
							 CCSequence::create(
												CCMoveTo::create(7/30.f,ccp(240,170)),
												CCMoveTo::create(7/30.f,ccp(240,175)),
												CCCallFunc::create(this, callfunc_selector(RemoveCardAnimation::step4)),
												NULL
												)
							 );
		
		
	}
	
	void step4(){
		
		title->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,1)));
		
		repairBtn->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,1)));
		skipBtn->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,1)));
		
		cardLight->runAction(
							 CCFadeOut::create(16/30.f)
							 );
		card->setVisible(true);
		card->runAction(
						CCRepeatForever::create(CCSequence::create(CCTintTo::create(5/30.f, 255, 100, 100),CCTintTo::create(5/30.f, 255, 255, 255),CCMoveBy::create(0.5f,ccp(0,5)),CCMoveBy::create(0.5f,ccp(0,-5)),NULL))
						);
		this->runAction(CCSequence::create(CCDelayTime::create(0.1),CCCallFunc::create(this, callfunc_selector(RemoveCardAnimation::removeContinue)),NULL));
	}
	
	void removeContinue(){
		particle->setVisible(true);
		isOpening=false;
		
		this->schedule(schedule_selector(RemoveCardAnimation::removeSchedule));
		
		
	}
	
	void removeSchedule(float dt){
		float y =  particle->getPositionY();
		particle->setPositionY(y-removeSpeed);
		stencil->setPositionY(y-removeSpeed);
		if(y-removeSpeed<card->getPositionY()-(card->getContentSize().height/2*card->getScale())){
			particle->setVisible(false);
			doRemoveCard();
			this->unschedule(schedule_selector(RemoveCardAnimation::removeSchedule));
		}
	}
		
	void repairSchedule(float dt){
		float y =  particle->getPositionY();
		particle->setPositionY(y+1);
		stencil->setPositionY(y+1);
		if(y+1>=card->getPositionY()+(card->getContentSize().height/2*card->getScale())){
			stencil->setPositionY(500);
			particle->setVisible(false);
			doRepair();
			this->unschedule(schedule_selector(RemoveCardAnimation::repairSchedule));
		}
	}
	
	void doRepair(){
		//CCLog("doRepair");
		close();
	}
	
	void doRemoveCard(){
		//CCLog("doRemoveCard");
		close();
	}
	
	void close(){
		isOpening=true;
		
		title->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3,0)));
		
		
		card->runAction(CCMoveBy::create(0.2f,ccp(0,300)));
		roundSmall->runAction(CCScaleTo::create(0.3f,0));
		roundBig->runAction(
							CCSequence::create(
											   CCScaleTo::create(0.4f,0),
											   CCCallFunc::create(this, callfunc_selector(RemoveCardAnimation::closeStep2)),
											   NULL
											   )
							);
		
	}
	
	void closeStep2(){
		gray->runAction(
						CCSequence::create(
										   CCFadeOut::create(0.3f),
										   CCCallFunc::create(this, callfunc_selector(RemoveCardAnimation::closeStep3)),
										   NULL
										   )
						);
	}
	
	void closeStep3(){
		isOpening=false;
		this->removeFromParent();
		if(closeFunc)closeFunc();
	}
	
	void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, touch_priority, true);
		
	}
	
	
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
		//removeSpeed++;
		//stopRemove();
		//if(isOpening==false)this->close();
		return true;
	}
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class StrengthCardAnimation : public CCLayer{
	CCSprite* gray;
	CCLayer* roundBig;
	CCLayer* roundSmall;
	CCSprite* card;
	CCSprite* card2;
	CCSprite* cardLight;
	CCSprite* title;
	CCSprite* titleSuccess;
	CCSprite* titleFail;
	CCLabelTTF* optionLbl;
	int touch_priority;
	bool isOpening;
	float kScale;
	bool isSuccess;
public:
	//	CREATE_FUNC(TakeCardAnimation);
	//CC_SYNTHESIZE(CCNode*, m_contentNode, ContentNode);
	
	static StrengthCardAnimation* create(CCSprite* _card,CCSprite* _card2, int _touch_priority){
		StrengthCardAnimation* ret = new StrengthCardAnimation();
		ret->init(_card,_card2,_touch_priority);
		ret->autorelease();
		return ret;
	}
	
	bool init(CCSprite* _card,CCSprite* _card2,int _touch_priority){
		if(CCLayer::init()==false)return false;
		
		//141,188
		kScale = 141/_card->getContentSize().width;
		isSuccess=true;
		isOpening=true;
		touch_priority=_touch_priority;
		setTouchEnabled(true);
		card=_card;
		card->setPosition(ccp(240,175));
		card->setVisible(false);
		card->setScale(kScale*0.7);
		this->addChild(card,3);
		
		optionLbl = CCLabelTTF::create("", mySGD->getFont().c_str(), 15);
		optionLbl->setPosition(ccp(240,35));
		optionLbl->setScale(0);
		this->addChild(optionLbl,4);
		
		card2=_card2;
		card2->setPosition(ccp(240,-410));
		card2->setAnchorPoint(ccp(0.5,0));
		this->addChild(card2,5);
		
		//kScale = card2->getScale();
		card2->setScale(kScale*2);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		title = CCSprite::create("ani_title_strength.png");
		title->setPosition(ccp(240,276));
		title->setScale(0);
		this->addChild(title,2);
		
		titleSuccess = CCSprite::create("ani_title_s_success.png");
		titleSuccess->setPosition(ccp(240,276));
		titleSuccess->setScale(0);
		this->addChild(titleSuccess,2);
		
		titleFail = CCSprite::create("ani_title_s_fail.png");
		titleFail->setPosition(ccp(240,276));
		titleFail->setScale(0);
		this->addChild(titleFail,2);
		
		gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(240,160));
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		addChild(gray, 1);
		
		auto ret = KS::loadCCBI<CCLayer*>(this, "roundBlue1.ccbi");
		roundBig = CCLayer::create();
		roundBig->addChild(ret.first);
		roundBig->setPosition(ccp(0,-75));
		roundBig->setScale(0);
		this->addChild(roundBig,2);
		
		auto ret2 = KS::loadCCBI<CCLayer*>(this, "roundBlue2.ccbi");
		roundSmall = CCLayer::create();
		roundSmall->addChild(ret2.first);
		roundSmall->setPosition(ccp(0,-75));
		roundSmall->setScale(0);
		this->addChild(roundSmall,2);
		
		cardLight = CCSprite::create("cardLightCoverBlue.png");
		cardLight->setScale(0.75);
		cardLight->setPosition(ccp(240,500));
		
		this->addChild(cardLight,4);
		
		return true;
	}
	std::function<void(void)> closeFunc;
	
	void setCloseFunc(std::function<void(void)> func){
		closeFunc = func;
	}
	
	
	void startSuccess(string option){
		isOpening=true;
		isSuccess=true;
		optionLbl->setString(option.c_str());
		gray->runAction(CCSequence::create(
										   CCFadeIn::create(0.5f),
										   CCCallFunc::create(this, callfunc_selector(StrengthCardAnimation::step2)),
										   CCDelayTime::create(0.2f)
										   ,NULL
										   )
						);
		
		
		
		
		
		
	}

	void startFail(string option){
		isOpening=true;
		isSuccess=false;
		optionLbl->setString(option.c_str());
		gray->runAction(CCSequence::create(
										   CCFadeIn::create(0.5f),
										   CCCallFunc::create(this, callfunc_selector(StrengthCardAnimation::step2)),
										   CCDelayTime::create(0.2f)
										   ,NULL
										   )
						);
		
		
		
		
		
		
	}
	
	void step2(){
		
		roundBig->runAction(
							CCSequence::create(
											   CCScaleTo::create(8/60.f, 1, 0.2f),
											   CCDelayTime::create(0.2f),
											   CCCallFunc::create(this, callfunc_selector(StrengthCardAnimation::step3))
											   ,NULL
											   )
							);
		
		roundSmall->runAction(CCScaleTo::create(8/60.f, 1, 0.2f));
		
		
	}
	
	void step3(){
		
		cardLight->runAction(
							 CCSequence::create(
												CCMoveTo::create(7/60.f,ccp(240,170)),
												CCMoveTo::create(7/60.f,ccp(240,175)),
												CCCallFunc::create(this, callfunc_selector(StrengthCardAnimation::step4)),
												NULL
												)
							 );
		
		
	}
	
	void step4(){
		title->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,1)));
		cardLight->runAction(
							 CCSequence::create(
												CCFadeOut::create(16/60.f),
												CCDelayTime::create(10/30.f),
												CCCallFunc::create(this, callfunc_selector(StrengthCardAnimation::step5)),
												NULL
												)
							 );
		card->setVisible(true);
//		card->runAction(
//						CCRepeatForever::create(
//												CCSequence::create(
//																   CCMoveBy::create(0.5f,ccp(0,5)),
//																   CCMoveBy::create(0.5f,ccp(0,-5))
//																   ,NULL)
//												)
//						);
		
	}
	
	//재료카드액션
	void step5(){

		
		card2->runAction(
						 CCSequence::create(
											CCSpawn::create(
															CCMoveTo::create(10/30.f, ccp(240,120)),
															CCScaleTo::create(10/30.f, kScale*0.7),
															NULL
															),
											CCSpawn::create(
															CCMoveTo::create(0, ccp(240,-410)),
															CCScaleTo::create(0, kScale*2.f),
															NULL
															),
											CCSpawn::create(
															CCMoveTo::create(10/30.f, ccp(240,120)),
															CCScaleTo::create(10/30.f, kScale*0.7),
															NULL
															),
											CCSpawn::create(
															CCMoveTo::create(0, ccp(240,-410)),
															CCScaleTo::create(0, kScale*2.f),
															NULL
															),
											CCSpawn::create(
															CCMoveTo::create(10/30.f, ccp(240,120)),
															CCScaleTo::create(10/30.f, kScale*0.7),
															NULL
															),
											CCSpawn::create(
															CCMoveTo::create(0, ccp(240,-410)),
															CCScaleTo::create(0, kScale*2.f),
															NULL
															),
											CCDelayTime::create(0.5f),
											CCCallFunc::create(this,callfunc_selector(StrengthCardAnimation::step55)),
						 NULL
						 )
		);
		cardLight->runAction(
							 CCSpawn::create(
											CCFadeIn::create(24/30.f),
											 CCSequence::create(
																CCDelayTime::create(10/30.f),
																CCScaleTo::create(2/30.f,0.8),
																CCScaleTo::create(0,0.75),
																CCDelayTime::create(8/30.f),
																CCScaleTo::create(2/30.f,0.8),
																CCScaleTo::create(0,0.75),
																CCDelayTime::create(8/30.f),
																CCScaleTo::create(2/30.f,0.8),
																CCScaleTo::create(0,0.75),
																NULL
											 ),
											 NULL
											)
							 );
	}
	
	void step55(){
		card->setVisible(false);
		cardLight->setPositionY(175);
		
		cardLight->runAction(
							 CCSequence::create(
												CCOrbitCamera::create(0.1f, 0.15f, 0, 0, 90, 0, 0),
												CCOrbitCamera::create(0.1f, 0.15f, 0, -90, 90, 0, 0),
												CCOrbitCamera::create(0.1f, 0.15f, 0, 0, 90, 0, 0),
												CCOrbitCamera::create(0.1f, 0.15f, 0, -90, 90, 0, 0),
												CCOrbitCamera::create(0.1f, 0.15f, 0, 0, 90, 0, 0),
												CCOrbitCamera::create(0.1f, 0.15f, 0, -90, 90, 0, 0),
												CCCallFunc::create(this,callfunc_selector(StrengthCardAnimation::step6)),
												NULL
												)
							 );
	}
	//카드라이트페이드아웃
	void step6(){
		card->setVisible(true);
		cardLight->runAction(
							 CCSequence::create(
												CCFadeOut::create(24/30.f),
												CCCallFunc::create(this,callfunc_selector(StrengthCardAnimation::step7)),
												NULL)
							 );
							
		optionLbl->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,1)));
		title->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,0)));
		
		if(isSuccess)
			titleSuccess->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3f,1)));
		else
			titleFail->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3f,1)));
	}
	
	void step7(){
		
		card->runAction(
								CCRepeatForever::create(
														CCSequence::create(
																		   CCMoveBy::create(0.5f,ccp(0,5)),
																		   CCMoveBy::create(0.5f,ccp(0,-5))
																		   ,NULL)
														)
								);
		isOpening=false;
	}
	
	void close(){
		isOpening=true;
		
		titleFail->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,0)));
		titleSuccess->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,0)));
		optionLbl->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,0)));
		
		card->runAction(CCMoveBy::create(0.2f,ccp(0,300)));
		roundSmall->runAction(CCScaleTo::create(0.3f,0));
		roundBig->runAction(
							CCSequence::create(
											   CCScaleTo::create(0.4f,0),
											   CCCallFunc::create(this, callfunc_selector(StrengthCardAnimation::closeStep2)),
											   NULL
											   )
							);
		
		
	}
	
	void closeStep2(){
		gray->runAction(
						CCSequence::create(
										   CCFadeOut::create(0.3f),
										   CCCallFunc::create(this, callfunc_selector(StrengthCardAnimation::closeStep3)),
										   NULL
										   )
						);
	}
	
	void closeStep3(){
		isOpening=false;
		this->removeFromParent();
		if(closeFunc)closeFunc();
	}
	
	void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, touch_priority, true);
		
	}
	
	
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
		if(isOpening==false)this->close();
		return true;
	}
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// 위너(사진,등수,점수)

class ChangeRankAnimation : public CCLayer{
	CCSprite* gray;

	CCSprite* title;
	int touch_priority;
	bool isOpening;
	float kScale;
	
	CCLayer* backRolling;
	CCSprite* backLayer;
	
	CCSprite* winnerBack;
	CCSprite* loserBack;
	
	CCLabelTTF* winnerLbl;
	CCLabelTTF* loserLbl;
	
	CCLabelTTF* winnerPointLbl;
	CCLabelTTF* loserPointLbl;
	
	CommonButton* closeBtn;
	CommonButton* sendBtn;
	
	CCLabelTTF* msgLbl;
	
	CCSprite* winnerArrow;
	CCSprite* loserArrow;
	
public:
	//	CREATE_FUNC(TakeCardAnimation);
	//CC_SYNTHESIZE(CCNode*, m_contentNode, ContentNode);
	
	static ChangeRankAnimation* create(string winner_picture_url,int winner_point, int winner_rank, string winner_name,string loser_picture_url,int loser_point, int loser_rank, string loser_name, int _touch_priority){
		ChangeRankAnimation* ret = new ChangeRankAnimation();
		ret->init(winner_picture_url,winner_point,winner_rank,winner_name,loser_picture_url,loser_point,loser_rank,loser_name,_touch_priority);
		ret->autorelease();
		return ret;
	}
	
	bool init(string winner_picture_url,int winner_point, int winner_rank, string winner_name,string loser_picture_url,int loser_point, int loser_rank, string loser_name, int _touch_priority){
		if(CCLayer::init()==false)return false;
		
		CCSprite* winner_picture = GDWebSprite::create(winner_picture_url, "ending_noimg.png");
		
		CCSprite* loser_picture = GDWebSprite::create(loser_picture_url, "ending_noimg.png");
		
		isOpening=true;
		touch_priority=_touch_priority;
		setTouchEnabled(true);
		kScale = 56/winner_picture->getContentSize().width;
		
	
		
		auto back = KS::loadCCBI<CCLayer*>(this, "gameresult_rankchange_back.ccbi");

		backRolling = back.first;

		//backRolling->setPosition(ccp(240,160));
		
		backRolling->setScale(1);
		
		
		this->addChild(backRolling,2);
		
		
		backLayer = CCSprite::create("rankchange_back.png");
		backLayer->setScale(2);
		backLayer->setAnchorPoint(ccp(0.5,0.5));
		backLayer->setPosition(ccp(240,160));
		this->addChild(backLayer,3);
		
		backLayer->setScale(0);
		
		/// winner ///////////////////////////////////////////////////////////////////////
		
		winnerBack = CCSprite::create("puzzle_right_ranklist_me.png");
		winnerBack->setPosition(ccp(220/2, 42/2)); //CGMakePoint(209, 42);
		winnerBack->setScale(0.5f);
		backLayer->addChild(winnerBack,4);
		
		winner_picture->setAnchorPoint(ccp(0.5f,0.5f));
		winner_picture->setPosition(ccp(21,21));
		winner_picture->setScale(0.8f);
		winnerBack->addChild(winner_picture,1);
		
		winnerLbl = CCLabelTTF::create(winner_name.c_str(), mySGD->getFont().c_str(), 12);
		winnerLbl->setPosition(ccp(80,27));
		winnerBack->addChild(winnerLbl,4);
		
		
		winnerPointLbl = CCLabelTTF::create(CCString::createWithFormat("%d",winner_point)->getCString(), mySGD->getFont().c_str(), 15);
		winnerPointLbl->setPosition(ccp(80,12));
		winnerBack->addChild(winnerPointLbl,4);
		
		
		/// loser ///////////////////////////////////////////////////////////////////////
		
		if(loser_rank==1){
			loserBack = CCSprite::create("puzzle_right_ranklist_gold.png");
			
		}else if(loser_rank==2){
			loserBack = CCSprite::create("puzzle_right_ranklist_silver.png");
			
		}else if(loser_rank==3){
			loserBack = CCSprite::create("puzzle_right_ranklist_bronze.png");
			
		}else{
			loserBack = CCSprite::create("puzzle_right_ranklist_normal.png");
		}
		loserBack->setScale(0.5f);
		loserBack->setPosition(ccp(160/2, 110/2)); //CGMakePoint(158, 114);
		
		
		loser_picture->setPosition(ccp(21,21));
		loser_picture->setAnchorPoint(ccp(0.5f,0.5f));
		loser_picture->setScale(0.8f);
		loserBack->addChild(loser_picture,1);
		
		
		loserLbl = CCLabelTTF::create(loser_name.c_str(), mySGD->getFont().c_str(), 12);
		loserLbl->setPosition(ccp(80,27));
		loserBack->addChild(loserLbl,4);
		
		backLayer->addChild(loserBack,4);
		
		
		loserPointLbl = CCLabelTTF::create(CCString::createWithFormat("%d",loser_point)->getCString(), mySGD->getFont().c_str(), 15);
		loserPointLbl->setPosition(ccp(80,12));
		loserBack->addChild(loserPointLbl,4);
		
		
		
		
		winnerArrow = CCSprite::create("rankchange_arrow_up.png");
		winnerArrow->setPosition(ccp(loserBack->getPositionX()+50,loserBack->getPositionY()));
		winnerArrow->setScale(0.5f);
		winnerArrow->runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f,ccp(0,-10)),CCMoveBy::create(0.5f,ccp(0,10)),NULL)));
		backLayer->addChild(winnerArrow);
		
		
		loserArrow = CCSprite::create("rankchange_arrow_down.png");
		loserArrow->setPosition(ccp(winnerBack->getPositionX()-50,winnerBack->getPositionY()));
		loserArrow->setScale(0.5f);
		loserArrow->runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f,ccp(0,10)),CCMoveBy::create(0.5f,ccp(0,-10)),NULL)));
		backLayer->addChild(loserArrow);
		
		
		//cardchange_cancel
		
		closeBtn = CommonButton::createCloseButton(touch_priority-1); //CCControlButton::create("닫기",mySGD->getFont().c_str(),20);
		closeBtn->setFunction([=](CCObject *){this->closeByBtn();});
		//closeBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(ThisClassType::closeByBtn), CCControlEventTouchUpInside);
		closeBtn->setPosition(ccp(770/2,560/2));
		closeBtn->setTouchPriority(touch_priority-1);
		closeBtn->setScale(0);
		addChild(closeBtn,9);
										   
										   
		sendBtn = CommonButton::create("도전하기", 15, CCSizeMake(90,45), CommonButtonBlue, touch_priority-1); //CCControlButton::create("도전하기",mySGD->getFont().c_str(),20);
		
		sendBtn->setFunction([=](CCObject*){this->sendMsgByBtn();});
		//sendBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(ThisClassType::sendMsgByBtn), CCControlEventTouchUpInside);
		sendBtn->setPosition(ccp(240,35));
		sendBtn->setTouchPriority(touch_priority-1);
		sendBtn->setScale(0);
		sendBtn->setTitleColor(ccc3(0,0,0));
		addChild(sendBtn,9);
		
		msgLbl = CCLabelTTF::create("친구에게 도전메세지를 전송합니다.", mySGD->getFont().c_str(), 10);
		msgLbl->setPosition(ccp(240,70));
		msgLbl->setScale(0);
		addChild(msgLbl,4);
		
//		kScale = 141/_card->getContentSize().width;
//		
//		
//		card=_card;
//		card->setPosition(ccp(240,155));
//		card->setVisible(false);
//		card->setScale(kScale*0.7);
//		
//		this->addChild(card,3);
//
		title = CCSprite::create("ani_title_changerank.png");
		title->setPosition(ccp(240,276));
		title->setScale(0);
		this->addChild(title,2);
//

		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;

		gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(240,160));
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		addChild(gray, 1);
		
		
//
//		auto ret = KS::loadCCBI<CCLayer*>(this, "roundBlue1.ccbi");
//		roundBig = ret.first;
//		roundBig->setPosition(ccp(0,-95));
//		roundBig->setScale(0);
//		this->addChild(roundBig,2);
//		
//		auto ret2 = KS::loadCCBI<CCLayer*>(this, "roundBlue2.ccbi");
//		roundSmall = ret2.first;
//		roundSmall->setPosition(ccp(0,-95));
//		roundSmall->setScale(0);
//		this->addChild(roundSmall,2);
//		
//		cardLight = CCSprite::create("cardLightCoverBlue.png");
//		cardLight->setScale(0.7);
//		cardLight->setPosition(ccp(240,500));
//		
//		this->addChild(cardLight,4);
		
		return true;
	}

	std::function<void(void)> closeFunc;
	
	void setCloseFunc(std::function<void(void)> func){
		closeFunc = func;
	}
	
	std::function<void(void)> cancelFunc;
	
	void setCancelFunc(std::function<void(void)> func){
		cancelFunc = func;
	}
	
	
	std::function<void(void)> sendFunc;
	
	void setSendFunc(std::function<void(void)> func){
		sendFunc = func;
	}
	
	
	void closeByBtn(){
		this->close();
		if(cancelFunc)cancelFunc();
	}
	void sendMsgByBtn(){
		this->close();
		if(sendFunc)sendFunc();
	}
	void start(){
		isOpening=true;
		gray->runAction(CCSequence::create(
										   CCFadeIn::create(0.5f),
										   CCCallFunc::create(this, callfunc_selector(ThisClassType::step2)),
										   NULL
										   )
						);
		
		
		
		
	}
	void step2(){
		
		backRolling->runAction(CCScaleTo::create(0.3f,2));
		backLayer->runAction(CCScaleTo::create(0.3f,2));
			
		title->runAction(
							CCSequence::create(
											   CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)),
											   CCDelayTime::create(1.f),
											   CCCallFunc::create(this, callfunc_selector(ThisClassType::step3))
											   ,NULL
											   )
							);
//
//		roundSmall->runAction(CCScaleTo::create(8/60.f, 1, 0.2f));
		
		
	}
	
	void step3(){
		
		winnerArrow->runAction(CCMoveBy::create(0.3,ccp(0,200)));
		loserArrow->runAction(CCMoveBy::create(0.3,ccp(0,-200)));
		
		CCPoint wPo = winnerBack->getPosition();
		CCPoint lPo = loserBack->getPosition();
		
		loserBack->runAction(CCMoveTo::create(0.3f, wPo));
		
		winnerBack->runAction(
							  CCSequence::create(
												 CCMoveTo::create(0.3f, lPo),
												 CCCallFunc::create(this, callfunc_selector(ThisClassType::step4))
												 ,NULL
												 )
		);
		
		
		
	}
	
	void step4(){
		
		winnerArrow->setVisible(false);
		loserArrow->setVisible(false);
		
		closeBtn->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)));
		msgLbl->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)));
		sendBtn->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)));
//		
//		title->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,1)));
//		
//		cardLight->runAction(
//							 CCFadeOut::create(16/30.f)
//							 );
//		card->setVisible(true);
//		card->runAction(
//						CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f,ccp(0,5)),CCMoveBy::create(0.5f,ccp(0,-5)),NULL))
//						);
		
		isOpening=false;
	}
	
	void close(){
		isOpening=true;
		
		backRolling->runAction(CCScaleTo::create(0.3f,0));
		backLayer->runAction(CCScaleTo::create(0.3f,0));
		msgLbl->runAction(CCScaleTo::create(0.3f,0));
		closeBtn->runAction(CCScaleTo::create(0.3f,0));
		sendBtn->runAction(CCScaleTo::create(0.3f,0));
		title->runAction(CCScaleTo::create(0.3f,0));
		
		this->runAction(
							CCSequence::create(
											   CCDelayTime::create(0.4f),
											   CCCallFunc::create(this, callfunc_selector(ThisClassType::closeStep2)),
											   NULL
											   )
							);
		
//		
//		title->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,0.01f)));
//		
//		card->runAction(CCMoveBy::create(0.2f,ccp(0,300)));
//		roundSmall->runAction(CCScaleTo::create(0.3f,0.01f));
//		
//		roundBig->runAction(
//							CCSequence::create(
//											   CCScaleTo::create(0.4f,0.01f),
//											   CCCallFunc::create(this, callfunc_selector(ThisClassType::closeStep2)),
//											   NULL
//											   )
//							);
//
		
		
	}
	
	void closeStep2(){
		gray->runAction(
						CCSequence::create(
										   CCFadeOut::create(0.3f),
										   CCCallFunc::create(this, callfunc_selector(ThisClassType::closeStep3)),
										   NULL
										   )
						);
	}
	
	void closeStep3(){
		isOpening=false;
		this->removeFromParent();
		if(closeFunc)closeFunc();
	}
	
	void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, touch_priority, true);
		
	}
	
	
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
		//if(isOpening==false)this->close();
		return true;
	}
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// 위너(사진,등수,점수)

class FightResultAnimation : public CCLayer{
	CCSprite* gray;
	
	CCSprite* title;
	int touch_priority;
	bool isOpening;
	float kScale;
	
	CCLayer* backRolling;
	CCSprite* backLayer;
	
	CCSprite* myBack;
	CCSprite* otherBack;
	
	CCLabelTTF* myLbl;
	CCLabelTTF* otherLbl;
	
	CCLabelTTF* myPointLbl;
	CCLabelTTF* otherPointLbl;
	
	CommonButton* closeBtn;
	CommonButton* sendBtn;
	CommonButton* confirmBtn;
	
	CCLabelTTF* msgLbl;
	
	CCSprite* myArrow;
	CCSprite* otherArrow;
	
	CCSprite* titleWin;
	CCSprite* titleLose;
	
	int myPoint;
	int otherPoint;
	
	int pointCnt;
public:
	//	CREATE_FUNC(TakeCardAnimation);
	//CC_SYNTHESIZE(CCNode*, m_contentNode, ContentNode);
	
	static FightResultAnimation* create(string my_picture_url,int my_point, string my_name,string other_picture_url,int other_point, string other_name, int _touch_priority){
		FightResultAnimation* ret = new FightResultAnimation();
		ret->init(my_picture_url,my_point,my_name,other_picture_url,other_point,other_name,_touch_priority);
		ret->autorelease();
		return ret;
	}
	
	bool init(string my_picture_url,int my_point, string my_name,string other_picture_url,int other_point, string other_name, int _touch_priority){
		if(CCLayer::init()==false)return false;
		
		CCSprite* my_picture = GDWebSprite::create(my_picture_url, "ending_noimg.png");
		CCSprite* other_picture = GDWebSprite::create(other_picture_url, "ending_noimg.png");
		
		
		isOpening=true;
		touch_priority=_touch_priority;
		setTouchEnabled(true);
		kScale = 56/my_picture->getContentSize().width;
		
		myPoint = my_point;
		otherPoint = other_point;
		pointCnt = 0;
		
		auto back = KS::loadCCBI<CCLayer*>(this, "gameresult_rankchange_back.ccbi");
		
		backRolling = back.first;
		
		//backRolling->setPosition(ccp(240,160));
		
		backRolling->setScale(1);
		
		
		this->addChild(backRolling,2);
		
		
		backLayer = CCSprite::create("rankchange_back.png");
		backLayer->setScale(2);
		backLayer->setAnchorPoint(ccp(0.5,0.5));
		backLayer->setPosition(ccp(240,160));
		this->addChild(backLayer,3);
		
		backLayer->setScale(0);
		
		/// my ///////////////////////////////////////////////////////////////////////
		
		myBack = CCSprite::create("puzzle_right_ranklist_me.png");
		myBack->setPosition(ccp(220/2, 42/2)); //CGMakePoint(209, 42);
		myBack->setScale(0.5f);
		backLayer->addChild(myBack,4);
		
		my_picture->setAnchorPoint(ccp(0.5f,0.5f));
		my_picture->setPosition(ccp(21,21));
		my_picture->setScale(0.8f);
		myBack->addChild(my_picture,1);
		
		myLbl = CCLabelTTF::create(my_name.c_str(), mySGD->getFont().c_str(), 12);
		myLbl->setPosition(ccp(80,27));
		myBack->addChild(myLbl,4);
		
		
		myPointLbl = CCLabelTTF::create("0", mySGD->getFont().c_str(), 15);
		myPointLbl->setPosition(ccp(80,12));
		myBack->addChild(myPointLbl,4);
		
		
		/// other ///////////////////////////////////////////////////////////////////////
		
		
		otherBack = CCSprite::create("puzzle_right_ranklist_normal.png");
		
		otherBack->setScale(0.5f);
		otherBack->setPosition(ccp(160/2, 110/2)); //CGMakePoint(158, 114);
		
		
		other_picture->setPosition(ccp(21,21));
		other_picture->setAnchorPoint(ccp(0.5f,0.5f));
		other_picture->setScale(0.8f);
		otherBack->addChild(other_picture,1);
		
		
		otherLbl = CCLabelTTF::create(other_name.c_str(), mySGD->getFont().c_str(), 12);
		otherLbl->setPosition(ccp(80,27));
		otherBack->addChild(otherLbl,4);
		
		backLayer->addChild(otherBack,4);
		
		
		otherPointLbl = CCLabelTTF::create("0", mySGD->getFont().c_str(), 15);
		otherPointLbl->setPosition(ccp(80,12));
		otherBack->addChild(otherPointLbl,4);
		
		
		
		
		myArrow = CCSprite::create("rankchange_arrow_up.png");
		myArrow->setPosition(ccp(otherBack->getPositionX()+50,otherBack->getPositionY()));
		myArrow->setScale(0.5f);
		myArrow->runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f,ccp(0,-10)),CCMoveBy::create(0.5f,ccp(0,10)),NULL)));
		backLayer->addChild(myArrow);
		
		
		otherArrow = CCSprite::create("rankchange_arrow_down.png");
		otherArrow->setPosition(ccp(myBack->getPositionX()-50,myBack->getPositionY()));
		otherArrow->setScale(0.5f);
		otherArrow->runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f,ccp(0,10)),CCMoveBy::create(0.5f,ccp(0,-10)),NULL)));
		backLayer->addChild(otherArrow);
		
		
		//cardchange_cancel
		
		//closeBtn = CCControlButton::create("닫기",mySGD->getFont().c_str(),20);
		//closeBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(ThisClassType::closeByBtn), CCControlEventTouchUpInside);
		
		closeBtn = CommonButton::createCloseButton(touch_priority-1);
		closeBtn->setFunction([=](CCObject *){this->closeByBtn();});
		closeBtn->setPosition(ccp(770/2,560/2));
		closeBtn->setScale(0);
		addChild(closeBtn,9);
		
		
		//sendBtn = CCControlButton::create("확인",mySGD->getFont().c_str(),20);
		//sendBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(ThisClassType::sendMsgByBtn), CCControlEventTouchUpInside);
		sendBtn = CommonButton::create("확인", 15, CCSizeMake(90,45), CommonButtonBlue, touch_priority-1);
		sendBtn->setFunction([=](CCObject *){this->sendMsgByBtn();});
		sendBtn->setPosition(ccp(240,35));
		sendBtn->setTitleColor(ccc3(0,0,0));
		sendBtn->setTouchPriority(touch_priority-1);
		sendBtn->setScale(0);
		addChild(sendBtn,9);
		
//		confirmBtn = CCControlButton::create("확인",mySGD->getFont().c_str(),20);
//		confirmBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(ThisClassType::ConfirmByBtn), CCControlEventTouchUpInside);
		confirmBtn = CommonButton::create("확인", 15, CCSizeMake(90,45), CommonButtonBlue, touch_priority-1);
		confirmBtn->setFunction([=](CCObject *){this->ConfirmByBtn();});
		confirmBtn->setPosition(ccp(240,35));
		confirmBtn->setTitleColor(ccc3(0,0,0));
		confirmBtn->setScale(0);
		addChild(confirmBtn,9);
		
		msgLbl = CCLabelTTF::create("친구에게 도전메세지를 전송합니다.", mySGD->getFont().c_str(), 10);
		msgLbl->setPosition(ccp(240,70));
		msgLbl->setScale(0);
		addChild(msgLbl,4);
		
		//		kScale = 141/_card->getContentSize().width;
		//
		//
		//		card=_card;
		//		card->setPosition(ccp(240,155));
		//		card->setVisible(false);
		//		card->setScale(kScale*0.7);
		//
		//		this->addChild(card,3);
		//
		title = CCSprite::create("ani_title_fight.png");
		title->setPosition(ccp(240,276));
		title->setScale(0);
		this->addChild(title,2);
		
		titleWin = CCSprite::create("ani_title_win.png");
		titleWin->setPosition(ccp(240,276));
		titleWin->setScale(0);
		this->addChild(titleWin,2);
		
		
		titleLose = CCSprite::create("ani_title_lose.png");
		titleLose->setPosition(ccp(240,276));
		titleLose->setScale(0);
		this->addChild(titleLose,2);
		//
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(240,160));
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		addChild(gray, 1);
		
		
		//
		//		auto ret = KS::loadCCBI<CCLayer*>(this, "roundBlue1.ccbi");
		//		roundBig = ret.first;
		//		roundBig->setPosition(ccp(0,-95));
		//		roundBig->setScale(0);
		//		this->addChild(roundBig,2);
		//
		//		auto ret2 = KS::loadCCBI<CCLayer*>(this, "roundBlue2.ccbi");
		//		roundSmall = ret2.first;
		//		roundSmall->setPosition(ccp(0,-95));
		//		roundSmall->setScale(0);
		//		this->addChild(roundSmall,2);
		//
		//		cardLight = CCSprite::create("cardLightCoverBlue.png");
		//		cardLight->setScale(0.7);
		//		cardLight->setPosition(ccp(240,500));
		//
		//		this->addChild(cardLight,4);
		
		return true;
	}
	
	std::function<void(void)> closeFunc;
	
	void setCloseFunc(std::function<void(void)> func){
		closeFunc = func;
	}
	
	std::function<void(void)> cancelFunc;
	
	void setCancelFunc(std::function<void(void)> func){
		cancelFunc = func;
	}
	
	
	std::function<void(void)> sendFunc;
	
	void setSendFunc(std::function<void(void)> func){
		sendFunc = func;
	}
	
	
	std::function<void(void)> confirmFunc;
	
	void setConfirmFunc(std::function<void(void)> func){
		confirmFunc = func;
	}
	
	
	void closeByBtn(){
		this->close();
		if(cancelFunc)cancelFunc();
	}
	void sendMsgByBtn(){
		this->close();
		if(sendFunc)sendFunc();
	}
	
	void ConfirmByBtn(){
		this->close();
		if(confirmFunc)confirmFunc();
	}
	
	void startLose(){
		isOpening=true;
		gray->runAction(CCSequence::create(
																			 CCFadeIn::create(0.5f),
																			 CCCallFunc::create(this, callfunc_selector(ThisClassType::loseStep1)),
																			 NULL
																			 )
										);
		
	}
	
	void loseStep1(){
		backRolling->runAction(CCScaleTo::create(0.3f,2));
		backLayer->runAction(CCScaleTo::create(0.3f,2));
		
		title->runAction(
										 CCSequence::create(
																				CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)),
																				CCDelayTime::create(1.f),
																				CCCallFunc::create(this, callfunc_selector(ThisClassType::loseStep2))
																				,NULL
																				)
										 );
	}
	
	void loseStep2(){
		
		
		myPointLbl->addChild(KSGradualValue<float>::create(0, (float)myPoint, 1.5f,
																											 [=](float t)
																											 {
																												 myPointLbl->setString(CCString::createWithFormat("%d",(int)t)->getCString());
																												 
																												 otherPointLbl->setString(CCString::createWithFormat("%d",(int)t)->getCString());
																												 
																											 },
																											 [=](float t)
																											 {
																												 
																												 this->addChild(KSTimer::create(1.f, [=](){
																													 titleLose->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)));
																													 
																													 title->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,0)));
																													 this->loseStep3();
																													 otherPointLbl->addChild(KSGradualValue<float>::create((float)myPoint, (float)otherPoint, 1.0f,
																																																							[=](float t)
																																																							{
																																																								otherPointLbl->setString(CCString::createWithFormat("%d",(int)t)->getCString());
																																																							},[=](float t){
																																																								
																																																								
																																																							}));
																												 }));
																											 }));
	}
	
	void loseStep3(){
		
		myArrow->runAction(CCMoveBy::create(0.3,ccp(0,-200)));
		otherArrow->runAction(
													CCSequence::create(
																						 CCMoveBy::create(0.3,ccp(0,200)),
																						 CCCallFunc::create(this, callfunc_selector(ThisClassType::loseStep4)),
																						 NULL)
													);
		
		//		CCPoint wPo = myBack->getPosition();
		//		CCPoint lPo = otherBack->getPosition();
		//
		//		otherBack->runAction(CCMoveTo::create(0.3f, wPo));
		//
		//		myBack->runAction(
		//							  CCSequence::create(
		//												 CCMoveTo::create(0.3f, lPo),
		//												 CCCallFunc::create(this, callfunc_selector(ThisClassType::loseStep4))
		//												 ,NULL
		//												 )
		//							  );
		
		
		
	}
	
	void loseStep4(){
		
		myArrow->setVisible(false);
		otherArrow->setVisible(false);
		
		closeBtn->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)));
		//msgLbl->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)));
		confirmBtn->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)));
		
		
		isOpening=false;
	}
	
	void startWin(){
		isOpening=true;
		gray->runAction(CCSequence::create(
																			 CCFadeIn::create(0.5f),
																			 CCCallFunc::create(this, callfunc_selector(ThisClassType::winStep1)),
																			 NULL
																			 )
										);
		
	}
	
	void winStep1(){
		backRolling->runAction(CCScaleTo::create(0.3f,2));
		backLayer->runAction(CCScaleTo::create(0.3f,2));
		
		title->runAction(
										 CCSequence::create(
																				CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)),
																				CCDelayTime::create(1.f),
																				CCCallFunc::create(this, callfunc_selector(ThisClassType::winStep2))
																				,NULL
																				)
										 );
	}
	
	void winStep2(){
		
		
		myPointLbl->addChild(KSGradualValue<float>::create(0, (float)otherPoint, 1.5f,
																											 [=](float t)
																											 {
																												 myPointLbl->setString(CCString::createWithFormat("%d",(int)t)->getCString());
																												 
																												 otherPointLbl->setString(CCString::createWithFormat("%d",(int)t)->getCString());
																												 
																											 },
																											 [=](float t)
																											 {
																												 
																												 this->addChild(KSTimer::create(1.f, [=](){
																													 titleWin->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)));
																													 
																													 title->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,0)));
																													 this->winStep3();
																													 myPointLbl->addChild(KSGradualValue<float>::create((float)otherPoint, (float)myPoint, 1.0f,
																																																							[=](float t)
																																																							{
																																																								myPointLbl->setString(CCString::createWithFormat("%d",(int)t)->getCString());
																																																							},[=](float t){
																																																								
																																																								
																																																							}));
																												 }));
																											 }));
	}
	
	void winStep3(){
		
		myArrow->runAction(CCMoveBy::create(0.3,ccp(0,200)));
		otherArrow->runAction(CCMoveBy::create(0.3,ccp(0,-200)));
		
		
		CCPoint wPo = myBack->getPosition();
		CCPoint lPo = otherBack->getPosition();
		
		otherBack->runAction(CCMoveTo::create(0.3f, wPo));
		
		myBack->runAction(
											CCSequence::create(
																				 CCMoveTo::create(0.3f, lPo),
																				 CCCallFunc::create(this, callfunc_selector(ThisClassType::winStep4))
																				 ,NULL
																				 )
											);
		
		
		
	}
	
	void winStep4(){
		
		myArrow->setVisible(false);
		otherArrow->setVisible(false);
		
		closeBtn->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)));
		msgLbl->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)));
		sendBtn->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)));
		
		
		isOpening=false;
	}
	
	void start(){
		isOpening=true;
		gray->runAction(CCSequence::create(
																			 CCFadeIn::create(0.5f),
																			 CCCallFunc::create(this, callfunc_selector(ThisClassType::step2)),
																			 NULL
																			 )
										);
		
		
		
		
	}
	void step2(){
		
		backRolling->runAction(CCScaleTo::create(0.3f,2));
		backLayer->runAction(CCScaleTo::create(0.3f,2));
		
		title->runAction(
										 CCSequence::create(
																				CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)),
																				CCDelayTime::create(1.f),
																				CCCallFunc::create(this, callfunc_selector(ThisClassType::step3))
																				,NULL
																				)
										 );
		//
		//		roundSmall->runAction(CCScaleTo::create(8/60.f, 1, 0.2f));
		
		
	}
	
	void step3(){
		
		myArrow->runAction(CCMoveBy::create(0.3,ccp(0,200)));
		otherArrow->runAction(CCMoveBy::create(0.3,ccp(0,-200)));
		
		CCPoint wPo = myBack->getPosition();
		CCPoint lPo = otherBack->getPosition();
		
		otherBack->runAction(CCMoveTo::create(0.3f, wPo));
		
		myBack->runAction(
											CCSequence::create(
																				 CCMoveTo::create(0.3f, lPo),
																				 CCCallFunc::create(this, callfunc_selector(ThisClassType::step4))
																				 ,NULL
																				 )
											);
		
		
		
	}
	
	void step4(){
		
		myArrow->setVisible(false);
		otherArrow->setVisible(false);
		
		closeBtn->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)));
		msgLbl->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)));
		sendBtn->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,1)));
		//
		//		title->runAction(CCEaseBounceOut::create(CCScaleTo::create(0.3,1)));
		//
		//		cardLight->runAction(
		//							 CCFadeOut::create(16/30.f)
		//							 );
		//		card->setVisible(true);
		//		card->runAction(
		//						CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f,ccp(0,5)),CCMoveBy::create(0.5f,ccp(0,-5)),NULL))
		//						);
		
		isOpening=false;
	}
	
	void close(){
		isOpening=true;
		
		backRolling->runAction(CCScaleTo::create(0.3f,0));
		backLayer->runAction(CCScaleTo::create(0.3f,0));
		msgLbl->runAction(CCScaleTo::create(0.3f,0));
		closeBtn->runAction(CCScaleTo::create(0.3f,0));
		confirmBtn->runAction(CCScaleTo::create(0.3f,0));
		sendBtn->runAction(CCScaleTo::create(0.3f,0));
		titleLose->runAction(CCScaleTo::create(0.3f,0));
		titleWin->runAction(CCScaleTo::create(0.3f,0));
		
		this->runAction(
										CCSequence::create(
																			 CCDelayTime::create(0.4f),
																			 CCCallFunc::create(this, callfunc_selector(ThisClassType::closeStep2)),
																			 NULL
																			 )
										);
		
		//
		//		title->runAction(CCEaseBounceIn::create(CCScaleTo::create(0.3f,0.01f)));
		//
		//		card->runAction(CCMoveBy::create(0.2f,ccp(0,300)));
		//		roundSmall->runAction(CCScaleTo::create(0.3f,0.01f));
		//
		//		roundBig->runAction(
		//							CCSequence::create(
		//											   CCScaleTo::create(0.4f,0.01f),
		//											   CCCallFunc::create(this, callfunc_selector(ThisClassType::closeStep2)),
		//											   NULL
		//											   )
		//							);
		//
		
		
	}
	
	void closeStep2(){
		gray->runAction(
										CCSequence::create(
																			 CCFadeOut::create(0.3f),
																			 CCCallFunc::create(this, callfunc_selector(ThisClassType::closeStep3)),
																			 NULL
																			 )
										);
	}
	
	void closeStep3(){
		isOpening=false;
		this->removeFromParent();
		if(closeFunc)closeFunc();
	}
	
	void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, touch_priority, true);
		
	}
	
	
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
		return true;
	}
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
		return;
	}
	
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif /* defined(__DGproto__TakeCardAnimation__) */
