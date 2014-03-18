// PlayUI.cpp
//

#include "PlayUI.h"
#include "KSCumberBase.h"
#include "StartSettingScene.h"
#include "AchieveNoti.h"
#include "RollingButton.h"
#include "KSLabelTTF.h"
#include "EnumDefine.h"
#define LZZ_INLINE inline
using namespace cocos2d;
using namespace std;
using namespace std::placeholders;
ComboView * ComboView::create (int combo)
{
	ComboView* t_cv = new ComboView();
	t_cv->myInit(combo);
	t_cv->autorelease();
	return t_cv;
}
void ComboView::changeCombo (int combo)
{
	combo_label->setString(CCString::createWithFormat("%d", combo)->getCString());
}
void ComboView::setPercentage (float t_percent)
{
//	combo_timer->setPercentage(t_percent*100);
}
void ComboView::myInit (int combo)
{
	initWithFile("combo_back.png");
	setPosition(ccp(480-30,myDSH->ui_top-100));//78));
	setOpacity(0);
	
//	combo_str = CCSprite::create("combo_front.png");
//	combo_str->setPosition(ccp(getContentSize().width/2.f-13, getContentSize().height/2.f));
//	addChild(combo_str);
	
//	combo_timer = CCProgressTimer::create(CCSprite::create("combo_front.png"));
//	combo_timer->setType(kCCProgressTimerTypeBar);
//	combo_timer->setMidpoint(ccp(0,0));
//	combo_timer->setBarChangeRate(ccp(1,0));
//	combo_timer->setPercentage(100);
//	combo_timer->setPosition(ccp(getContentSize().width/2.f-5, getContentSize().height/2.f));
//	addChild(combo_timer);
	
	KSLabelTTF* combo_ment = KSLabelTTF::create("콤보", mySGD->getFont().c_str(), 18);
	combo_ment->setColor(ccc3(50,215,0));
	combo_ment->setAnchorPoint(ccp(1,0.5));
	combo_ment->enableOuterStroke(ccBLACK, 1.f);
	combo_ment->setPosition(ccp(20,6));
	addChild(combo_ment);
	
	
	combo_label = KSLabelTTF::create(CCString::createWithFormat("%d", combo)->getCString(), mySGD->getFont().c_str(), 23);//CCLabelBMFont::create(CCString::createWithFormat("%d", combo)->getCString(), "combo.fnt");
	combo_label->setColor(ccc3(50, 215, 0));
	combo_label->setAnchorPoint(ccp(1,0.5));
	combo_label->enableOuterStroke(ccBLACK, 1.f);
	combo_label->setPosition(ccp(-17,6));
	addChild(combo_label);
}
ComboParent * ComboParent::create (CCNode* t_score_label)
{
	ComboParent* t_cp = new ComboParent();
	t_cp->myInit(t_score_label);
	t_cp->autorelease();
	return t_cp;
}
void ComboParent::showCombo (int t_combo)
{
	if(getChildrenCount() > 0)
	{
		ComboView* t_cv = (ComboView*)getChildren()->objectAtIndex(0);// randomObject();
		t_cv->changeCombo(t_combo);
	}
	else
	{
		ComboView* t_cv = ComboView::create(t_combo);
		t_cv->setScale(1.f/1.5f);
		t_cv->setPosition(ccpAdd(score_label->getPosition(), ccp(20,-22)));
		addChild(t_cv,0,1);// 1 : ComboView
	}
	
	keeping_frame = 500;
	if(!is_keeping)
		startKeep();
}
void ComboParent::stopKeep ()
{
	unschedule(schedule_selector(ComboParent::keeping));
	removeChildByTag(1);
	myGD->communication("UI_setComboCnt", 0);
	is_keeping = false;
//	score_label->setVisible(true);
}
void ComboParent::startKeep ()
{
//	score_label->setVisible(false);
	is_keeping = true;
	schedule(schedule_selector(ComboParent::keeping));
}
void ComboParent::keeping ()
{
	//		if(myGD->getJackState() == jackState::jackStateDrawing)
	//		{
	//			if(!is_holding)
	//			{
	//				is_holding = true;
	//				hold_frame = 1;
	//			}
	//			else
	//			{
	//				hold_frame++;
	//			}
	//		}
	//		else
	//		{
	//			if(is_holding)
	//			{
	//				is_holding = false;
	//				keeping_frame -= hold_frame;
	//			}
	
	keeping_frame--;
//	((ComboView*)getChildByTag(1))->setPercentage(keeping_frame/500.f);
	
	if(keeping_frame <= 0)
	{
		stopKeep();
	}
	//		}
}
void ComboParent::myInit (CCNode* t_score_label)
{
	is_keeping = false;
	score_label = t_score_label;
	//		is_holding = false;
}
FeverParent * FeverParent::create (CCNode* t_counting_label)
{
	FeverParent* t_fp = new FeverParent();
	t_fp->myInit(t_counting_label);
	t_fp->autorelease();
	return t_fp;
}
void FeverParent::addFeverGage (int count)
{
	if(ing_fever)	return;
	
	keeping_count = 300;
//	if(!is_keeping)
//		startKeep();
	
	recent_count += count;
	if(recent_count >= 15)
	{
		if(((CCLabelTTF*)counting_label)->getColor().g > 150)
			counting_label->setVisible(false);
		
		fever_back->setVisible(true);
		fever_top->setVisible(true);
		
		ing_fever = true;
		entered_fever_cnt++;
		int total_fever_cnt = myDSH->getIntegerForKey(kDSH_Key_achieve_totalFeverCnt)+1;
		myDSH->setIntegerForKey(kDSH_Key_achieve_totalFeverCnt, total_fever_cnt);
		
		AchieveConditionReward* shared_acr = AchieveConditionReward::sharedInstance();
		
		if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_feverMania1) == 0 &&
		   entered_fever_cnt >= shared_acr->getCondition(kAchievementCode_feverMania1))
		{
			myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_feverMania1, 1);
			AchieveNoti* t_noti = AchieveNoti::create(kAchievementCode_feverMania1);
			CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
		}
		
		for(int i=kAchievementCode_feverMania2;i<=kAchievementCode_feverMania3;i++)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) == 0 &&
			   total_fever_cnt == shared_acr->getCondition((AchievementCode)i))
			{
				myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, i, 1);
				AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
			}
		}
		
		
		recent_count = 15;
		
		fever_top->setPercentage(100.f);
		
		myLog->addLog(kLOG_show_fever, myGD->getCommunication("UI_getUseTime"));
		myGD->communication("GIM_startFever");
		
		myGD->setAlphaSpeed(myGD->getAlphaSpeed() + 1.5f);
		
		fever_particle = CCParticleSystemQuad::createWithTotalParticles(100);
		fever_particle->setPositionType(kCCPositionTypeFree);
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("fever_particle.png");
		fever_particle->setTexture(texture);
		fever_particle->setEmissionRate(100);
		fever_particle->setAngle(90.0);
		fever_particle->setAngleVar(40.0);
		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
		fever_particle->setBlendFunc(blendFunc);
		fever_particle->setDuration(-1.0);
		fever_particle->setEmitterMode(kCCParticleModeGravity);
		ccColor4F startColor = {1.0,0.8,0.4,1.0}; // 0.76 0.25 0.12
		fever_particle->setStartColor(startColor);
		ccColor4F startColorVar = {0,0,0,0.3};
		fever_particle->setStartColorVar(startColorVar);
		ccColor4F endColor = {0.0,0.0,0.0,1.00};
		fever_particle->setEndColor(endColor);
		ccColor4F endColorVar = {0,0,0,0.3};
		fever_particle->setEndColorVar(endColorVar);
		fever_particle->setStartSize(10.0);
		fever_particle->setStartSizeVar(10.0);
		fever_particle->setEndSize(5.0);
		fever_particle->setEndSizeVar(10.0);
		fever_particle->setGravity(ccp(0,-400));
		fever_particle->setRadialAccel(0.0);
		fever_particle->setRadialAccelVar(0.0);
		fever_particle->setSpeed(200);
		fever_particle->setSpeedVar(50.0);
		fever_particle->setTangentialAccel(0);
		fever_particle->setTangentialAccelVar(0);
		fever_particle->setTotalParticles(100);
		fever_particle->setLife(1.0);
		fever_particle->setLifeVar(0.0);
		fever_particle->setStartSpin(0.0);
		fever_particle->setStartSpinVar(360.f);
		fever_particle->setEndSpin(0.0);
		fever_particle->setEndSpinVar(360.f);
		fever_particle->setPosition(ccp(240,140));
		fever_particle->setPosVar(ccp(240,160));
		addChild(fever_particle);
		
		unschedule(schedule_selector(FeverParent::keeping));
		is_keeping = false;
		
		CCDelayTime* t_delay = CCDelayTime::create(NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_feverTime_i, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1));
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(FeverParent::endFever));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		runAction(t_seq);
		
		CCProgressFromTo* progress_to = CCProgressFromTo::create(NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_feverTime_i, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1), 100.f, 0.f);
		fever_top->runAction(progress_to);
	}
	else
	{
		CCProgressTo* progress_to = CCProgressTo::create(0.3f, recent_count/15.f*100.f);
		fever_top->runAction(progress_to);
	}
}
void FeverParent::endFever ()
{
	if(ing_fever)
	{
		counting_label->setVisible(true);
		fever_back->setVisible(false);
		fever_top->setVisible(false);
		
		ing_fever = false;
		recent_count = 0;
		
		myGD->communication("GIM_stopFever");
		
		myGD->setAlphaSpeed(myGD->getAlphaSpeed() - 1.5f);
		
		fever_particle->setDuration(0.f);
		fever_particle->setAutoRemoveOnFinish(true);
		
		fever_top->stopAllActions();
		fever_top->setPercentage(0.f);
		CCProgressTo* progress_to = CCProgressTo::create(0.3f, recent_count/20.f*100.f);
		fever_top->runAction(progress_to);
	}
}
void FeverParent::startKeep ()
{
	is_keeping = true;
	schedule(schedule_selector(FeverParent::keeping));
}
void FeverParent::keeping ()
{
	keeping_count--;
	
	if(keeping_count <= 0)
		stopKeep();
}
void FeverParent::stopKeep ()
{
	unschedule(schedule_selector(FeverParent::keeping));
	is_keeping = false;
	recent_count = 0;
	CCProgressTo* progress_to = CCProgressTo::create(0.3f, recent_count/10.f*100.f);
	fever_top->runAction(progress_to);
}
void FeverParent::myInit (CCNode* t_counting_label)
{
	counting_label = t_counting_label;
	ing_fever = false;
	keeping_count = 0;
	is_keeping = false;
	entered_fever_cnt = 0;
	
	fever_back = CCSprite::create("fever_gage_back.png");
	fever_back->setPosition(ccpAdd(counting_label->getPosition(), ccp(0,-20)));
	addChild(fever_back);
	fever_back->setVisible(false);
	
	fever_top = CCProgressTimer::create(CCSprite::create("fever_gage_top.png"));
	fever_top->getSprite()->setColor(ccc3(150, 0, 255));
	fever_top->setType(kCCProgressTimerTypeBar);
	fever_top->setMidpoint(ccp(0,0));
	fever_top->setBarChangeRate(ccp(1,0));
	fever_top->setPosition(fever_back->getPosition());
	fever_top->setPercentage(0.f);
	addChild(fever_top);
	fever_top->setVisible(false);
	
	recent_count = 0;
	
	fever_particle = NULL;
}
GoldLabel * GoldLabel::create ()
{
	GoldLabel* t_gl = new GoldLabel();
	t_gl->myInit();
	t_gl->autorelease();
	return t_gl;
}
void GoldLabel::setString (char const * after_gold)
{
	stopAllActions();
	if(is_incresing)
		stopIncreasing();
	startIncreasing();
}
void GoldLabel::startIncreasing ()
{
	is_incresing = true;
	CCScaleTo* t_scale1 = CCScaleTo::create(0.2f, 1.5f);
	CCScaleTo* t_scale2 = CCScaleTo::create(0.5f, 1.f);
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale1, t_scale2);
	runAction(t_seq);
	
	int stageGold = mySGD->getStageGold();
	
	keep_gold_string = CCString::createWithFormat("%d", stageGold)->getCString();
	base_gold = atof(getString()); // 원래 가지고 있던 골드
	keep_gold = stageGold - base_gold; // 이번에 얻은 골드
	decrease_gold = keep_gold;
	increase_gold = 0.f;
	schedule(schedule_selector(GoldLabel::increasing));
}
void GoldLabel::increasing (float dt)
{
	if(decrease_gold > 0)
	{
		float decreaseUnit = keep_gold / 1.f * dt;
		
		if(decrease_gold < decreaseUnit)
		{
			increase_gold += decrease_gold;
			decrease_gold = 0;
		}
		else {
			if(decreaseUnit <= 0.01)
			{
				increase_gold += decrease_gold;
				decrease_gold = 0;
			}
			else {
				decrease_gold -= decreaseUnit;
				increase_gold += decreaseUnit;
			}
		}
		CCLabelBMFont::setString(CCString::createWithFormat("%.0f",base_gold+increase_gold)->getCString());
	}
	else
		stopIncreasing();
}
void GoldLabel::stopIncreasing ()
{
	unschedule(schedule_selector(GoldLabel::increasing));
	is_incresing = false;
	CCLabelBMFont::setString(keep_gold_string.c_str());
}
void GoldLabel::myInit ()
{
	is_incresing = false;
	CCLabelBMFont::initWithString(CCString::createWithFormat("%d", mySGD->getStageGold())->getCString(), "goldfont.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentRight, CCPointZero);
	stopIncreasing();
	setAnchorPoint(ccp(0,0.5));
	
	setPosition(ccp(52, myDSH->ui_top-50));
//	if(myGD->gamescreen_type == kGT_leftUI)			setPosition(ccp((480-50-myGD->boarder_value*2)*1.1f/4.f+50+myGD->boarder_value,myDSH->ui_top-15));
//	else if(myGD->gamescreen_type == kGT_rightUI)	setPosition(ccp((480-50-myGD->boarder_value*2)*1.1f/4.f+myGD->boarder_value,myDSH->ui_top-15));
//	else											setPosition(ccp((480-myGD->boarder_value*2)*1.1f/4.f,myDSH->ui_top-15));
	
	mySGD->setGoldLabel(this);
}
MyGold * MyGold::create ()
{
	MyGold* t_mg = new MyGold();
	t_mg->myInit();
	t_mg->autorelease();
	return t_mg;
}
void MyGold::startMoving ()
{
	getParent()->reorderChild(this, z_order);
	ing_frame = 0;
	r_dy = i_dy;
	schedule(schedule_selector(MyGold::moving));
}
void MyGold::moving ()
{
	ing_frame++;
	
	setScale(getScale()*dscale);
	setOpacity(getOpacity() - 4);
	after_position = ccpAdd(getPosition(), ccp(0, r_dy));
	r_dy += MY_GRAVITY;
	
	if(after_position.y < init_position.y)
	{
		r_dy = i_dy/2.f;
		i_dy /= 2.f;
		
		after_position.y = init_position.y + r_dy;
	}
	
	after_position = ccpAdd(after_position, t_dv);
	init_position = ccpAdd(init_position, t_dv);
	
	setPosition(after_position);
	
	if(ing_frame > 60)
	{
		unschedule(schedule_selector(MyGold::moving));
		removeFromParentAndCleanup(true);
	}
}
void MyGold::myInit ()
{
	initWithFile("gold.png");
	init_position = CCPointZero;
	setPosition(init_position);
	
	i_dy = rand()%3+6;
	
	float t_angle = rand()%360-180.f;
	t_dv.x = 1;
	t_dv.y = tanf(t_angle/180.f*M_PI);
	
	if(t_angle >= 90.f || t_angle < -90.f)
		t_dv = ccpMult(t_dv, -1.f);
	
	float div_value = sqrtf(powf(t_dv.x, 2.f) + powf(t_dv.y, 2.f));
	t_dv = ccpMult(t_dv, 1.f/div_value);
	
	float t_distance = rand()%40 + 40.f;
	t_dv = ccpMult(t_dv, t_distance/60.f);
	
	dscale = 1.f - t_dv.y/1.3f*0.01f; // -0.1 ~ 0.1
	
	z_order = fabs(t_angle-90);
	if(z_order > 180)	z_order = 360-z_order;
}
GetGold * GetGold::create (CCPoint t_sp, int t_duration_frame)
{
	GetGold* t_gg = new GetGold();
	t_gg->myInit(t_sp, t_duration_frame);
	t_gg->autorelease();
	return t_gg;
}
void GetGold::startMyAction ()
{
	ing_frame = 0;
	schedule(schedule_selector(GetGold::myAction));
}
void GetGold::myAction ()
{
	ing_frame++;
	
	if(ing_frame%create_frame == 0)
	{
		MyGold* t_mg = MyGold::create();
		batch_node->addChild(t_mg);
		t_mg->startMoving();
	}
	
	if(ing_frame%5 == 0)
	{
		AudioEngine::sharedInstance()->playEffect("sound_get_coin.mp3", false);
	}
	
	if(ing_frame >= duration_frame)
	{
		unschedule(schedule_selector(GetGold::myAction));
		schedule(schedule_selector(GetGold::selfRemove));
	}
}
void GetGold::selfRemove ()
{
	if(batch_node->getChildrenCount() <= 0)
	{
		unschedule(schedule_selector(GetGold::selfRemove));
		removeFromParentAndCleanup(true);
	}
}
void GetGold::myInit (CCPoint t_sp, int t_duration_frame)
{
	AudioEngine::sharedInstance()->playEffect("sound_get_coin.mp3", false);
	duration_frame = t_duration_frame;
	mySGD->setGold(mySGD->getGold() + duration_frame);
	
	create_frame = duration_frame/60 + 1;
	
	batch_node = CCSpriteBatchNode::create("gold.png");
	batch_node->setPosition(t_sp);
	addChild(batch_node);
}
GetPercentage * GetPercentage::create (float t_gp, bool is_item)
{
	GetPercentage* t_g = new GetPercentage();
	t_g->myInit(t_gp, is_item);
	t_g->autorelease();
	return t_g;
}
void GetPercentage::startFadeOut ()
{
//	CCFadeOut* t_fadeout1 = CCFadeOut::create(1.f);
//	
//	backImg->runAction(t_fadeout1);
	
	t_value+=3;
	if(t_value >= 255)
		removeFromParent();
	else
		KS::setOpacity(my_label, 255-t_value);
}
void GetPercentage::selfRemove ()
{
	removeFromParentAndCleanup(true);
}
void GetPercentage::myInit (float t_gp, bool is_item)
{
	my_label = KSLabelTTF::create(CCString::createWithFormat("+%.1f%%", t_gp < 0.01f ? 0.f : t_gp)->getCString(), mySGD->getFont().c_str(), 12);
	// CCLabelBMFont::create(CCString::createWithFormat("%.1f%%", t_gp < 0.01f ? 0.f : t_gp)->getCString(), "gain.fnt");
	my_label->setColor(ccYELLOW);
	my_label->enableOuterStroke(ccBLACK, 1.f);
	addChild(my_label, kZorderGetPercentage_label);
	
	t_value = 0;
	
//	if(is_item)
//	{
//		CCSprite* t_texture = CCSprite::create("get_percentage.png");
//		backImg = CCSprite::createWithTexture(t_texture->getTexture(), CCRectMake(0, 24, 52.5, 24));
//		addChild(backImg, kZorderGetPercentage_backImg);
//		
//		CCAnimation* t_animation = CCAnimation::create();
//		t_animation->setDelayPerUnit(0.2);
//		for(int i=1;i<=3;i++)
//			t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(0, i*24, 52.5, 24));
//		CCAnimate* t_animate = CCAnimate::create(t_animation);
//		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(GetPercentage::startFadeOut));
//		CCSequence* t_seq = CCSequence::createWithTwoActions(t_animate, t_call);
//		
//		backImg->runAction(t_seq);
//	}
//	else
//	{
//		backImg = CCSprite::create("get_percentage.png", CCRectMake(0, 0, 52.5, 24));
//		addChild(backImg, kZorderGetPercentage_backImg);
	schedule(schedule_selector(GetPercentage::startFadeOut));
//		startFadeOut();
//	}
}
TakeSpeedUp * TakeSpeedUp::create (int t_step, std::function<void()> t_end_func)
{
	TakeSpeedUp* t_g = new TakeSpeedUp();
	t_g->myInit(t_step, t_end_func);
	t_g->autorelease();
	return t_g;
}
void TakeSpeedUp::startFadeOut ()
{
	CCFadeOut* t_fadeout1 = CCFadeOut::create(1.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(TakeSpeedUp::selfRemove));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fadeout1, t_call);
	
	runAction(t_seq);
}
void TakeSpeedUp::selfRemove ()
{
	end_function();
	removeFromParentAndCleanup(true);
}
void TakeSpeedUp::myInit (int t_step, std::function<void()> t_end_func)
{
	end_function = t_end_func;
	initWithFile(CCString::createWithFormat("speed_step%d.png", t_step)->getCString());
	
	startFadeOut();
}

DetailWarning * DetailWarning::create (const std::string& fileName)
{
	DetailWarning* t_w = new DetailWarning();
	t_w->myInit(fileName);
	t_w->autorelease();
	return t_w;
}
void DetailWarning::startAction ()
{
	//CCMoveTo* t_move1 = CCMoveTo::create(0.4f, ccp(240,myDSH->ui_center_y));
	//CCHide* t_hide = CCHide::create();
	//CCDelayTime* t_delay1 = CCDelayTime::create(0.05f);
	//CCShow* t_show = CCShow::create();
	//CCDelayTime* t_delay2 = CCDelayTime::create(0.1f);
	//CCRepeat* t_repeat = CCRepeat::create(CCSequence::create(t_hide, t_delay1, t_show, t_delay2, NULL), 4);
	//CCMoveTo* t_move2 = CCMoveTo::create(0.4f, ccp(-160,myDSH->ui_center_y));
	//CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(DetailWarning::selfRemove));

	//runAction(CCSequence::create(t_move1, t_repeat, t_move2, t_call, NULL));
}
void DetailWarning::selfRemove ()
{
	removeFromParentAndCleanup(true);
}
void DetailWarning::myInit (const std::string& fileName)
{
	CCSprite* warningSprite = KS::loadCCBI<CCSprite*>(this, fileName).first;
	if(!warningSprite)
	{
		warningSprite = CCSprite::create("attack_warning.png");
	}
	warningSprite->setPosition(ccp(240, myDSH->ui_center_y));
	addChild(warningSprite);
	addChild(KSTimer::create(3.f, [=]()
				{
					removeFromParentAndCleanup(true);
				}));
	//initWithFile("attack_warning.png");

	//if(t1 == 1)
	//{
		//setColor(ccWHITE);
	//}
	//else if(t1 == 2)
	//{
		//setColor(ccRED);
	//}
	//else if(t1 == 3)
	//{
		//setColor(ccGREEN);
	//}
	//setPosition(ccp(640,myDSH->ui_center_y));
}

Warning * Warning::create (int t1)
{
	Warning* t_w = new Warning();
	t_w->myInit(t1);
	t_w->autorelease();
	return t_w;
}
void Warning::startAction ()
{
	CCMoveTo* t_move1 = CCMoveTo::create(0.4f, ccp(240,myDSH->ui_center_y));
	CCHide* t_hide = CCHide::create();
	CCDelayTime* t_delay1 = CCDelayTime::create(0.05f);
	CCShow* t_show = CCShow::create();
	CCDelayTime* t_delay2 = CCDelayTime::create(0.1f);
	CCRepeat* t_repeat = CCRepeat::create(CCSequence::create(t_hide, t_delay1, t_show, t_delay2, NULL), 4);
	CCMoveTo* t_move2 = CCMoveTo::create(0.4f, ccp(-160,myDSH->ui_center_y));
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Warning::selfRemove));
	
	runAction(CCSequence::create(t_move1, t_repeat, t_move2, t_call, NULL));
}
void Warning::selfRemove ()
{
	removeFromParentAndCleanup(true);
}
void Warning::myInit (int t1)
{
	initWithFile("attack_warning.png");
	
	if(t1 == 1)
	{
		setColor(ccWHITE);
	}
	else if(t1 == 2)
	{
		setColor(ccRED);
	}
	else if(t1 == 3)
	{
		setColor(ccGREEN);
	}
	setPosition(ccp(640,myDSH->ui_center_y));
}
TakeCoin * TakeCoin::create ()
{
	TakeCoin* t_w = new TakeCoin();
	t_w->myInit();
	t_w->autorelease();
	return t_w;
}
void TakeCoin::startMyAction()
{
	unschedule(schedule_selector(TakeCoin::startMyAction));
	CCSprite* take_coin = KS::loadCCBI<CCSprite*>(this, "ui_change.ccbi").first;
	addChild(take_coin);
	
	CCDelayTime* t_delay = CCDelayTime::create(3.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(CCNode::removeFromParent));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
	runAction(t_seq);
}
void TakeCoin::myInit ()
{
	setPosition(ccp(240,myDSH->ui_center_y));
	
	schedule(schedule_selector(TakeCoin::startMyAction));
}
AreaScroll * AreaScroll::create ()
{
	AreaScroll* t_w = new AreaScroll();
	t_w->myInit();
	t_w->autorelease();
	return t_w;
}
void AreaScroll::startAction ()
{
	CCSprite* main_view = CCSprite::create("show_area_scroll.png");
	main_view->setPosition(ccp(640,myDSH->ui_center_y));
	addChild(main_view);
	
	CCMoveTo* t_move1 = CCMoveTo::create(0.4f, ccp(240,myDSH->ui_center_y));
	CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(AreaScroll::showArrow));
	CCHide* t_hide = CCHide::create();
	CCDelayTime* t_delay1 = CCDelayTime::create(0.05f);
	CCShow* t_show = CCShow::create();
	CCDelayTime* t_delay2 = CCDelayTime::create(0.1f);
	CCRepeat* t_repeat = CCRepeat::create(CCSequence::create(t_hide, t_delay1, t_show, t_delay2, NULL), 4);
	CCMoveTo* t_move2 = CCMoveTo::create(0.4f, ccp(-160,myDSH->ui_center_y));
	CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(AreaScroll::selfRemove));
	
	main_view->runAction(CCSequence::create(t_move1, t_call1, t_repeat, t_move2, t_call2, NULL));
}
void AreaScroll::selfRemove ()
{
	removeFromParentAndCleanup(true);
}
void AreaScroll::showArrow ()
{
	for(int i=0;i<8;i++)
	{
		CCSprite* t_down = CCSprite::create("area_scroll_down.png");
		t_down->setPosition(ccp(65+i*50, myDSH->ui_center_y-18));
		t_down->setOpacity(0);
		addChild(t_down);
		
		CCFadeTo* down_fade = CCFadeTo::create(0.5f, 255);
		CCMoveBy* down_move = CCMoveBy::create(0.7f, ccp(0,-250));
		CCSequence* down_seq = CCSequence::createWithTwoActions(down_fade, down_move);
		
		t_down->runAction(down_seq);
		
		
		CCSprite* t_up = CCSprite::create("area_scroll_up.png");
		t_up->setPosition(ccp(65+i*50, myDSH->ui_center_y+18));
		t_up->setOpacity(0);
		addChild(t_up);
		
		CCFadeTo* up_fade = CCFadeTo::create(0.5f, 255);
		CCMoveBy* up_move = CCMoveBy::create(0.7f, ccp(0,250));
		CCSequence* up_seq = CCSequence::createWithTwoActions(up_fade, up_move);
		
		t_up->runAction(up_seq);
	}
}
void AreaScroll::myInit ()
{
}
ChangeCard * ChangeCard::create ()
{
	ChangeCard* t_w = new ChangeCard();
	t_w->myInit();
	t_w->autorelease();
	return t_w;
}
void ChangeCard::startMyAction()
{
	unschedule(schedule_selector(ChangeCard::startMyAction));
	CCSprite* change_card = KS::loadCCBI<CCSprite*>(this, "ui_cardchange.ccbi").first;
	addChild(change_card);
	
	CCDelayTime* t_delay = CCDelayTime::create(1.2f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(CCNode::removeFromParent));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
	runAction(t_seq);
}
void ChangeCard::myInit ()
{
	setPosition(ccp(480,myDSH->ui_top*0.67f));
	
	schedule(schedule_selector(ChangeCard::startMyAction));
}
PlayUI * PlayUI::create ()
{
	PlayUI* t_ui = new PlayUI();
	t_ui->myInit();
	t_ui->autorelease();
	return t_ui;
}
PlayUI::~ PlayUI ()
{
	jack_array->removeAllObjects();
	jack_array->release();
	exchange_dic->removeAllObjects();
	exchange_dic->release();
}
void PlayUI::addScore (int t_score)
{
	score_value = score_value.getV() + t_score;
	score_label->setString(CCString::createWithFormat("%d", int(score_value.getV()))->getCString());
	
	if(mySGD->is_write_replay)
	{
		if(mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_scoreTime)].size() > 0)
			myGD->communication("UI_checkScoreTimeVector");
		else
			myGD->communication("UI_writeScore");
	}
}
void PlayUI::decreasePercentage ()
{
	percentage_decrease_cnt++;
	if(percentage_decrease_cnt >= mySD->must_cnt/1000.f)
	{
		beforePercentage ^= t_tta;
		beforePercentage -= 1;
		percentageLabel->setString(CCString::createWithFormat("%.1f%%", beforePercentage/10.f)->getCString());
		if(m_areaGage)
			m_areaGage->setPercentage(beforePercentage/1000.f);
		beforePercentage ^= t_tta;
		percentage_decrease_cnt = 0;
	}
}
float PlayUI::getScore ()
{
	return atoi(score_label->getString());
}
float PlayUI::getPercentage ()
{
	return (beforePercentage^t_tta)/1000.f;
}
void PlayUI::setPercentage (float t_p, bool t_b)
{
	if(isFirst)
	{
		isFirst = false;
		beforePercentage = (int(t_p*1000))^t_tta;
	}
	else
	{
		//			t_p = 0.99f;
		
		myGD->communication("CP_changeMaxSize", t_p);
		float t_beforePercentage = (beforePercentage^t_tta)/1000.f;
		if(t_b)
		{
			AudioEngine::sharedInstance()->playEffect("sound_jack_basic_missile_shoot.mp3", false);
			myLog->addLog(kLOG_getPercent_f, myGD->getCommunication("UI_getUseTime"), t_p-t_beforePercentage);
			
			if((clr_cdt_type == kCLEAR_bigArea || clr_cdt_type == kCLEAR_perfect) && t_p >= t_beforePercentage + 0.001f)
			{
				IntPoint jackPoint = myGD->getJackPoint();
				CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize + 1, (jackPoint.y-1)*pixelSize + 1);
				myGD->communication("Main_percentageGettingEffect", floorf((t_p-t_beforePercentage)*10000.f)/10000.f*100.f, true, jackPosition);
			}
		}
		
		AchieveConditionReward* shared_acr = AchieveConditionReward::sharedInstance();
		
		for(int i=kAchievementCode_luckySeven1;i<=kAchievementCode_luckySeven3;i++)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) == 0 &&
			   t_p == t_beforePercentage + shared_acr->getCondition((AchievementCode)i)/0.001f)
			{
				myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, i, 1);
				AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
			}
		}
		
		
		int item_value = mySGD->getSmallAreaValue();
		
		if(clr_cdt_type == kCLEAR_bigArea && !is_cleared_cdt && t_p - t_beforePercentage >= clr_cdt_per-item_value/100.f)
			takeBigArea();
		
		mySGD->is_draw_button_tutorial = false;
		myGD->communication("Main_offDrawButtonTutorial");
		
		if(t_p >= t_beforePercentage + 0.01f && t_p < clearPercentage)
		{
			int up_count = (t_p - t_beforePercentage)/0.01f;
			my_fp->addFeverGage(up_count);
		}
		
		if(t_p >= t_beforePercentage + NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_percent_d, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1)/100.f)
		{
			float cmCnt = (t_p - t_beforePercentage)/(NSDS_GD(kSDS_GI_characterInfo_int1_statInfo_percent_d, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1)/100.f);
			
			//string missile_code;
			//if(mySGD->getIsUsingFriendCard())
				//missile_code = NSDS_GS(kSDS_CI_int1_missile_type_s, mySGD->getSelectedFriendCardData().card_number);
			//else
				//missile_code = NSDS_GS(kSDS_CI_int1_missile_type_s, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
			//int missile_type = MissileDamageData::getMissileType(missile_code.c_str());
			
			//				myGD->communication("Main_goldGettingEffect", jackPosition, int((t_p - t_beforePercentage)/JM_CONDITION*myDSH->getGoldGetRate()));
			//float missile_speed = NSDS_GD(kSDS_CI_int1_missile_speed_d, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
			int slot_cnt = NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1);

			int t_jack_life = jack_life;
			if(t_jack_life >= slot_cnt)
				t_jack_life = slot_cnt-1;

			int beautystone_id = myDSH->getIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1, slot_cnt-t_jack_life);
			if(beautystone_id > 0)
			{
				int beautystone_type = myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, beautystone_id);
				int beautystone_rank = myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, beautystone_id);
				int beautystone_level = myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, beautystone_id);
				myGD->createJackMissileWithStoneFunctor((StoneType)beautystone_type, beautystone_rank, beautystone_level, cmCnt, myGD->getJackPoint().convertToCCP());
				//myGD->communication("MP_createJackMissile", missile_type, cmCnt, missile_speed, myGD->getJackPoint().convertToCCP());
			}
		}
		
		if(!is_exchanged && !is_show_exchange_coin && !isGameover && t_p < clearPercentage)
		{
			if(t_p >= t_beforePercentage + 0.2f) // 0.2
			{
				takeCoinModeOn();
			}
			else if(t_p >= t_beforePercentage + 0.15f)
			{
				if(rand()%10 < 7) // 70%
					takeCoinModeOn();
			}
			else if(t_p >= t_beforePercentage + 0.1f)
			{
				if(rand()%2 == 0) // 50%
					takeCoinModeOn();
			}
			else if(t_p >= t_beforePercentage + 0.08f)
			{
				if(rand()%20 < 7) // 35%
					takeCoinModeOn();
			}
			else if(t_p >= t_beforePercentage + 0.06f)
			{
				if(rand()%5 == 0) // 20%
					takeCoinModeOn();
			}
		}
		
		beforePercentage = (int(t_p*1000))^t_tta;
	}
	
	if(t_p > 0.5f && !is_show_exchange_coin && t_p < clearPercentage)
	{
		takeCoinModeOn();
	}
	
	percentageLabel->setString(CCString::createWithFormat("%d%%", int(floorf(t_p*10000))/100)->getCString());
	
	int item_value = mySGD->getWidePerfectValue();
	
	if(clr_cdt_type == kCLEAR_perfect && !isGameover && !is_cleared_cdt && floorf(t_p*10000.f)/10000.f*100.f >= (clr_cdt_per-item_value/200.f)*100.f &&
	   floorf(t_p*10000.f)/10000.f*100.f <= (clr_cdt_per+clr_cdt_range+item_value/200.f)*100.f)
		conditionClear();
	
	if(m_areaGage)
		m_areaGage->setPercentage(t_p);
	percentage_decrease_cnt = 0;
	
	if(!isGameover && t_p > clearPercentage) // clear 80%
	{
		myGD->communication("GIM_stopCoin");
		
		if(clr_cdt_type == kCLEAR_timeLimit)
		{
			if(playtime_limit - countingCnt >= ing_cdt_cnt)
				conditionClear();
			else
				conditionFail();
		}
		
		if(clr_cdt_type == kCLEAR_default)
			conditionClear();
		
		if(is_cleared_cdt)
		{
			int boss_count = myGD->getMainCumberCount();
			for(int i=0;i<boss_count;i++)
			{
				myGD->communication("MP_bombCumber", myGD->getMainCumberCCNodeVector()[i]);
			}
			
			isGameover = true;
			myGD->setIsGameover(true);
			myGD->communication("CP_setGameover");
			stopCounting();
			myGD->communication("Main_allStopSchedule");
			myGD->communication("Main_startMoveToBossPosition");
			myGD->communication("CP_startDieAnimation");
			AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
			
			CCDelayTime* t_delay = CCDelayTime::create(1.5f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PlayUI::addResultClearCCB));
			CCSequence* t_seq = CCSequence::create(t_delay, t_call, NULL);
			runAction(t_seq);
			
			endGame(t_p < 1.f && t_p > 0.99f);
		}
		else
		{
			conditionFail();
			
			mySGD->fail_code = kFC_missionfail;
			
			stopCounting();
			// timeover
			isGameover = true;
			myGD->communication("CP_setGameover");
			myGD->communication("Main_allStopSchedule");
			AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
			
			addResultCCB("ui_missonfair.ccbi");
			
			endGame(false);
		}
	}
}

void PlayUI::addResultClearCCB()
{
	addResultCCB("ui_stageclear.ccbi");
}

void PlayUI::addResultCCB(string ccb_filename)
{
	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* reader = new CCBReader(nodeLoader);
	result_sprite = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile(ccb_filename.c_str(),this));
	result_sprite->setPosition(ccp(240,myDSH->ui_center_y));
	addChild(result_sprite);
	reader->release();
}

void PlayUI::conditionClear ()
{
//	removeChildByTag(kCT_UI_clrCdtLabel);
	is_cleared_cdt = true;
	mission_button->doClose();
	mission_button->isSuccessed(true);
//	((CCMenu*)getChildByTag(kCT_UI_clrCdtIcon))->setEnabled(false);
	
//	CCSprite* condition_clear = CCSprite::create("condition_clear.png");
//	condition_clear->setPosition(getChildByTag(kCT_UI_clrCdtIcon)->getPosition());
//	addChild(condition_clear);
	
	CCNode* success_node = CCNode::create();
	success_node->setPosition(ccp(480,myDSH->ui_top*0.67f));
	addChild(success_node);
	
	
	CCSprite* success_ccb = KS::loadCCBI<CCSprite*>(this, "ui_missonsuccess.ccbi").first;
	success_node->addChild(success_ccb);
	
	CCDelayTime* t_delay = CCDelayTime::create(1.2f);
	CCCallFunc* t_call = CCCallFunc::create(success_node, callfunc_selector(CCNode::removeFromParent));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
	success_node->runAction(t_seq);
}
void PlayUI::conditionFail ()
{
//	((CCMenu*)getChildByTag(kCT_UI_clrCdtIcon))->setEnabled(false);
//	
//	CCSprite* condition_fail = CCSprite::create("condition_fail.png");
//	condition_fail->setPosition(getChildByTag(kCT_UI_clrCdtIcon)->getPosition());
//	addChild(condition_fail);
}
void PlayUI::takeExchangeCoin (CCPoint t_start_position, int t_coin_number)
{
	if(isGameover)
		return;
	
	if(clr_cdt_type == kCLEAR_sequenceChange && !isGameover)
	{
		if(!mySGD->isUsingItem(kIC_randomChange) && t_coin_number != ing_cdt_cnt)
		{
			conditionFail();
			
			mySGD->fail_code = kFC_missionfail;
			
			stopCounting();
			// timeover
			isGameover = true;
			myGD->communication("Main_allStopSchedule");
			AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
			
			addResultCCB("ui_missonfair.ccbi");
			
			endGame(false);
			return;
		}
		else
		{
			ing_cdt_cnt++;
			
			if(mySGD->isUsingItem(kIC_randomChange))
			{
				mission_button->setTextAtIndex(CCString::createWithFormat("%d/%d", ing_cdt_cnt-1, 6)->getCString(), 1);
//				((CCLabelTTF*)getChildByTag(kCT_UI_clrCdtLabel))->setString(CCString::createWithFormat("%d/%d", ing_cdt_cnt-1, 6)->getCString());
			}
			else
			{
				mission_button->setTextAtIndex(CCString::createWithFormat("%d/%d", ing_cdt_cnt-1, 6)->getCString(), 1);
//				removeChildByTag(kCT_UI_clrCdtLabel);
//				if(ing_cdt_cnt <= 6)
//				{
//					CCSprite* clr_cdt_img = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", ing_cdt_cnt)->getCString());
//					clr_cdt_img->setPosition(ccpAdd(getChildByTag(kCT_UI_clrCdtIcon)->getPosition(), ccp(0,-5)));
//					addChild(clr_cdt_img, 0, kCT_UI_clrCdtLabel);
//				}
			}
		}
	}
	
	taked_coin_cnt++;
	
	CCSprite* t_coin_spr = (CCSprite*)exchange_dic->objectForKey(t_coin_number);
	CCPoint after_position = t_coin_spr->getPosition();
	t_coin_spr->removeFromParentAndCleanup(true);
	exchange_dic->removeObjectForKey(t_coin_number);
	
	
	CCParticleSystemQuad* coin_particle = CCParticleSystemQuad::createWithTotalParticles(30);
	coin_particle->setPositionType(kCCPositionTypeRelative);
	coin_particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("fever_particle.png"));
	coin_particle->setEmissionRate(60);
	coin_particle->setAngle(90.0);
	coin_particle->setAngleVar(30.0);
	ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
	coin_particle->setBlendFunc(blendFunc);
	coin_particle->setDuration(0.7f);
	coin_particle->setEmitterMode(kCCParticleModeGravity);
	coin_particle->setStartColor(ccc4f(1.f, 0.8f, 0.4f, 1.f));
	coin_particle->setStartColorVar(ccc4f(0,0,0,0));
	coin_particle->setEndColor(ccc4f(1.f,1.f,1.f,0.f));
	coin_particle->setEndColorVar(ccc4f(0, 0, 0, 0));
	coin_particle->setStartSize(13.0);
	coin_particle->setStartSizeVar(5.0);
	coin_particle->setEndSize(-1.0);
	coin_particle->setEndSizeVar(0.0);
	coin_particle->setGravity(ccp(0,80));
	coin_particle->setRadialAccel(0.0);
	coin_particle->setRadialAccelVar(0.0);
	coin_particle->setSpeed(140);
	coin_particle->setSpeedVar(60.0);
	coin_particle->setTangentialAccel(0);
	coin_particle->setTangentialAccelVar(0);
	coin_particle->setTotalParticles(30);
	coin_particle->setLife(0.5);
	coin_particle->setLifeVar(0.3);
	coin_particle->setStartSpin(0.0);
	coin_particle->setStartSpinVar(180.f);
	coin_particle->setEndSpin(0.0);
	coin_particle->setEndSpinVar(180.f);
	coin_particle->setPosVar(ccp(12,12));
	coin_particle->setPosition(t_start_position);
	coin_particle->setAutoRemoveOnFinish(true);
	addChild(coin_particle);
	
	
	CCSprite* new_coin_spr = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", t_coin_number)->getCString());
	new_coin_spr->setPosition(t_start_position);
	top_center_node->addChild(new_coin_spr);
	
	CCDelayTime* t_delay = CCDelayTime::create(0.7f);
	CCMoveTo* t_move = CCMoveTo::create(0.5f, after_position);
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_move);
	new_coin_spr->runAction(t_seq);
	
	
	exchange_dic->setObject(new_coin_spr, t_coin_number);
	
	if(taked_coin_cnt >= 6 && !isGameover && getPercentage() < clearPercentage)
	{
		if(clr_cdt_type == kCLEAR_sequenceChange)
		{
			conditionClear();
		}
		
		isFirst = true;
		is_exchanged = true;
		my_fp->addFeverGage(20);
		
		myGD->communication("Main_startExchange");
		myGD->communication("Main_showChangeCard");
		myGD->communication("Jack_positionRefresh");
		
		m_areaGage->onChange();
	}
}
void PlayUI::subBossLife (float t_life)
{
	//##
	// gamedata 로부터 boss pointer 받아서 해결.
	if(clr_cdt_type != kCLEAR_bossLifeZero || is_cleared_cdt || isGameover)
		return;
	
	//		t_life = MissileDamageData::getCorrelationDamage(t_life, main_cumber_element);
	
	//		if(bossLife < t_life)
	//			bossLife = 0;
	//		else
	//			bossLife -= t_life;
	
	//		m_bossLifeGage->setPercentage(bossLife/maxBossLife);
	KSCumberBase* cb = dynamic_cast<KSCumberBase*>(myGD->getMainCumberCCNodeVector()[0]);
	if(!is_cleared_cdt)
		mission_button->setTextAtIndex(CCString::createWithFormat("%.1f%%", cb->getLife()/cb->getTotalLife()*100.f)->getCString(), 1);
//		((CCLabelTTF*)getChildByTag(kCT_UI_clrCdtLabel))->setString(CCString::createWithFormat("%.1f%%", cb->getLife()/cb->getTotalLife()*100.f)->getCString());
	
	if(cb->getLife() <= 0.f && !is_cleared_cdt)
	{
		conditionClear();
	}
}
void PlayUI::setMaxBossLife (float t_life)
{
	maxBossLife = t_life;
	bossLife = maxBossLife*(100.f - mySGD->getBossLittleEnergyValue())/100.f;
}
void PlayUI::setClearPercentage (float t_p)
{
	clearPercentage = t_p;
	m_areaGage = AreaGage::create(clearPercentage);
	m_areaGage->setPosition(ccp(240,myDSH->ui_top-22));
	top_center_node->addChild(m_areaGage);
	m_areaGage->setPercentage(getPercentage());
}
void PlayUI::startCounting ()
{
	schedule(schedule_selector(PlayUI::counting));
}
void PlayUI::resumeCounting ()
{
	schedule(schedule_selector(PlayUI::counting));
}
void PlayUI::stopCounting ()
{
	unschedule(schedule_selector(PlayUI::counting));
}
void PlayUI::keepBossLife ()
{
	keepLife = bossLife;
}
void PlayUI::checkBossLife ()
{
	//		if((keepLife - bossLife)/maxBossLife > 0.1)
	//		{
	//			myGD->communication("CP_mainCumberShowEmotion", 2);
	//		}
	//		else if((keepLife - bossLife)/maxBossLife < 0.02)
	//		{
	//			myGD->communication("CP_mainCumberShowEmotion", 3);
	//		}
	//		else
	//		{
	//			myGD->communication("CP_mainCumberShowEmotion", 1);
	//		}
}
int PlayUI::getGameTime ()
{
	return countingCnt;
}
//void PlayUI::setControlTD (CCObject * t_main, SEL_CallFunc d_gesture, SEL_CallFunc d_button, SEL_CallFunc d_joystick, SEL_CallFunc d_startControl)
//{
//	target_main = t_main;
//	delegate_gesture = d_gesture;
//	delegate_button = d_button;
//	delegate_joystick = d_joystick;
//	delegate_startControl = d_startControl;
//}
void PlayUI::showPause ()
{
	AudioEngine::sharedInstance()->setAppBack();
	mySGD->is_paused = true;
	
	myGD->communication("Main_showPause");
//	PausePopupLayer* t_ppl = PausePopupLayer::create(this, callfunc_selector(PlayUI::goHome), this, callfunc_selector(PlayUI::cancelHome), target_main, delegate_gesture, delegate_button, delegate_joystick, this, callfunc_selector(PlayUI::goReplay));
//	addChild(t_ppl);
}
void PlayUI::showContinuePopup (CCObject * t_end, SEL_CallFunc d_end, CCObject * t_continue, SEL_CallFunc d_continue)
{
	target_continue = t_continue;
	delegate_continue = d_continue;
	AudioEngine::sharedInstance()->setAppBack();
	mySGD->is_paused = true;
	
	myGD->communication("Main_showContinue", t_end, d_end, this, callfunc_selector(PlayUI::continueAction));
}
void PlayUI::addGameTime30Sec ()
{
	if(countingCnt >= 130)
	{
		AudioEngine::sharedInstance()->stopEffect("sound_time_noti.mp3");
	}
	
	countingCnt -= 30;
	total_time += 30;
	if(mySGD->isUsingItem(kIC_longTime))
	{
		if(countingCnt < -mySGD->getLongTimeValue())
			countingCnt = -mySGD->getLongTimeValue();
	}
	else
	{
		if(countingCnt < 0)
			countingCnt = 0;
	}
}
bool PlayUI::beRevivedJack ()
{
	if(jack_life > 0)
	{
		addGameTime30Sec();
		
		jack_life--;
		jack_life_node->removeChild((CCNode*)jack_array->lastObject(), true);
		jack_array->removeLastObject();
		
		CCMoveTo* t_move = CCMoveTo::create(0.5f, ccp(20,myDSH->ui_top-60));
		jack_life_node->runAction(t_move);
		jack_life_hide_count = 0;
		
		return true;
	}
	else
	{
		return false;
	}
}
void PlayUI::takeAddTimeItem ()
{
	int change_time = NSDS_GI(mySD->getSilType(), kSDS_SI_itemOptionAddTimeSec_i);
	countingCnt -= change_time;
	total_time += change_time;
	
	CCLog("addtime value : %d", change_time);
	if(mySGD->isUsingItem(kIC_longTime))
	{
		if(countingCnt < -mySGD->getLongTimeValue())
			countingCnt = -mySGD->getLongTimeValue();
	}
	else
	{
		if(countingCnt < 0)
			countingCnt = 0;
	}
}
bool PlayUI::getIsExchanged ()
{
	return is_exchanged;
}
int PlayUI::getComboCnt ()
{
	return combo_cnt;
}
void PlayUI::setComboCnt (int t_combo)
{
	int before_combo = combo_cnt;
	combo_cnt = t_combo;
	
	if(before_combo < combo_cnt)
	{
		my_combo->showCombo(t_combo);
		
		AchieveConditionReward* shared_acr = AchieveConditionReward::sharedInstance();
		
		for(int i=kAchievementCode_comboMania1;i<=kAchievementCode_comboMania3;i++)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) == 0 &&
			   combo_cnt == shared_acr->getCondition((AchievementCode)i))
			{
				myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, i, 1);
				AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
			}
		}
	}
	else
	{
		myLog->addLog(kLOG_endCombo_i, myGD->getCommunication("UI_getUseTime"), before_combo);
	}
}
int PlayUI::getUseTime ()
{
	return use_time;
}
void PlayUI::takeCoinModeOn ()
{
	top_center_node->setPosition(ccpAdd(top_center_node->getPosition(), ccp(0,8)));
	
	is_show_exchange_coin = true;
	taked_coin_cnt = 0;
	for(int i=1;i<=6;i++)
	{
		CCSprite* t_gray_coin = (CCSprite*)exchange_dic->objectForKey(i);
		t_gray_coin->setVisible(true);
	}
	myGD->communication("Main_showCoin");
	myGD->communication("Main_showTakeCoin");
}

void PlayUI::checkMapTimeVector()
{
	Json::Value map_time_list = mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_mapTime)];
	int list_size = map_time_list.size();
	
	if(list_size > 0 && map_time_list[list_size-1].asInt() < use_time)
		writeMap();
	else
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_isChangedMap)] = true;
}

void PlayUI::writeMap()
{
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_mapIndex)] = mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_mapTime)].size();
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_mapTime)].append(use_time);
	
	Json::Value writing_map;
	int y, x, record, t_count, i, j;
	for(y = 0;y<43;++y)
	{
		record = 0;
		for(x = 0;x<32;++x)
		{
			t_count = 0;
			for(i = mapWidthInnerBegin+x*5;i<mapWidthInnerBegin+(x+1)*5;++i)
				for(j = mapHeightInnerBegin+y*5;j<mapHeightInnerBegin+(y+1)*5;++j)
					if(myGD->mapState[i][j] == mapOldget || myGD->mapState[i][j] == mapOldline)
						t_count++;
			
			record = record << 1;
			if(t_count >= 13)
			{
				++record;
			}
		}
		writing_map.append(record);
	}
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_mapData)].append(writing_map);
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_isChangedMap)] = false;
}

void PlayUI::checkScoreTimeVector()
{
	Json::Value score_time_list = mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_scoreTime)];
	int list_size = score_time_list.size();
	
	if(list_size > 0 && score_time_list[list_size-1].asInt() < use_time)
		writeScore();
	else
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_isChangedScore)] = true;
}

void PlayUI::writeScore()
{
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_scoreIndex)] = mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_scoreTime)].size();
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_scoreTime)].append(use_time);
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_scoreData)].append(int(getScore()));
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_isChangedScore)] = false;
}

void PlayUI::writePosition()
{
	CCPoint jack_position = myGD->getJackPoint().convertToCCP();
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_characterPositionX)] = jack_position.x;
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_characterPositionY)] = jack_position.y;
	
	
	vector<CCNode*> bossArray = myGD->getMainCumberCCNodeVector();
	int loop_cnt1 = bossArray.size();
	for(int i=0;i<loop_cnt1;i++)
	{
		CCNode* t_boss = (CCNode*)bossArray[i];
		CCPoint t_position = t_boss->getPosition();
		
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_boss)][i][mySGD->getReplayKey(kReplayKey_timeStamp_boss_x)] = t_position.x;
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_boss)][i][mySGD->getReplayKey(kReplayKey_timeStamp_boss_y)] = t_position.y;
	}
	
//	CCNode* boss_node = myGD->getCommunicationNode("CP_getMainCumberPointer");
//	CCPoint boss_position = boss_node->getPosition();
//	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_boss)][i] = boss_position.x;
//	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_boss)][i] = boss_position.y;
	
	vector<CCNode*> subCumberArray = myGD->getSubCumberCCNodeVector();
	int loop_cnt2 = subCumberArray.size();
	for(int i=0;i<loop_cnt2;i++)
	{
		CCNode* t_subCumber = (CCNode*)subCumberArray[i];
		CCPoint t_position = t_subCumber->getPosition();
		
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_monster)][i][mySGD->getReplayKey(kReplayKey_timeStamp_monster_x)] = t_position.x;
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_monster)][i][mySGD->getReplayKey(kReplayKey_timeStamp_monster_y)] = t_position.y;
	}
}

void PlayUI::writeDie()
{
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_isDie)] = true;
}

void PlayUI::writeImageChange()
{
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_isImageChange)] = true;
}

void PlayUI::writeGameOver(int t_i)
{
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_gameInfo)] = t_i;
}

void PlayUI::writeContinue()
{
	mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_isContinue)] = true;
}

void PlayUI::counting ()
{
	if(countingCnt-1 >= playtime_limit-30)
		is_urgent = true;
	else
		is_urgent = false;
	
	detail_counting_cnt++;
	bool t_is_die = false;
	if(detail_counting_cnt >= 60)
	{
		home_menu->setEnabled(true);
		detail_counting_cnt = 0;
		countingCnt++;
		use_time++;
		
		if(jack_life_hide_count != -1)
		{
			jack_life_hide_count++;
			if(jack_life_hide_count > 3)
			{
				CCMoveTo* t_move = CCMoveTo::create(1.f, ccp(20-200, myDSH->ui_top-60));
				jack_life_node->runAction(t_move);
				jack_life_hide_count = -1;
			}
		}
		
		if(mySGD->is_draw_button_tutorial && mySGD->draw_button_tutorial_ing > 0)
		{
			mySGD->draw_button_tutorial_ing++;
			if(mySGD->draw_button_tutorial_ing >= draw_button_tutorial_show)
			{
				myGD->communication("Main_showDrawButtonTutorial");
				mySGD->draw_button_tutorial_ing = 0;
				draw_button_tutorial_show += 10;
			}
		}
		
		if(mySGD->is_write_replay)
		{
			if(mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_isChangedMap)].asBool())
				writeMap();
			
			if(mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_isChangedScore)].asBool())
				writeScore();
			
			writePosition();
		}
		
		if(mySGD->is_play_replay)
		{
			if(mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_mapTime)].size() > 0)
			{
				myGD->communication("Main_refreshReplayThumb", use_time);
				myGD->communication("Main_refreshReplayPosition", use_time);
			}
			
			if(mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_scoreTime)].size() > 0)
				myGD->communication("Main_refreshReplayScore", use_time);
		}
		
		CCRotateBy* t_rotate = CCRotateBy::create(0.5, -180);
		sand_clock->runAction(t_rotate);
		
		if(countingCnt-1 == playtime_limit/3)
		{
			AudioEngine::sharedInstance()->playEffect("sound_time_noti.mp3", false);
			if(myGD->game_step == kGS_limited)
				myGD->communication("Main_setUnlimitMap");
		}
		else if(countingCnt-1 == playtime_limit-50)
		{
			AudioEngine::sharedInstance()->playEffect("sound_time_noti.mp3", false);
		}
		else if(countingCnt-1 == playtime_limit-30)
		{
			AudioEngine::sharedInstance()->playEffect("sound_time_noti.mp3", true);
		}
		
		if(countingCnt-1 >= playtime_limit)
		{
			t_is_die = true;
			//			if(jack_life > 0)
			//			{
			myGD->communication("Jack_startDieEffect", DieType::kDieType_timeover);
			//			}
			//			else
			//			{
			//				stopCounting();
			//				// timeover
			//
			//				mySGD->fail_code = kFC_timeover;
			//
			//				myGD->communication("Main_allStopSchedule");
			//				AudioEngine::sharedInstance()->playEffect("sound_stamp.mp3", false);
			//				result_sprite = CCSprite::create("game_timeover.png");
			//				result_sprite->setRotation(-25);
			//				result_sprite->setPosition(ccp(240,myDSH->ui_center_y));
			//				addChild(result_sprite);
			//
			//				endGame();
			//			}
		}
	}
	
	int label_value = playtime_limit-countingCnt;
	if(label_value < 0)
		label_value = 0;
	
	if(is_urgent)
	{
		countingLabel->setVisible(true);
		
		if(detail_counting_cnt%20 == 0)
		{
			if(countingLabel->getColor().b > 50)
				countingLabel->setColor(ccRED);
			else
				countingLabel->setColor(ccWHITE);
		}
		
		
//		countingLabel->setOpacity(100);
//		countingLabel->setScale(4.f);
//		countingLabel->setPosition(ccp(240,myDSH->ui_center_y));
		if(!t_is_die)
			countingLabel->setString(CCString::createWithFormat("%d.%d", label_value, 9 - detail_counting_cnt/6)->getCString());
		else
		{
			countingLabel->setString(CCString::createWithFormat("%d.%d", 0, 0)->getCString());
			stopCounting();
		}
	}
	else
	{
		if(countingLabel->getOpacity() == 100 && myGD->getCommunicationBool("Main_isFever"))
		{
			countingLabel->setVisible(true);
		}
		
		countingLabel->setColor(ccWHITE);
		countingLabel->setOpacity(255);
		countingLabel->setScale(1.f);
		countingLabel->setPosition(ccp(240,35));
		countingLabel->setString(CCString::createWithFormat("%d.%d", label_value, 9 - detail_counting_cnt/6)->getCString());
	}
	
}
void PlayUI::lifeBonus ()
{
	if(jack_life > 0)
	{
		jack_life--;
		CCSprite* t_jack = (CCSprite*)jack_array->lastObject();
		jack_array->removeObject(t_jack);
		
		CCParticleSystemQuad* t_particle = CCParticleSystemQuad::createWithTotalParticles(100);
		t_particle->setPositionType(kCCPositionTypeRelative);
		t_particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("bonus_score_particle.png"));
		t_particle->setEmissionRate(100);
		t_particle->setAngle(90.0);
		t_particle->setAngleVar(180.0);
		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
		t_particle->setBlendFunc(blendFunc);
		t_particle->setDuration(-1.f);
		t_particle->setEmitterMode(kCCParticleModeGravity);
		t_particle->setStartColor(ccc4f(1.f, 0.8f, 0.4f, 1.f));
		t_particle->setStartColorVar(ccc4f(0.f, 0.f, 0.f, 0.f));
		t_particle->setEndColor(ccc4f(1.f, 1.f, 1.f, 0.f));
		t_particle->setEndColorVar(ccc4f(0.f, 0.f, 0.f, 0.f));
		t_particle->setStartSize(30.0);
		t_particle->setStartSizeVar(10.0);
		t_particle->setEndSize(-1.0);
		t_particle->setEndSizeVar(0.0);
		t_particle->setGravity(ccp(0,0));
		t_particle->setRadialAccel(0.0);
		t_particle->setRadialAccelVar(0.0);
		t_particle->setSpeed(40);
		t_particle->setSpeedVar(20.0);
		t_particle->setTangentialAccel(0);
		t_particle->setTangentialAccelVar(0);
		t_particle->setTotalParticles(100);
		t_particle->setLife(1.0);
		t_particle->setLifeVar(0.25);
		t_particle->setStartSpin(0);
		t_particle->setStartSpinVar(0);
		t_particle->setEndSpin(0);
		t_particle->setEndSpinVar(0);
		t_particle->setPosVar(ccp(10,10));
		t_particle->setPosition(ccpAdd(t_jack->getPosition(), t_jack->getParent()->getPosition()));
		t_particle->setAutoRemoveOnFinish(true);
		addChild(t_particle);
		
		CCMoveTo* particle_move = CCMoveTo::create(0.5f, score_label->getPosition());
		CCCallFuncO* particle_remove = CCCallFuncO::create(this, callfuncO_selector(PlayUI::removeParticle), t_particle);
		CCSequence* particle_seq = CCSequence::createWithTwoActions(particle_move, particle_remove);
		t_particle->runAction(particle_seq);
		
		CCFadeTo* jack_fade = CCFadeTo::create(0.5f, 0);
		CCCallFunc* jack_remove = CCCallFunc::create(t_jack, callfunc_selector(CCSprite::removeFromParent));
		CCSequence* jack_seq = CCSequence::createWithTwoActions(jack_fade, jack_remove);
		t_jack->runAction(jack_seq);
	}
	else
	{
		int grade_value = 1;
		if(is_exchanged)				grade_value++;
		if(keep_percentage.getV() >= 1.f)					grade_value++;
		
		mySGD->gameClear(grade_value, atoi(score_label->getString()), keep_percentage.getV(), countingCnt, use_time, total_time);
		CCDelayTime* n_d = CCDelayTime::create(2.5f);
		CCCallFunc* nextScene = CCCallFunc::create(this, callfunc_selector(PlayUI::nextScene));
		
		CCSequence* sequence = CCSequence::createWithTwoActions(n_d, nextScene);
		
		runAction(sequence);
	}
}
void PlayUI::removeParticle (CCObject * sender)
{
	((CCParticleSystemQuad*)sender)->setDuration(0);
	addScore(getScore()*0.1f);
	lifeBonus();
}
void PlayUI::createBonusScore ()
{
	my_combo->stopKeep();
	CCSprite* bonus_score = CCSprite::create("bonus_score.png");
	bonus_score->setOpacity(0);
	bonus_score->setPosition(ccp(240,myDSH->ui_center_y+50));
	addChild(bonus_score);
	
	CCMoveTo* t_move = CCMoveTo::create(0.5f, ccp(20,myDSH->ui_top-60));
	jack_life_node->runAction(t_move);
	
	CCFadeTo* t_fade = CCFadeTo::create(1.f, 255);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PlayUI::lifeBonus));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
	bonus_score->runAction(t_seq);
}
void PlayUI::endGame (bool is_show_reason)
{
	AudioEngine::sharedInstance()->stopEffect("sound_time_noti.mp3");
	//		myGD->communication("CP_setGameover");
	if(myGD->getIsGameover())
	{
		if(!is_show_reason)
		{
			keep_percentage = getPercentage();
			
			if(jack_life > 0)
			{
				CCDelayTime* t_delay = CCDelayTime::create(4.f);
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PlayUI::createBonusScore));
				CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
				runAction(t_seq);
			}
			else
			{
				
				int grade_value = 1;
				if(is_exchanged)				grade_value++;
				if((beforePercentage^t_tta)/1000.f >= 1.f)					grade_value++;
				
				mySGD->gameClear(grade_value, atoi(score_label->getString()), (beforePercentage^t_tta)/1000.f, countingCnt, use_time, total_time);
				CCDelayTime* n_d = CCDelayTime::create(4.5f);
				CCCallFunc* nextScene = CCCallFunc::create(this, callfunc_selector(PlayUI::nextScene));
				
				CCSequence* sequence = CCSequence::createWithTwoActions(n_d, nextScene);
				
				runAction(sequence);
			}
		}
		else
		{
			keep_percentage = getPercentage();
			
			CCDelayTime* n_d1 = CCDelayTime::create(4.5f);
			CCCallFunc* nextScene1 = CCCallFunc::create(this, callfunc_selector(PlayUI::searchEmptyPosition));
			CCDelayTime* n_d2 = CCDelayTime::create(2.f);
			CCFiniteTimeAction* nextScene2;
//			if(mySGD->getStar() >= mySGD->getGachaOnePercentFee())
//			{
				nextScene2 = CCCallFunc::create(this, callfunc_selector(PlayUI::showGachaOnePercent));
//			}
//			else
//			{
//				if(jack_life > 0)
//				{
//					CCDelayTime* t_delay = CCDelayTime::create(2.f);
//					CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PlayUI::createBonusScore));
//					nextScene2 = CCSequence::createWithTwoActions(t_delay, t_call);
//				}
//				else
//				{
//					int grade_value = 1;
//					if(is_exchanged)				grade_value++;
//					if((beforePercentage^t_tta)/1000.f >= 1.f)					grade_value++;
//					
//					mySGD->gameClear(grade_value, atoi(score_label->getString()), (beforePercentage^t_tta)/1000.f, countingCnt, use_time, total_time);
//					nextScene2 = CCCallFunc::create(this, callfunc_selector(PlayUI::nextScene));
//				}
//			}
			
			CCSequence* sequence = CCSequence::create(n_d1, nextScene1, n_d2, nextScene2, NULL);
			
			runAction(sequence);
		}
	}
	else
	{
		CCDelayTime* t_delay = CCDelayTime::create(2.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PlayUI::nextScene));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		runAction(t_seq);
	}
	
	myGD->communication("Main_hideThumb");
}
void PlayUI::showGachaOnePercent ()
{
	OnePercentGacha* t_popup = OnePercentGacha::create(this, callfunc_selector(PlayUI::cancelOnePercentGacha), this, callfuncF_selector(PlayUI::gachaOnOnePercent), keep_percentage.getV());
	addChild(t_popup);
}
void PlayUI::gachaOnOnePercent (float t_percent)
{
	vector<SaveUserData_Key> save_userdata_list;
	
	save_userdata_list.push_back(kSaveUserData_Key_star);
	
	myDSH->saveUserData(save_userdata_list, nullptr);
	
	beforePercentage = (int(t_percent*1000))^t_tta;
	m_areaGage->setPercentage(t_percent);
	
	if(jack_life > 0)
	{
		createBonusScore();
	}
	else
	{
		int grade_value = 1;
		if(is_exchanged && t_percent >= 1.f)		grade_value+=2;
		else
		{
			if(t_percent >= 1.f)
			{
				grade_value++;
			}
		}
		
		mySGD->gameClear(grade_value, atoi(score_label->getString()), t_percent, countingCnt, use_time, total_time);
		nextScene();
	}
}
void PlayUI::searchEmptyPosition ()
{
	CCPoint found_empty_position;
	bool break_flag = false;
	for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd && !break_flag;i++)
	{
		for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd && !break_flag;j++)
		{
			if(myGD->mapState[i][j] == mapEmpty && mySD->silData[i][j])
			{
				break_flag = true;
				found_empty_position = IntPoint(i,j).convertToCCP();
			}
		}
	}
	
	myGD->communication("MS_showEmptyPoint", found_empty_position);
	myGD->communication("Main_startMoveToCCPoint", found_empty_position);
}
void PlayUI::cancelOnePercentGacha ()
{
	if(jack_life > 0)
	{
		createBonusScore();
	}
	else
	{
		int grade_value = 1;
		if(is_exchanged)				grade_value++;
		if(keep_percentage.getV() >= 1.f)					grade_value++;
		
		mySGD->gameClear(grade_value, atoi(score_label->getString()), keep_percentage.getV(), countingCnt, use_time, total_time);
		nextScene();
	}
}
void PlayUI::nextScene ()
{
	if(mySGD->is_write_replay)
	{
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)][use_time][mySGD->getReplayKey(kReplayKey_timeStamp_scoreIndex)] = mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_scoreTime)].size();
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_scoreTime)].append(use_time);
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_scoreData)].append(int(mySGD->getScore()));
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_isChangedScore)] = false;
	}
	
	result_sprite->setVisible(false);
	myGD->communication("Main_gameover");
}
void PlayUI::catchSubCumber ()
{
	if(is_cleared_cdt || clr_cdt_type != kCLEAR_subCumberCatch || isGameover)
		return;
	
	ing_cdt_cnt++;
	
	mission_button->setTextAtIndex(CCString::createWithFormat("%d/%d", ing_cdt_cnt, clr_cdt_cnt)->getCString(), 1);
//	((CCLabelTTF*)getChildByTag(kCT_UI_clrCdtLabel))->setString(CCString::createWithFormat("%d/%d", ing_cdt_cnt, clr_cdt_cnt)->getCString());
	if(ing_cdt_cnt >= clr_cdt_cnt)		conditionClear();
}
void PlayUI::takeBigArea ()
{
	if(is_cleared_cdt || clr_cdt_type != kCLEAR_bigArea || isGameover)
		return;
	
	ing_cdt_cnt++;
	
	int item_value = mySGD->getSmallAreaValue();
	
	mission_button->setTextAtIndex(CCString::createWithFormat("%2.0f%%:%d/%d", (clr_cdt_per-item_value/100.f)*100.f, ing_cdt_cnt, clr_cdt_cnt)->getCString(), 1);
//	((CCLabelTTF*)getChildByTag(kCT_UI_clrCdtLabel))->setString(CCString::createWithFormat("%2.0f%%:%d/%d", (clr_cdt_per-item_value/100.f)*100.f, ing_cdt_cnt, clr_cdt_cnt)->getCString());
	if(ing_cdt_cnt >= clr_cdt_cnt)		conditionClear();
}
void PlayUI::takeItemCollect ()
{
	if(is_cleared_cdt || clr_cdt_type != kCLEAR_itemCollect || isGameover)
		return;
	
	ing_cdt_cnt++;
	
	mission_button->setTextAtIndex(CCString::createWithFormat("%d/%d", ing_cdt_cnt, clr_cdt_cnt)->getCString(), 1);
//	((CCLabelTTF*)getChildByTag(kCT_UI_clrCdtLabel))->setString(CCString::createWithFormat("%d/%d", ing_cdt_cnt, clr_cdt_cnt)->getCString());
	if(ing_cdt_cnt >= clr_cdt_cnt)		conditionClear();
}

void PlayUI::setUseFriendCard()
{
	mySGD->setIsUsingFriendCard(true);
	
	myGD->communication("CP_chagePassiveData", mySGD->getSelectedFriendCardData().card_passive);
	
	jack_life++;
	
	CCSprite* jack_img = CCSprite::create("basic_character.png");
	jack_img->setColor(ccGREEN);
	jack_img->setOpacity(0);
	jack_img->setPosition(ccp((jack_life-1)*20, 0));
//	if(myGD->gamescreen_type == kGT_leftUI)			jack_img->setPosition(ccp(25, myDSH->ui_center_y-30-(jack_life-1)*20));
//	else if(myGD->gamescreen_type == kGT_rightUI)	jack_img->setPosition(ccp(480-25,myDSH->ui_center_y-30-(jack_life-1)*20));
//	else											jack_img->setPosition(ccp(80+(jack_life-1)*20, myDSH->ui_top-35));
	jack_life_node->addChild(jack_img);
	
	jack_img->runAction(CCFadeTo::create(1.f, 255));
	
	jack_array->addObject(jack_img);
}

void PlayUI::myInit ()
{
	isGameover = false;
	
	score_value = 0;
	
	percentage_decrease_cnt = 0;
	combo_cnt = 0;
	
	clearPercentage = 1;
	
	if(mySD->getSilType() == 1 && !myDSH->getBoolForKey(kDSH_Key_isDisableDrawButton))
	{
		mySGD->is_draw_button_tutorial = true;
		mySGD->draw_button_tutorial_ing = 1;
		draw_button_tutorial_show = 11;
	}
	else
	{
		mySGD->is_draw_button_tutorial = false;
		mySGD->draw_button_tutorial_ing = 0;
	}
	
	ui_case = CCSprite::create("ui_back.png");
	if(myGD->gamescreen_type == kGT_leftUI)			ui_case->setPosition(ccp(25,myDSH->ui_center_y));
	else if(myGD->gamescreen_type == kGT_rightUI)		ui_case->setPosition(ccp(480-25,myDSH->ui_center_y));
	else
	{
		ui_case->setPosition(ccp(25,myDSH->ui_center_y));
		ui_case->setOpacity(0);
	}
	addChild(ui_case);
	
//	gold_label = GoldLabel::create();
//	addChild(gold_label);
//	gold_label->setString("0");
	
//	CCSprite* gold_img = CCSprite::create("ui_gold_img.png");
//	gold_img->setPosition(ccpAdd(gold_label->getPosition(), ccp(-15,9)));
//	addChild(gold_img);
	
	score_label = CountingBMLabel::create("0", "scorefont.fnt", 2.f, "%d");
	((CountingBMLabel*)score_label)->onChangeScale(false);
	score_label->setAnchorPoint(ccp(1,1));
	score_label->setPosition(ccp(480-8,myDSH->ui_top-13));
//	if(myGD->gamescreen_type == kGT_leftUI)			score_label->setPosition(ccp((480-50-myGD->boarder_value*2)/2.f+50+myGD->boarder_value,myDSH->ui_top-15));
//	else if(myGD->gamescreen_type == kGT_rightUI)	score_label->setPosition(ccp((480-50-myGD->boarder_value*2)/2.f+myGD->boarder_value,myDSH->ui_top-15));
//	else											score_label->setPosition(ccp(240,myDSH->ui_top-15));
	addChild(score_label);
	
	top_center_node = CCNode::create();
	addChild(top_center_node, 1);
	
	m_areaGage = NULL;
	
	percentageLabel = KSLabelTTF::create("0%", mySGD->getFont().c_str(), 14);// CCLabelTTF::create("0%%", mySGD->getFont().c_str(), 14);
	percentageLabel->setAnchorPoint(ccp(0.5, 0.5));
	percentageLabel->enableOuterStroke(ccBLACK, 1.f);
	percentageLabel->setPosition(ccp(185,myDSH->ui_top-22));
//	if(myGD->gamescreen_type == kGT_leftUI)			percentageLabel->setPosition(ccp(36,myDSH->ui_center_y));
//	else if(myGD->gamescreen_type == kGT_rightUI)		percentageLabel->setPosition(ccp(480-50+36,myDSH->ui_center_y));
//	else									percentageLabel->setPosition(ccp(470,myDSH->ui_top-60));
	top_center_node->addChild(percentageLabel, 1);
	
//	CCSprite* percentage_gain = CCSprite::create("ui_gain.png");
//	percentage_gain->setAnchorPoint(ccp(0,0.5));
//	percentage_gain->setPosition(ccp(205,myDSH->ui_top-35));
//	addChild(percentage_gain);
//	
//	CCSprite* percentage_p = CCSprite::create("maingame_percentage.png");
//	percentage_p->setAnchorPoint(ccp(0,0.5));
//	percentage_p->setPosition(ccpAdd(percentageLabel->getPosition(), ccp(2,0)));
//	addChild(percentage_p);
	
	
	//		CCSprite* counting_tiem = CCSprite::create("maingame_time.png");
	//		counting_tiem->setPosition(ccp(140, 410));
	//		addChild(counting_tiem);
	
	sand_clock = CCSprite::create("maingame_ui_sandclock.png");
	sand_clock->setPosition(ccp(147,463));
	addChild(sand_clock);
	
	countingCnt = -mySGD->getLongTimeValue();
	detail_counting_cnt = 0;
	is_urgent = false;
	use_time = 0;
	playtime_limit = mySDS->getIntegerForKey(kSDF_stageInfo, mySD->getSilType(), "playtime");
	total_time = playtime_limit;
	
//	CCSprite* time_back = CCSprite::create("ui_time_back.png");
//	time_back->setPosition(ccp(240-25,myDSH->ui_top-25));
//	if(myGD->gamescreen_type == kGT_leftUI)			time_back->setPosition(ccp((480-50-myGD->boarder_value*2)*3.1f/4.f+50+myGD->boarder_value,myDSH->ui_top-25));
//	else if(myGD->gamescreen_type == kGT_rightUI)	time_back->setPosition(ccp((480-50-myGD->boarder_value*2)*3.1f/4.f+myGD->boarder_value,myDSH->ui_top-25));
//	else											time_back->setPosition(ccp(480.f*3.1f/4.f,myDSH->ui_top-25));
//	addChild(time_back);
	
	countingLabel = CCLabelBMFont::create(CCString::createWithFormat("%d", playtime_limit-countingCnt)->getCString(), "timefont.fnt");
	countingLabel->setAlignment(kCCTextAlignmentCenter);
	countingLabel->setAnchorPoint(ccp(0.5f,0.5f));
	countingLabel->setPosition(ccp(240,35));
	addChild(countingLabel);
	
	isFirst = true;
	//		beforePercentage = 0;
	
	
	m_bossLifeGage = BossLifeGage::create();
	m_bossLifeGage->setPosition(ccp(108,600));//441));
	addChild(m_bossLifeGage);
	
	
	CCSprite* n_home = CCSprite::create("ui_stop.png");
	CCSprite* s_home = CCSprite::create("ui_stop.png");
	s_home->setColor(ccGRAY);
	
	CCMenuItem* home_item = CCMenuItemSprite::create(n_home, s_home, this, menu_selector(PlayUI::menuAction));
	home_item->setTag(kMenuTagUI_home);
	
	home_menu = CCMenu::createWithItem(home_item);
	home_menu->setPosition(ccp(25, myDSH->ui_top-22));
//	if(myGD->gamescreen_type == kGT_leftUI)				home_menu->setPosition(ccp(25,myDSH->ui_top-25));
//	else if(myGD->gamescreen_type == kGT_rightUI)		home_menu->setPosition(ccp(480-25,myDSH->ui_top-25));
//	else												home_menu->setPosition(ccp(25,myDSH->ui_top-25));
	home_menu->setEnabled(false);
	addChild(home_menu);
	
	
	jack_array = new CCArray(1);
	jack_life_hide_count = 0;
	jack_life = NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1)-1;//NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_life_i, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1)-1;
	
	jack_life_node = CCNode::create();
	jack_life_node->setPosition(ccp(20,myDSH->ui_top-60));
	addChild(jack_life_node);
	for(int i=0;i<jack_life;i++)
	{
		CCSprite* jack_img = CCSprite::create("basic_character.png");
		jack_img->setPosition(ccp(i*20, 0));
		jack_life_node->addChild(jack_img);
		
		jack_array->addObject(jack_img);
	}
	
	is_exchanged = false;
	is_show_exchange_coin = false;
	exchange_dic = new CCDictionary();
	
	for(int i=1;i<=6;i++)
	{
		CCSprite* exchange_spr = CCSprite::create(CCString::createWithFormat("exchange_%d_unact.png", i)->getCString());
		if(myGD->gamescreen_type == kGT_leftUI)			exchange_spr->setPosition(ccp(240-20*3.5f+i*20,myDSH->ui_top-37));
		else if(myGD->gamescreen_type == kGT_rightUI)		exchange_spr->setPosition(ccp(240-20*3.5f+i*20,myDSH->ui_top-37));
		else									exchange_spr->setPosition(ccp(240-20*3.5f+i*20,myDSH->ui_top-37));
		top_center_node->addChild(exchange_spr);
		
		exchange_spr->setVisible(false);
		
		exchange_dic->setObject(exchange_spr, i);
	}
	
	is_show_condition = false;
	clr_cdt_type = mySD->getClearCondition();
	
	mission_button = RollingButton::create("");
	mission_button->setPosition(ccp(64, myDSH->ui_top-22));
	addChild(mission_button);
	
	mission_button->startMarquee();
	
	mission_button->setOpenFunction([&](){
		mission_button->runAction(CCMoveBy::create(0.3,ccp(176,0)));
		top_center_node->setVisible(false);
	});
	
	mission_button->setCloseFunction([&](){
		mission_button->runAction(CCMoveBy::create(0.3,ccp(-176,0)));
		top_center_node->setVisible(true);
	});
	
//	CCPoint icon_menu_position;
//	if(myGD->gamescreen_type == kGT_leftUI)				icon_menu_position = ccp(25,myDSH->ui_center_y+43);
//	else if(myGD->gamescreen_type == kGT_rightUI)		icon_menu_position = ccp(480-25,myDSH->ui_center_y+43);
//	else												icon_menu_position = ccp(440,myDSH->ui_top-25);
//	
//	CCSprite* condition_back = CCSprite::create("ui_condition_back.png");
//	condition_back->setPosition(icon_menu_position);
//	addChild(condition_back);
	
	if(clr_cdt_type == kCLEAR_bossLifeZero)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
//		CCSprite* n_icon = CCSprite::create("condition1_menu.png");
//		CCSprite* s_icon = CCSprite::create("condition1_menu.png");
//		s_icon->setColor(ccGRAY);
//		
//		CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
//		icon_item->setTag(kMenuTagUI_condition);
//		
//		CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
//		icon_menu->setPosition(icon_menu_position);
//		addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
		
		int start_percentage = 100 - mySGD->getBossLittleEnergyValue();
		
		mission_button->setTextAtIndex(mySD->getConditionContent().c_str(), 0);
		mission_button->addText(CCString::createWithFormat("%d%%", start_percentage)->getCString());
		
//		CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d%%", start_percentage)->getCString(), mySGD->getFont().c_str(), 12);
//		clr_cdt_label->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
//		addChild(clr_cdt_label, 0, kCT_UI_clrCdtLabel);
	}
	else if(clr_cdt_type == kCLEAR_subCumberCatch)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
//		CCSprite* n_icon = CCSprite::create("condition2_menu.png");
//		CCSprite* s_icon = CCSprite::create("condition2_menu.png");
//		s_icon->setColor(ccGRAY);
//		
//		CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
//		icon_item->setTag(kMenuTagUI_condition);
//		
//		CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
//		icon_menu->setPosition(icon_menu_position);
//		addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
		
		clr_cdt_cnt = mySD->getClearConditionCatchSubCumber();
		ing_cdt_cnt = 0;
		
		mission_button->setTextAtIndex(mySD->getConditionContent().c_str(), 0);
		mission_button->addText(CCString::createWithFormat("%d/%d", ing_cdt_cnt, clr_cdt_cnt)->getCString());
		
//		CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d/%d", ing_cdt_cnt, clr_cdt_cnt)->getCString(), mySGD->getFont().c_str(), 12);
//		clr_cdt_label->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
//		addChild(clr_cdt_label, 0, kCT_UI_clrCdtLabel);
	}
	else if(clr_cdt_type == kCLEAR_bigArea)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
//		CCSprite* n_icon = CCSprite::create("condition3_menu.png");
//		CCSprite* s_icon = CCSprite::create("condition3_menu.png");
//		s_icon->setColor(ccGRAY);
//		
//		CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
//		icon_item->setTag(kMenuTagUI_condition);
//		
//		CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
//		icon_menu->setPosition(icon_menu_position);
//		addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
		
		clr_cdt_per = mySD->getClearConditionBigAreaPer();
		clr_cdt_cnt = mySD->getClearConditionBigAreaCnt();
		ing_cdt_cnt = 0;
		
		int item_value = mySGD->getSmallAreaValue();
		
		mission_button->setTextAtIndex(mySD->getConditionContent().c_str(), 0);
		mission_button->addText(CCString::createWithFormat("%2.0f%%:%d/%d", (clr_cdt_per-item_value/100.f)*100.f, ing_cdt_cnt, clr_cdt_cnt)->getCString());
//		
//		CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%2.0f%%:%d/%d", (clr_cdt_per-item_value/100.f)*100.f, ing_cdt_cnt, clr_cdt_cnt)->getCString(), mySGD->getFont().c_str(), 12);
//		clr_cdt_label->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
//		addChild(clr_cdt_label, 0, kCT_UI_clrCdtLabel);
	}
	else if(clr_cdt_type == kCLEAR_itemCollect)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
//		CCSprite* n_icon = CCSprite::create("condition4_menu.png");
//		CCSprite* s_icon = CCSprite::create("condition4_menu.png");
//		s_icon->setColor(ccGRAY);
//		
//		CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
//		icon_item->setTag(kMenuTagUI_condition);
//		
//		CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
//		icon_menu->setPosition(icon_menu_position);
//		addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
		
		clr_cdt_cnt = mySD->getClearConditionItemCollect();
		ing_cdt_cnt = 0;
		
		mission_button->setTextAtIndex(mySD->getConditionContent().c_str(), 0);
		mission_button->addText(CCString::createWithFormat("%d/%d", ing_cdt_cnt, clr_cdt_cnt)->getCString());
		
//		CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d/%d", ing_cdt_cnt, clr_cdt_cnt)->getCString(), mySGD->getFont().c_str(), 12);
//		clr_cdt_label->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
//		addChild(clr_cdt_label, 0, kCT_UI_clrCdtLabel);
	}
	else if(clr_cdt_type == kCLEAR_perfect)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
//		CCSprite* n_icon = CCSprite::create("condition5_menu.png");
//		CCSprite* s_icon = CCSprite::create("condition5_menu.png");
//		s_icon->setColor(ccGRAY);
//		
//		CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
//		icon_item->setTag(kMenuTagUI_condition);
//		
//		CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
//		icon_menu->setPosition(icon_menu_position);
//		addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
		
		clr_cdt_per = mySD->getClearConditionPerfectBase();
		clr_cdt_range = mySD->getClearConditionPerfectRange();
		
		mission_button->setTextAtIndex(mySD->getConditionContent().c_str(), 0);
		mission_button->addText(CCString::createWithFormat("%.0f", clr_cdt_per*100.f)->getCString());
		
//		CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", clr_cdt_per*100.f)->getCString(), mySGD->getFont().c_str(), 12);
//		clr_cdt_label->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
//		addChild(clr_cdt_label, 0, kCT_UI_clrCdtLabel);
	}
	else if(clr_cdt_type == kCLEAR_sequenceChange)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
//		CCSprite* n_icon = CCSprite::create("condition6_menu.png");
//		CCSprite* s_icon = CCSprite::create("condition6_menu.png");
//		s_icon->setColor(ccGRAY);
//		
//		CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
//		icon_item->setTag(kMenuTagUI_condition);
//		
//		CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
//		icon_menu->setPosition(icon_menu_position);
//		addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
		
		ing_cdt_cnt = 1;
		
		if(mySGD->isUsingItem(kIC_randomChange))
		{
			mission_button->setTextAtIndex(mySD->getConditionContent().c_str(), 0);
			mission_button->addText(CCString::createWithFormat("%d/%d", ing_cdt_cnt-1, 6)->getCString());
			
//			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d/%d", ing_cdt_cnt-1, 6)->getCString(), mySGD->getFont().c_str(), 12);
//			clr_cdt_label->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
//			addChild(clr_cdt_label, 0, kCT_UI_clrCdtLabel);
		}
		else
		{
			mission_button->setTextAtIndex(mySD->getConditionContent().c_str(), 0);
			mission_button->addText(CCString::createWithFormat("exchange_%d_act.png", ing_cdt_cnt)->getCString());
			
//			CCSprite* clr_cdt_img = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", ing_cdt_cnt)->getCString());
//			clr_cdt_img->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
//			addChild(clr_cdt_img, 0, kCT_UI_clrCdtLabel);
		}
	}
	else if(clr_cdt_type == kCLEAR_timeLimit)
	{
		mission_button->doOpen();
		
		is_cleared_cdt = false;
		
//		CCSprite* n_icon = CCSprite::create("condition7_menu.png");
//		CCSprite* s_icon = CCSprite::create("condition7_menu.png");
//		s_icon->setColor(ccGRAY);
//		
//		CCMenuItem* icon_item = CCMenuItemSprite::create(n_icon, s_icon, this, menu_selector(PlayUI::menuAction));
//		icon_item->setTag(kMenuTagUI_condition);
//		
//		CCMenu* icon_menu = CCMenu::createWithItem(icon_item);
//		icon_menu->setPosition(icon_menu_position);
//		addChild(icon_menu, 0, kCT_UI_clrCdtIcon);
		
		ing_cdt_cnt = mySD->getClearConditionTimeLimit();
		
		mission_button->setTextAtIndex(mySD->getConditionContent().c_str(), 0);
		mission_button->addText(CCString::createWithFormat("%d", ing_cdt_cnt)->getCString());
		
//		CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", ing_cdt_cnt)->getCString(), mySGD->getFont().c_str(), 12);
//		clr_cdt_label->setPosition(ccpAdd(icon_menu->getPosition(), ccp(0,-5)));
//		addChild(clr_cdt_label, 0, kCT_UI_clrCdtLabel);
	}
	else if(clr_cdt_type == kCLEAR_default)
	{
		if(mySD->getSilType() != 1)
		{
			mission_button->doClose();
		}
		else
		{
			CCNode* t_node = CCNode::create();
			mission_button->addChild(t_node);
			CCDelayTime* t_delay = CCDelayTime::create(4.f);
			CCCallFunc* t_call1 = CCCallFunc::create(mission_button, callfunc_selector(RollingButton::doClose));
			CCCallFunc* t_call2 = CCCallFunc::create(t_node, callfunc_selector(CCNode::removeFromParent));
			CCSequence* t_seq = CCSequence::create(t_delay, t_call1, t_call2, NULL);
			t_node->runAction(t_seq);
			
			mission_button->doOpen();
		}
		
		is_cleared_cdt = true;
		
		mission_button->setTextAtIndex(mySD->getConditionContent().c_str(), 0);
		
//		CCSprite* icon_img = CCSprite::create("condition0_menu.png");
//		icon_img->setPosition(icon_menu_position);
//		addChild(icon_img, 0, kCT_UI_clrCdtIcon);
	}
	
	my_combo = ComboParent::create(score_label);
	my_combo->setPosition(CCPointZero);
	addChild(my_combo);
	
	my_fp = FeverParent::create(countingLabel);
	my_fp->setPosition(CCPointZero);
	addChild(my_fp);
	
	myGD->V_I["UI_addScore"] = std::bind(&PlayUI::addScore, this, _1);
	myGD->V_FB["UI_setPercentage"] = std::bind(&PlayUI::setPercentage, this, _1, _2);
	myGD->V_F["UI_subBossLife"] = std::bind(&PlayUI::subBossLife, this, _1);
	myGD->V_V["UI_decreasePercentage"] = std::bind(&PlayUI::decreasePercentage, this);
	myGD->B_V["UI_beRevivedJack"] = std::bind(&PlayUI::beRevivedJack, this);
	myGD->V_TDTD["UI_showContinuePopup"] = std::bind(&PlayUI::showContinuePopup, this, _1, _2, _3, _4);
	myGD->V_V["UI_catchSubCumber"] = std::bind(&PlayUI::catchSubCumber, this);
	myGD->V_V["UI_takeItemCollect"] = std::bind(&PlayUI::takeItemCollect, this);
	myGD->V_V["UI_takeAddTimeItem"] = std::bind(&PlayUI::takeAddTimeItem, this);
	myGD->V_V["UI_stopCounting"] = std::bind(&PlayUI::stopCounting, this);
	myGD->V_V["UI_resumeCounting"] = std::bind(&PlayUI::resumeCounting, this);
	myGD->F_V["UI_getMapPercentage"] = std::bind(&PlayUI::getPercentage, this);
	myGD->I_V["UI_getComboCnt"] = std::bind(&PlayUI::getComboCnt, this);
	myGD->V_I["UI_setComboCnt"] = std::bind(&PlayUI::setComboCnt, this, _1);
	myGD->I_V["UI_getUseTime"] = std::bind(&PlayUI::getUseTime, this);
	myGD->V_V["UI_endFever"] = std::bind(&FeverParent::endFever, my_fp);
	myGD->V_V["UI_stopCombo"] = std::bind(&ComboParent::stopKeep, my_combo);
	myGD->B_V["UI_isExchanged"] = std::bind(&PlayUI::isExchanged, this);
	myGD->V_V["UI_addGameTime30Sec"] = std::bind(&PlayUI::addGameTime30Sec, this);
	myGD->V_V["UI_writeMap"] = std::bind(&PlayUI::writeMap, this);
	myGD->V_V["UI_checkMapTimeVector"] = std::bind(&PlayUI::checkMapTimeVector, this);
	myGD->V_V["UI_writeScore"] = std::bind(&PlayUI::writeScore, this);
	myGD->V_V["UI_checkScoreTimeVector"] = std::bind(&PlayUI::checkScoreTimeVector, this);
	myGD->V_V["UI_writeDie"] = std::bind(&PlayUI::writeDie, this);
	myGD->V_V["UI_writeImageChange"] = std::bind(&PlayUI::writeImageChange, this);
	myGD->V_I["UI_writeGameOver"] = std::bind(&PlayUI::writeGameOver, this, _1);
	myGD->V_V["UI_writeContinue"] = std::bind(&PlayUI::writeContinue, this);
}
bool PlayUI::isExchanged ()
{
	return is_exchanged;
}
void PlayUI::continueAction ()
{
	vector<SaveUserData_Key> save_userdata_list;
	
	save_userdata_list.push_back(kSaveUserData_Key_gold);
	
	myDSH->saveUserData(save_userdata_list, nullptr);
	
	
	if(countingCnt >= 130)
	{
		AudioEngine::sharedInstance()->stopEffect("sound_time_noti.mp3");
	}
	
	total_time += countingCnt;
	
	if(mySGD->isUsingItem(kIC_longTime))
	{
		countingCnt = -mySGD->getLongTimeValue();
	}
	else
	{
		countingCnt = 0;
	}
	
	jack_life = NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1)-1;//NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_life_i, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1)-1;
	for(int i=0;i<jack_life;i++)
	{
		CCSprite* jack_img = CCSprite::create("basic_character.png");
		jack_img->setPosition(ccp(i*20, 0));
//		if(myGD->gamescreen_type == kGT_leftUI)			jack_img->setPosition(ccp(25, myDSH->ui_center_y-30-i*20));
//		else if(myGD->gamescreen_type == kGT_rightUI)		jack_img->setPosition(ccp(480-25,myDSH->ui_center_y-30-i*20));
//		else									jack_img->setPosition(ccp(80+i*20,myDSH->ui_top-35));
		jack_life_node->addChild(jack_img);
		
		jack_array->addObject(jack_img);
	}
	
	CCMoveTo* t_move = CCMoveTo::create(0.5f, ccp(20,myDSH->ui_top-60));
	jack_life_node->runAction(t_move);
	
	jack_life_hide_count = 0;
	
	
	writeContinue();
	
	(target_continue->*delegate_continue)();
}
void PlayUI::menuAction (CCObject * sender)
{
	AudioEngine::sharedInstance()->playEffect("sound_buttonClick_Low.mp3", false);
	int tag = ((CCNode*)sender)->getTag();
	if(tag == kMenuTagUI_home && !isGameover)
	{
		showPause();
	}
	else if(tag == kMenuTagUI_condition && !isGameover)
	{
		showCondition();
	}
}
void PlayUI::showCondition ()
{
	if(is_show_condition)
	{
		((ConditionPopup*)getChildByTag(kCT_UI_clrCdtPopup))->holdingPopup();
	}
	else
	{
		is_show_condition = true;
		ConditionPopup* t_cdt = ConditionPopup::create(this, callfunc_selector(PlayUI::closeCondition));
		addChild(t_cdt, 0, kCT_UI_clrCdtPopup);
	}
}
void PlayUI::closeCondition ()
{
	is_show_condition = false;
}
void PlayUI::closeShutter ()
{
	mySGD->is_paused = false;
	AudioEngine::sharedInstance()->setAppFore();
	CCDirector::sharedDirector()->resume();
	
	endCloseShutter();
}
void PlayUI::endCloseShutter ()
{
	myGD->communication("Main_gameover");
}
//void PlayUI::goHome ()
//{
//	myLog->addLog(kLOG_getCoin_i, -1, mySGD->getStageGold());
//	
//	myLog->sendLog(CCString::createWithFormat("home_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStage))->getCString());
//	AudioEngine::sharedInstance()->stopAllEffects();
//	AudioEngine::sharedInstance()->stopSound();
//	myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_fail);
//	closeShutter();
//}
//void PlayUI::goReplay ()
//{
//	myDSH->setIntegerForKey(kDSH_Key_achieve_seqNoFailCnt, 0);
//	myLog->addLog(kLOG_getCoin_i, -1, mySGD->getStageGold());
//	
//	myLog->sendLog(CCString::createWithFormat("replay_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStage))->getCString());
//	AudioEngine::sharedInstance()->stopAllEffects();
//	AudioEngine::sharedInstance()->stopSound();
//	
//	mySGD->is_paused = false;
//	AudioEngine::sharedInstance()->setAppFore();
//	CCDirector::sharedDirector()->resume();
//	mySGD->gameOver(0, 0, 0);
//	mySGD->resetLabels();
//	myGD->resetGameData();
//	
//	myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stageSetting);
////	CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
//	CCDirector::sharedDirector()->replaceScene(StartSettingScene::scene());
//}
//void PlayUI::cancelHome ()
//{
//	(target_main->*delegate_startControl)();
//	mySGD->is_paused = false;
//	AudioEngine::sharedInstance()->setAppFore();
//	CCDirector::sharedDirector()->resume();
//}
void PlayUI::alertAction (int t1, int t2)
{
	
}
#undef LZZ_INLINE
