//
//  FailPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 19..
//
//

#include "FailPopup.h"
#include "StarGoldData.h"
#include "EnumDefine.h"
#include "FailHelpPopup.h"
#include "LogData.h"
#include "CountingBMLabel.h"
#include "DownloadFile.h"
#include "StageSettingPopup.h"
#include "PuzzleMapScene.h"
#include "KSUtil.h"
#include "KSAlertView.h"
#include "ShopPopup.h"
#include "ChallengeSend.h"
#include "HelpResultSend.h"
#include "SendMessageUtil.h"
#include "UnknownFriends.h"
#include "StartSettingScene.h"
#include "ASPopupView.h"
#include "CardAnimations.h"
#include "TouchSuctionLayer.h"
#include "KHAlertView.h"
#include "CommonButton.h"
#include "CardCase.h"

#include "HeartTime.h"
#include "MyLocalization.h"
#include "GDWebSprite.h"

typedef enum tMenuTagFailPopup{
	kMT_FP_main = 1,
	kMT_FP_replay,
}MenuTagFailPopup;

typedef enum tZorderFailPopup{
	kZ_FP_gray = 1,
	kZ_FP_back,
	kZ_FP_img,
	kZ_FP_table,
	kZ_FP_menu,
	kZ_FP_popup
}ZorderFailPopup;

void FailPopup::setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool FailPopup::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	AudioEngine::sharedInstance()->preloadEffectScene("Ending");
	
	is_menu_enable = false;
	is_loaded_list = false;
	is_end_popup_animation = false;
	
	myLog->addLog(kLOG_getCoin_i, -1, mySGD->getStageGold());
	myLog->addLog(kLOG_remainHeart_i, -1, myDSH->getIntegerForKey(kDSH_Key_heartCnt));
	
	Json::Value param;
	param["key"] = CCSTR_CWF("stage_over_%d", mySD->getSilType())->getCString();
	
	hspConnector::get()->command("increaseStats", param, NULL);
	
	myLog->sendLog(CCString::createWithFormat("fail_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)))->getCString());
	
	if(mySGD->getScore() > myDSH->getIntegerForKey(kDSH_Key_allHighScore))
	{
		myDSH->setIntegerForKey(kDSH_Key_allHighScore, int(mySGD->getScore()));
	}
	Json::Value p1;
	p1["memberID"] = hspConnector::get()->getKakaoID();
	p1["score"] = int(mySGD->getScore());
	Json::Value p1_data;
	p1_data["selectedcard"] = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	p1_data["allhighscore"] = mySGD->getScore();//myDSH->getIntegerForKey(kDSH_Key_allHighScore);
	p1_data["highstage"] = mySGD->suitable_stage;
	Json::FastWriter p1_data_writer;
	p1["data"] = p1_data_writer.write(p1_data);
	hspConnector::get()->command("addweeklyscore", p1, NULL);
	
	
	if(mySGD->save_stage_rank_stageNumber == mySD->getSilType())
	{
		string my_id = hspConnector::get()->getKakaoID();
		
		vector<RankFriendInfo>::iterator iter = find(mySGD->save_stage_rank_list.begin(), mySGD->save_stage_rank_list.end(), my_id);
		if(iter != mySGD->save_stage_rank_list.end())
		{
			if((*iter).score < mySGD->getScore())
			{
				(*iter).score = mySGD->getScore();
				struct t_FriendSort{
					bool operator() (const RankFriendInfo& a, const RankFriendInfo& b)
					{
						return a.score > b.score;
					}
				} pred;
				
				sort(mySGD->save_stage_rank_list.begin(), mySGD->save_stage_rank_list.end(), pred);
				
				for(int i=0;i<mySGD->save_stage_rank_list.size();i++)
					mySGD->save_stage_rank_list[i].rank = i+1;
			}
		}
		else
		{
			Json::Value my_kakao = hspConnector::get()->myKakaoInfo;
			
			RankFriendInfo fInfo;
			fInfo.nickname = my_kakao["nickname"].asString();
			fInfo.img_url = my_kakao["profile_image_url"].asString();
			fInfo.user_id = my_kakao["user_id"].asString();
			fInfo.score = mySGD->getScore();
			fInfo.is_play = true;
			mySGD->save_stage_rank_list.push_back(fInfo);
			
			struct t_FriendSort{
				bool operator() (const RankFriendInfo& a, const RankFriendInfo& b)
				{
					return a.score > b.score;
				}
			} pred;
			
			sort(mySGD->save_stage_rank_list.begin(), mySGD->save_stage_rank_list.end(), pred);
			
			for(int i=0;i<mySGD->save_stage_rank_list.size();i++)
				mySGD->save_stage_rank_list[i].rank = i+1;
		}
		
		friend_list = mySGD->save_stage_rank_list;
		is_loaded_list = true;
	}
	
	
	Json::Value p;
	p["memberID"]=hspConnector::get()->getKakaoID();
	p["score"]=int(mySGD->getScore());
	p["stageNo"]=mySD->getSilType();
	hspConnector::get()->command("setStageScore",p,NULL);
	
	
//	int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
//	
//	if(selected_card_number > 0)
//	{
//		if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number) <= 0)
//		{
//			// 소멸
//			
//			CCSprite* card = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png",selected_card_number)->getCString());
//			CardCase* cardCase = CardCase::create(selected_card_number);
//			card->addChild(cardCase);
//			
//			RemoveCardAnimation* b = RemoveCardAnimation::create(card,-210);
//			
//			b->setSkipFunc([this](){
//				CCLog("skip Func");
//			});
//			b->setRepairFunc([=](){
//				CCLog("repair Func");
//				
//				if(mySGD->getStar() >= mySGD->getCardDurabilityUpFee())
//				{
//					mySGD->setStar(mySGD->getStar() - mySGD->getCardDurabilityUpFee());
//					myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number, myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, selected_card_number));
//					
//					myDSH->saveUserData({kSaveUserData_Key_star}, nullptr);
//					
//					//복구하기
//					b->repair();
//				}
//				else
//				{
//					b->skip();
//				}
//			});
//			b->setCloseFunc([=](){
//				CCLog("close Func");
//				if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number) <= 0)
//					myDSH->setIntegerForKey(kDSH_Key_selectedCard, 0);
//				this->endDecreaseCardDuration();
//			});
//			
//			b->start();
//			addChild(b, kZ_FP_popup);
//		}
//		else
//		{
//			// 내구 하락
//			int cardNo = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
//			
//			CCSprite* card = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png",cardNo)->getCString());
//			CardCase* cardCase = CardCase::create(cardNo);
//			card->addChild(cardCase);
//			
//			DownCardAnimation* b = DownCardAnimation::create(card,-210);
//			b->setCloseFunc([this](){
//				this->endDecreaseCardDuration();
//			});
//			b->start();
//			addChild(b, kZ_FP_popup);
//		}
//	}
//	else
//	{
		this->endDecreaseCardDuration();
//	}
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kZ_FP_gray);
	
	main_case = CCSprite::create("ending_back.png");
	main_case->setAnchorPoint(ccp(0.5f,0.5f));
	main_case->setPosition(ccp(240,160-450));
	addChild(main_case, kZ_FP_back);
	
	
	if(mySGD->is_before_selected_event_stage)
	{
		int stage_number = mySD->getSilType();
		
		CCLabelTTF* stage_number_label = CCLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(),	mySGD->getFont().c_str(), 10);
		stage_number_label->setPosition(ccp(60, main_case->getContentSize().height+40-68));
		main_case->addChild(stage_number_label);
		
		mySGD->is_before_selected_event_stage = false;
	}
	else
	{
		int stage_number = mySD->getSilType();
		int puzzle_number = NSDS_GI(stage_number, kSDS_SI_puzzle_i);
		int piece_number = NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number);
		
		CCLabelTTF* piece_number_label = CCLabelTTF::create(CCString::createWithFormat("%d-%d", puzzle_number, piece_number)->getCString(),	mySGD->getFont().c_str(), 10);
		piece_number_label->setPosition(ccp(60, main_case->getContentSize().height+40-68));
		main_case->addChild(piece_number_label);
	}
	
	FailCode fail_code = mySGD->fail_code;
	
	if(fail_code == kFC_timeover)
	{
		CCSprite* title = CCSprite::create("ending_timeover.png");
		title->setPosition(ccp(131,230.5f));
		main_case->addChild(title, kZ_FP_img);
	}
	else if(fail_code == kFC_missionfail)
	{
		CCSprite* title = CCSprite::create("ending_missionfail.png");
		title->setPosition(ccp(131,230.5f));
		main_case->addChild(title, kZ_FP_img);
		
		//		CLEAR_CONDITION clr_cdt = mySD->getClearCondition();
		//
		//		CCSprite* fail_condition = CCSprite::create(CCString::createWithFormat("condition%d_menu.png", clr_cdt)->getCString());
		//		fail_condition->setPosition(ccp(240,170));
		//		addChild(fail_condition, kZ_FP_img);
		//
		//		if(clr_cdt == kCLEAR_subCumberCatch)
		//		{
		//			int catch_cnt = mySD->getClearConditionCatchSubCumber();
		//
		//			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", catch_cnt)->getCString(), mySGD->getFont().c_str(), 12);
		//			clr_cdt_label->setPosition(ccp(240,165));
		//			addChild(clr_cdt_label, kZ_FP_img);
		//		}
		//		else if(clr_cdt == kCLEAR_bigArea)
		//		{
		//			float clr_cdt_per = mySD->getClearConditionBigAreaPer();
		//			int clr_cdt_cnt = mySD->getClearConditionBigAreaCnt();
		//
		//			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%2.0f%%:%d", clr_cdt_per*100.f, clr_cdt_cnt)->getCString(), mySGD->getFont().c_str(), 12);
		//			clr_cdt_label->setPosition(ccp(240,165));
		//			addChild(clr_cdt_label, kZ_FP_img);
		//		}
		//		else if(clr_cdt == kCLEAR_itemCollect)
		//		{
		//			int collect_cnt = mySD->getClearConditionItemCollect();
		//
		//			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", collect_cnt)->getCString(), mySGD->getFont().c_str(), 12);
		//			clr_cdt_label->setPosition(ccp(240,165));
		//			addChild(clr_cdt_label, kZ_FP_img);
		//		}
		//		else if(clr_cdt == kCLEAR_perfect)
		//		{
		//			float perfect_per = mySD->getClearConditionPerfectBase();
		//
		//			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%.0f%%", perfect_per*100.f)->getCString(), mySGD->getFont().c_str(), 12);
		//			clr_cdt_label->setPosition(ccp(240,165));
		//			addChild(clr_cdt_label, kZ_FP_img);
		//		}
		//		else if(clr_cdt == kCLEAR_sequenceChange)
		//		{
		//			CCSprite* c_coin = CCSprite::create("exchange_1_act.png");
		//			c_coin->setPosition(ccp(240,165));
		//			addChild(c_coin, kZ_FP_img);
		//		}
		//		else if(clr_cdt == kCLEAR_timeLimit)
		//		{
		//			int clr_time = mySD->getClearConditionTimeLimit();
		//
		//			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", clr_time)->getCString(), mySGD->getFont().c_str(), 12);
		//			clr_cdt_label->setPosition(ccp(240,165));
		//			addChild(clr_cdt_label, kZ_FP_img);
		//		}
	}
	else
	{
		CCSprite* title = CCSprite::create("ending_gameover.png");
		title->setPosition(ccp(131,230.5f));
		main_case->addChild(title, kZ_FP_img);
	}
	
	score_label = CCLabelTTF::create("0", mySGD->getFont().c_str(), 18); // CCLabelBMFont::create("0", "mb_white_font.fnt");
	score_label->setColor(ccc3(220, 190, 125));
	score_label->setAnchorPoint(ccp(1.f,0.5));
	score_label->setPosition(ccp(230,78));
	main_case->addChild(score_label, kZ_FP_img);
	
	gold_label = CCLabelTTF::create("0", mySGD->getFont().c_str(), 18); // CCLabelBMFont::create("0", "mb_white_font.fnt");
	gold_label->setColor(ccc3(220, 190, 125));
	gold_label->setAnchorPoint(ccp(1.f,0.5));
	gold_label->setPosition(ccp(230,106));
	main_case->addChild(gold_label, kZ_FP_img);
	
	time_label = CCLabelTTF::create("0", mySGD->getFont().c_str(), 18); // CCLabelBMFont::create("0", "mb_white_font.fnt");
	time_label->setColor(ccc3(220, 190, 125));
	time_label->setAnchorPoint(ccp(1.f,0.5));
	time_label->setPosition(ccp(230,132));
	main_case->addChild(time_label, kZ_FP_img);
	
	
	string main_filename;
	
	if(mySGD->getIsAcceptHelp())
	{
		main_filename = "ending_main.png";
	}
	else if(mySGD->getIsMeChallenge())
	{
		main_filename = "ending_fail.png";
	}
	else if(mySGD->getIsAcceptChallenge())
	{
		main_filename = "ending_fail.png";
	}
	else
	{
		main_filename = "ending_main.png";
	}
	
	
	CCSprite* n_main = CCSprite::create(main_filename.c_str());
	CCSprite* s_main = CCSprite::create(main_filename.c_str());
	s_main->setColor(ccGRAY);
	
	CCMenuItem* main_item = CCMenuItemSprite::create(n_main, s_main, this, menu_selector(FailPopup::menuAction));
	main_item->setTag(kMT_FP_main);
	
	main_menu = CCMenu::createWithItem(main_item);
	main_menu->setVisible(false);
	main_menu->setPosition(ccp(348.5f,38));
	main_case->addChild(main_menu, kZ_FP_menu);
	main_menu->setTouchPriority(-190);
	
	
	if(!mySGD->getIsMeChallenge() && !mySGD->getIsAcceptChallenge() && !mySGD->getIsAcceptHelp())
	{
		CCSprite* n_replay = CCSprite::create("ending_replay2.png");
		CCSprite* s_replay = CCSprite::create("ending_replay2.png");
		s_replay->setColor(ccGRAY);
		
		CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(FailPopup::menuAction));
		replay_item->setTag(kMT_FP_replay);
		
		replay_menu = CCMenu::createWithItem(replay_item);
		replay_menu->setVisible(false);
		replay_menu->setPosition(ccp(130,38));
		main_case->addChild(replay_menu, kZ_FP_menu);
		replay_menu->setTouchPriority(-190);
	}
	else
	{
		if(mySGD->getIsMeChallenge() || mySGD->getIsAcceptChallenge())
		{
			CCSprite* n_replay = CCSprite::create("ending_replay2.png");
			CCSprite* s_replay = CCSprite::create("ending_replay2.png");
			s_replay->setColor(ccGRAY);
			
			CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(FailPopup::menuAction));
			replay_item->setTag(kMT_FP_replay);
			
			replay_menu = CCMenu::createWithItem(replay_item);
			replay_menu->setVisible(false);
			replay_menu->setPosition(ccp(130,38));
			main_case->addChild(replay_menu, kZ_FP_menu);
			replay_menu->setTouchPriority(-190);
		}
		else
			replay_menu = NULL;
	}
	
	
	
	
	is_saved_user_data = false;
	
	myDSH->saveAllUserData(json_selector(this, FailPopup::resultSavedUserData));
	
	return true;
}

void FailPopup::endDecreaseCardDuration()
{
	if(mySGD->getIsMeChallenge())
	{
		hspConnector* t_hsp = hspConnector::get();
		FightResultAnimation* b = FightResultAnimation::create(t_hsp->getKakaoProfileURL(),mySGD->getScore(),t_hsp->getKakaoNickname(),
															   mySGD->getMeChallengeTargetProfile(),mySGD->getMeChallengeTargetScore(),mySGD->getMeChallengeTargetNick(),
															   -210);
		b->setCancelFunc([](){
			CCLog("닫기눌렀을때");
		});
		b->setCloseFunc([=](){
			CCLog("창 다닫혔을때");
			closePopup();
		});
		
		addChild(b,kZ_FP_popup);
		
		b->setConfirmFunc([](){
			CCLog("패배시 확인눌렀을때");
		});
		
		b->startLose(); //내가졌을때
	}
	else if(mySGD->getIsAcceptChallenge())
	{
		mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_playIndex)] = 0;
		
		hspConnector* t_hsp = hspConnector::get();
		FightResultAnimation* b = FightResultAnimation::create(t_hsp->getKakaoProfileURL(),mySGD->getScore(),t_hsp->getKakaoNickname(),
															   mySGD->getAcceptChallengeProfile(),mySGD->getAcceptChallengeScore(),mySGD->getAcceptChallengeNick(),
															   -210);
		b->setCancelFunc([](){
			CCLog("닫기눌렀을때");
		});
		b->setCloseFunc([=](){
			CCLog("창 다닫혔을때");
			closePopup();
		});
		
		addChild(b,kZ_FP_popup);
		
		b->setConfirmFunc([](){
			CCLog("패배시 확인눌렀을때");
		});
		
		b->startLose(); //내가졌을때
	}
	else if(mySGD->getIsAcceptHelp())
	{
		/// ksks
		addChild(HelpResultSend::create(mySGD->getAcceptHelpId(), false, [=](){closePopup();}), kZ_FP_popup);
	}
	else
	{
		closePopup();
	}
}

void FailPopup::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
	startCalcAnimation();
}

void FailPopup::showPopup()
{
	myDSH->setIntegerForKey(kDSH_Key_achieve_seqNoFailCnt, 0);
	gray->setOpacity(255);
//	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
//	gray->runAction(gray_fade);

	main_case->setPosition(ccp(240,160));
	endShowPopup();
//	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,0));
//	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(FailPopup::endShowPopup));
//	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
//	main_case->runAction(main_seq);
}

void FailPopup::endShowPopup()
{
	
}

void FailPopup::hidePopup()
{
	is_menu_enable = false;
	rankTableView->setTouchEnabled(false);
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160-450));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(FailPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void FailPopup::endHidePopup()
{
	AudioEngine::sharedInstance()->unloadEffectScene("Ending");
	
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void FailPopup::startCalcAnimation()
{
	AudioEngine::sharedInstance()->playEffect("sound_calc.mp3", true);
	startScoreAnimation();
}

void FailPopup::startScoreAnimation()
{
	keep_score = mySGD->getScore();
	decrease_score = keep_score;
	increase_score = 0.f;
	schedule(schedule_selector(FailPopup::scoreAnimation));
}

void FailPopup::closePopup()
{
	is_end_popup_animation = true;
	
	if(is_end_popup_animation && is_saved_user_data && is_loaded_list)
	{
		main_menu->setVisible(true);
		if(myDSH->getIntegerForKey(kDSH_Key_heartCnt) > 0)
		{
			if(replay_menu)
				replay_menu->setVisible(true);
		}
		is_menu_enable = true;
	}
}

void FailPopup::scoreAnimation(float dt)
{
	if(decrease_score > 0)
	{
		int decreaseUnit = keep_score / 1.f * dt;
		
		if(decrease_score < decreaseUnit)
		{
			increase_score += decrease_score;
			decrease_score = 0;
		}
		else {
			if(decreaseUnit <= 0)
			{
				increase_score += decrease_score;
				decrease_score = 0;
			}
			else {
				decrease_score -= decreaseUnit;
				increase_score += decreaseUnit;
			}
		}
		score_label->setString(CCString::createWithFormat("%.0f",increase_score)->getCString());
	}
	else
		stopScoreAnimation();
}

void FailPopup::stopScoreAnimation()
{
	unschedule(schedule_selector(FailPopup::scoreAnimation));
	score_label->setString(CCString::createWithFormat("%.0f", mySGD->getScore())->getCString());
	startGoldAnimation();
}

void FailPopup::startGoldAnimation()
{
	keep_gold = mySGD->getStageGold();
	decrease_gold = keep_gold;
	increase_gold = 0.f;
	schedule(schedule_selector(FailPopup::goldAnimation));
}

void FailPopup::goldAnimation(float dt)
{
	if(decrease_gold > 0)
	{
		int decreaseUnit = keep_gold / 0.5f * dt;
		
		if(decrease_gold < decreaseUnit)
		{
			increase_gold += decrease_gold;
			decrease_gold = 0;
		}
		else {
			if(decreaseUnit <= 0)
			{
				increase_gold += decrease_gold;
				decrease_gold = 0;
			}
			else {
				decrease_gold -= decreaseUnit;
				increase_gold	+= decreaseUnit;
			}
		}
		gold_label->setString(CCString::createWithFormat("%.0f",increase_gold)->getCString());
	}
	else
		stopGoldAnimation();
}

void FailPopup::stopGoldAnimation()
{
	unschedule(schedule_selector(FailPopup::goldAnimation));
	gold_label->setString(CCString::createWithFormat("%d", mySGD->getStageGold())->getCString());
	startTimeAnimation();
}

void FailPopup::startTimeAnimation()
{
	keep_time = mySGD->getGameTime();
	decrease_time = keep_time;
	increase_time = 0.f;
	schedule(schedule_selector(FailPopup::timeAnimation));
}

void FailPopup::timeAnimation(float dt)
{
	if(decrease_time > 0)
	{
		int decreaseUnit = keep_time / 0.5f * dt;
		
		if(decrease_time < decreaseUnit)
		{
			increase_time += decrease_time;
			decrease_time = 0;
		}
		else {
			if(decreaseUnit <= 0)
			{
				increase_time += decrease_time;
				decrease_time = 0;
			}
			else {
				decrease_time -= decreaseUnit;
				increase_time	+= decreaseUnit;
			}
		}
		time_label->setString(CCString::createWithFormat("%.0f",increase_time)->getCString());
	}
	else
		stopTimeAnimation();
}

void FailPopup::stopTimeAnimation()
{
	unschedule(schedule_selector(FailPopup::timeAnimation));
	time_label->setString(CCString::createWithFormat("%d", mySGD->getGameTime())->getCString());
	AudioEngine::sharedInstance()->stopAllEffects();
}

void FailPopup::resultSavedUserData(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		is_saved_user_data = true;
		endLoad();
		
		if(is_loaded_list)
		{
			rankTableView = CCTableView::create(this, CCSizeMake(208, 199));
			
			rankTableView->setAnchorPoint(CCPointZero);
			rankTableView->setDirection(kCCScrollViewDirectionVertical);
			rankTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
			rankTableView->setPosition(ccp(243, 59.5f));
			
			rankTableView->setDelegate(this);
			main_case->addChild(rankTableView, kZ_FP_table);
			rankTableView->setTouchPriority(kCCMenuHandlerPriority+1);
		}
		else
		{
			resultLoadFriends(Json::Value());
		}
//		hspConnector::get()->kLoadFriends(json_selector(this, FailPopup::resultLoadFriends));
	}
	else
	{
		myDSH->saveAllUserData(json_selector(this, FailPopup::resultSavedUserData));
	}
}

void FailPopup::resultLoadFriends(Json::Value result_data)
{
//	CCLog("resultLoadFriends : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
//	if(result_data["status"].asInt() == 0)
//	{
//		Json::Value appfriends = result_data["app_friends_info"];
//		appfriends.append(hspConnector::get()->myKakaoInfo);
//		
		Json::Value p;
//		for(int i=0; i<appfriends.size();i++)
//		{
//			FailFriendRank t_friend_info;
//			t_friend_info.nickname = appfriends[i]["nickname"].asString().c_str();
//			t_friend_info.img_url = appfriends[i]["profile_image_url"].asString().c_str();
//			t_friend_info.user_id = appfriends[i]["user_id"].asString().c_str();
//			t_friend_info.score = 0;
//			t_friend_info.is_play = false;
//			t_friend_info.is_message_blocked = appfriends[i]["message_blocked"].asBool();
//			friend_list.push_back(t_friend_info);
//			
//			p["memberIDList"].append(appfriends[i]["user_id"].asString());
//		}

	Json::Value my_kakao = hspConnector::get()->myKakaoInfo;
	
	RankFriendInfo fInfo;
	fInfo.nickname = my_kakao["nickname"].asString();
	fInfo.img_url = my_kakao["profile_image_url"].asString();
	fInfo.user_id = my_kakao["user_id"].asString();
	fInfo.score = 0;
	fInfo.is_play = false;
	friend_list.push_back(fInfo);
	
	p["memberIDList"].append(my_kakao["user_id"].asString());
	
	for(auto i : KnownFriends::getInstance()->getFriends())
	{
		RankFriendInfo fInfo;
		fInfo.nickname = i.nick;
		fInfo.img_url = i.profileUrl;
		fInfo.user_id = i.userId;
		fInfo.score = 0;
		fInfo.is_play = false;
		fInfo.is_message_blocked = i.messageBlocked;
		friend_list.push_back(fInfo);
		
		p["memberIDList"].append(i.userId);
	}
	for(auto i : UnknownFriends::getInstance()->getFriends())
	{
		RankFriendInfo fInfo;
		fInfo.nickname = i.nick;
		fInfo.img_url = "";
		fInfo.user_id = i.userId;
		fInfo.score = 0;
		fInfo.is_play = false;
		fInfo.is_message_blocked = i.messageBlocked;
		friend_list.push_back(fInfo);
		
		p["memberIDList"].append(i.userId);
	}
	
	
	p["stageNo"]=mySD->getSilType();
	hspConnector::get()->command("getstagescorelist",p,json_selector(this, FailPopup::resultGetStageScoreList));
//	}
//	else
//	{
//		is_loaded_list = true;
//		endLoad();
//	}
}

void FailPopup::resultGetStageScoreList(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		Json::Value score_list = result_data["list"];
		for(int i=0;i<score_list.size();i++)
		{
			if(score_list[i]["memberID"].asString() == hspConnector::get()->getKakaoID())
			{
				if(mySGD->getScore() > score_list[i]["score"].asFloat())
				{
					CCSprite* high_score_img = CCSprite::create("ending_highscore.png");
					high_score_img->setPosition(ccp(105, 78));
					main_case->addChild(high_score_img, kZ_FP_img);
				}
			}
			
			vector<RankFriendInfo>::iterator iter = find(friend_list.begin(), friend_list.end(), score_list[i]["memberID"].asString().c_str());
			if(iter != friend_list.end())
			{
				(*iter).score = score_list[i]["score"].asFloat();
				(*iter).is_play = true;
			}
			else
				CCLog("not found friend memberID");
		}
		
		auto beginIter = std::remove_if(friend_list.begin(), friend_list.end(), [=](RankFriendInfo t_info)
																		{
																			return !t_info.is_play;
																		});
		friend_list.erase(beginIter, friend_list.end());
		
		struct t_FriendSort{
			bool operator() (const RankFriendInfo& a, const RankFriendInfo& b)
			{
				return a.score > b.score;
			}
		} pred;
		
		sort(friend_list.begin(), friend_list.end(), pred);
		
		for(int i=0;i<friend_list.size();i++)
			friend_list[i].rank = i+1;
		
		// create cell
		
//		CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 208, 199));
//		temp_back->setAnchorPoint(CCPointZero);
//		temp_back->setOpacity(100);
//		temp_back->setPosition(ccp(243, 62.5f));
//		main_case->addChild(temp_back, kZ_FP_menu);
		
		rankTableView = CCTableView::create(this, CCSizeMake(208, 199));
		
		rankTableView->setAnchorPoint(CCPointZero);
		rankTableView->setDirection(kCCScrollViewDirectionVertical);
		rankTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
		rankTableView->setPosition(ccp(243, 59.5f));
		
		rankTableView->setDelegate(this);
		main_case->addChild(rankTableView, kZ_FP_table);
		rankTableView->setTouchPriority(-190);
		
		//		int myPosition = rankTableView->minContainerOffset().y;
		//		for(int i=0; i<friend_list.size(); i++)
		//		{
		//			if(friend_list[i].user_id == hspConnector::get()->getKakaoID())
		//			{
		//				myPosition = friend_list.size() - i - 1;
		//				break;
		//			}
		//		}
		//		float yInitPosition = MAX(rankTableView->minContainerOffset().y, -cellSizeForTable(rankTableView).height*myPosition + rankTableView->getViewSize().height / 2.f);
		//		yInitPosition = MIN(0, yInitPosition);
		//		rankTableView->setContentOffsetInDuration(ccp(0, yInitPosition), 0.7f);
		
		if(suction)
		{
			suction->removeFromParent();
			suction = NULL;
		}
		
		suction = TouchSuctionLayer::create(-190+1);
		suction->setNotSwallowRect(CCRectMake(243, 59.5f, 208, 199));
		suction->setTouchEnabled(true);
		main_case->addChild(suction);
		
	}
	is_loaded_list = true;
	endLoad();
}

void FailPopup::endLoad()
{
	if(is_end_popup_animation && is_saved_user_data && is_loaded_list)
	{
		main_menu->setVisible(true);
		if(myDSH->getIntegerForKey(kDSH_Key_heartCnt) > 0)
		{
			if(replay_menu)
				replay_menu->setVisible(true);
		}
		is_menu_enable = true;
	}
}

CCPoint FailPopup::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kMT_FP_main)			return_value = ccp(100,67);
	else if(t_tag == kMT_FP_replay)		return_value = ccp(240,67);
	//	else if(t_tag == kMT_FP_help)		return_value = ccp(380,70);
	
	return return_value;
}

void FailPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	int tag = ((CCNode*)pSender)->getTag();
	
	is_menu_enable = false;
	
	if(tag == kMT_FP_main)
	{
		mySGD->setIsMeChallenge(false);
		mySGD->setIsAcceptChallenge(false);
		mySGD->setIsAcceptHelp(false);
		mySGD->selectFriendCard();
		AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
//		mySGD->resetLabels();
		hidePopup();
	}
	else if(tag == kMT_FP_replay)
	{
		AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
//		mySGD->resetLabels();
		
		if(mySD->getSilType() >= 10000)
			mySGD->is_before_selected_event_stage = true;
		
		CCDirector::sharedDirector()->replaceScene(StartSettingScene::scene());
//		StageSettingPopup* t_popup = StageSettingPopup::create();
//		t_popup->setHideFinalAction(target_final, delegate_final);
//		getParent()->addChild(t_popup, kPMS_Z_popup);
//		
//		target_final = NULL;
//		hidePopup();
	}
}

void FailPopup::cellAction( CCObject* sender )
{
	//	int tag = ((CCNode*)sender)->getTag();
	//	tag -= kFFC_T_menuBase;
	int tag = (int)((CCNode*)sender)->getUserData();
	CCLog("help memberID : %s", friend_list[tag].user_id.c_str());
	//김현수 89987036974054944
	std::string user_id = friend_list[tag].user_id;
	KS::KSLog("% %" , friend_list[tag].nickname, friend_list[tag].user_id);

	KHAlertView* av = KHAlertView::create(); 
	av->setTitleFileName("msg_help_request.png");
	av->setCloseButton(CCMenuItemImageLambda::create("cardchange_cancel.png", "cardchange_cancel.png",
																									 [=](CCObject*){
																									 }
																									));
	av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
	av->setWidth(240);
	av->setHeight(240);
	av->setTitleHeight(10);
	av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
	av->setCenterY(150);

	CCNode* emptyNode = CCNode::create();
	// friend_list[tag].nickname // 받을 친구의 닉네임.
	auto ttf = CCLabelTTF::create("도움을 요청합니다.", mySGD->getFont().c_str(), 14.f); 
	ttf->setHorizontalAlignment(kCCTextAlignmentCenter);
	//	con->setAnchorPoint(ccp(0, 0));
	//ttf->setAnchorPoint(ccp(0.5f, 0.5f));
	ttf->setColor(ccc3(255, 255, 255));
	ttf->setPosition(ccp(av->getContentRect().size.width / 2.f, -77));
	emptyNode->addChild(ttf);
	av->setContentNode(
			emptyNode
			);
	av->setContentSize(ttf->getDimensions());
	av->addButton(CommonButton::create("보내기", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
								[=](CCObject* e) {
									CCLog("ok!!");
									Json::Value p;
									Json::Value contentJson;

									contentJson["msg"] = (friend_list[tag].nickname + "님~ 못깨겠다. 좀 도와도...");
									contentJson["helpstage"] = mySD->getSilType();
									contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();

									KS::KSLog("%", hspConnector::get()->myKakaoInfo);
									//				 contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
									p["content"] = GraphDogLib::JsonObjectToString(contentJson);
									std::string recvId = user_id;
									recvId.erase(std::remove(recvId.begin(), recvId.end(), '-'), recvId.end()); // '-' ¡¶∞≈
									p["receiverMemberID"] = recvId;
									p["senderMemberID"] = hspConnector::get()->getKakaoID();
									p["type"] = kHelpRequest;
									hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
																							 {
																								 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
																								 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
																								 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];

																								 //																		setHelpSendTime(recvId);
																								 if(r["result"]["code"].asInt() != GDSUCCESS)
										return;

									setHelpSendTime(user_id);
									friend_list.erase(friend_list.begin() + tag);
									rankTableView->reloadData();
									
									 KHAlertView* av = KHAlertView::create(); 
						 // av->setTitleFileName("msg_challenge.png");
						 av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
						 av->setWidth(240);
						 av->setHeight(240);
						 av->setTitleHeight(10);
						 av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
						 av->setCenterY(150);

						 CCNode* emptyNode = CCNode::create();
						 auto ttf = CCLabelTTF::create("정상적으로 도움요청이 이루어졌습니다.\n친구분이 도와주길 기다리세요.^^", mySGD->getFont().c_str(), 12.f); 
						 ttf->setHorizontalAlignment(kCCTextAlignmentCenter);
						 //	con->setAnchorPoint(ccp(0, 0));
						 //ttf->setAnchorPoint(ccp(0.5f, 0.5f));
						 ttf->setColor(ccc3(255, 255, 255));
						 ttf->setPosition(ccp(av->getContentRect().size.width / 2.f, -77));
						 emptyNode->addChild(ttf);
						 av->setContentNode(
								 emptyNode
								 );
						 av->setContentSize(ttf->getDimensions());
						 av->addButton(CommonButton::create("ok", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
													 [=](CCObject* e) {
														 CCLog("ok!!");
													 });

						 addChild(av, kPMS_Z_helpRequest);
						 av->show();


						 //																		obj->removeFromParent();
						 Json::Value p2;
						 p2["receiver_id"] = recvId;
						 p2["message"] = "저를 도와주세요. 실력파인 당신이 저를 도와주시면 3대가 축복을 받으실거에요~^^";
						 hspConnector::get()->kSendMessage
							 (p2, [=](Json::Value r) {
								 GraphDogLib::JsonToLog("kSendMessage", r);
							 });
																							 });
								});
	

	addChild(av, kPMS_Z_helpRequest);
	av->show();
}

CCTableViewCell* FailPopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCLabelTTF* nickname_label;
	CCLabelTTF* score_label;
	RankFriendInfo* member = &friend_list[idx];
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CCSprite* profileImg = GDWebSprite::create((*member).img_url, "ending_noimg.png");
	profileImg->setAnchorPoint(ccp(0.5, 0.5));
	profileImg->setTag(kFFC_T_img);
	profileImg->setPosition(ccp(52, 22));
	cell->addChild(profileImg, -1);
	
	string my_id = hspConnector::get()->myKakaoInfo["user_id"].asString();
	string cell_id = (*member).user_id;
	
	//if(my_id != cell_id && KnownFriends::getInstance()->findById(cell_id) != nullptr)
	if(KnownFriends::getInstance()->findById(cell_id) != nullptr)
	{
		CCSprite* kakao_sign = CCSprite::create("puzzle_right_rank_kakao.png");
		kakao_sign->setPosition(ccp(41,29));
		cell->addChild(kakao_sign, kFFC_Z_img);
	}
	
	CCSprite* bg;
	if((*member).user_id == hspConnector::get()->getKakaoID())
	{
		bg = CCSprite::create("ending_cell_me.png");
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg,kFFC_Z_case);
		
		if(idx == 0)
		{
			CCSprite* rank_img = CCSprite::create("puzzle_right_rank_gold.png");
			rank_img->setPosition(ccp(21,21));
			rank_img->setTag(kFFC_T_rank);
			cell->addChild(rank_img, kFFC_Z_img);
		}
		else if(idx == 1)
		{
			CCSprite* rank_img = CCSprite::create("puzzle_right_rank_silver.png");
			rank_img->setPosition(ccp(21,21));
			rank_img->setTag(kFFC_T_rank);
			cell->addChild(rank_img, kFFC_Z_img);
		}
		else if(idx == 2)
		{
			CCSprite* rank_img = CCSprite::create("puzzle_right_rank_bronze.png");
			rank_img->setPosition(ccp(21,21));
			rank_img->setTag(kFFC_T_rank);
			cell->addChild(rank_img, kFFC_Z_img);
		}
		else
		{
			CCLabelTTF* rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", idx+1)->getCString(), mySGD->getFont().c_str(), 15);
			rank_label->setPosition(ccp(21,21));
			rank_label->setTag(kFFC_T_rank);
			cell->addChild(rank_label,kFFC_Z_img);
		}
	}
	else if(idx == 0)
	{
		bg = CCSprite::create("ending_cell_gold.png");
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg,kFFC_Z_case);
		
		CCSprite* rank_img = CCSprite::create("puzzle_right_rank_gold.png");
		rank_img->setPosition(ccp(21,21));
		rank_img->setTag(kFFC_T_rank);
		cell->addChild(rank_img, kFFC_Z_img);
	}
	else if(idx == 1)
	{
		bg = CCSprite::create("ending_cell_silver.png");
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg,kFFC_Z_case);
		
		CCSprite* rank_img = CCSprite::create("puzzle_right_rank_silver.png");
		rank_img->setPosition(ccp(21,21));
		rank_img->setTag(kFFC_T_rank);
		cell->addChild(rank_img, kFFC_Z_img);
	}
	else if(idx == 2)
	{
		bg = CCSprite::create("ending_cell_bronze.png");
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg,kFFC_Z_case);
		
		CCSprite* rank_img = CCSprite::create("puzzle_right_rank_bronze.png");
		rank_img->setPosition(ccp(21,21));
		rank_img->setTag(kFFC_T_rank);
		cell->addChild(rank_img, kFFC_Z_img);
	}
	else
	{
		bg = CCSprite::create("ending_cell_normal.png");
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg,kFFC_Z_case);
		
		CCLabelTTF* rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", idx+1)->getCString(), mySGD->getFont().c_str(), 15);
		rank_label->setPosition(ccp(21,21));
		rank_label->setTag(kFFC_T_rank);
		cell->addChild(rank_label,kFFC_Z_img);
	}
	
	nickname_label = CCLabelTTF::create((*member).nickname.c_str(), mySGD->getFont().c_str(), 12);
	nickname_label->enableStroke(ccBLACK, 0.5f);
	nickname_label->setPosition(ccp(114,28));
	nickname_label->setTag(kFFC_T_nickname);
	cell->addChild(nickname_label,kFFC_Z_img);
	
	score_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", (*member).score)->getCString(), mySGD->getFont().c_str(), 12);
	score_label->setColor(ccBLACK);
	score_label->setPosition(ccp(114,13));
	score_label->setTag(kFFC_T_score);
	cell->addChild(score_label,kFFC_Z_img);
	
	
	if((*member).user_id == hspConnector::get()->getKakaoID())
	{
		
	}
	else
	{
		if(!mySGD->getIsMeChallenge() && !mySGD->getIsAcceptChallenge() && !mySGD->getIsAcceptHelp())
		{
			if(!(*member).is_message_blocked)
			{
				int remain_time = ::getIsNotHelpableUser((*member).user_id.c_str(), mySGD->getHelpCoolTime());
				if(remain_time <= 0)
				{
					CCSprite* n_help = CCSprite::create("ending_help_on.png");
					CCSprite* s_help = CCSprite::create("ending_help_on.png");
					s_help->setColor(ccGRAY);
					
					CCMenuItem* help_item = CCMenuItemSprite::create(n_help, s_help, this, menu_selector(FailPopup::cellAction));
					help_item->setTag(kFFC_T_menuBase);
					help_item->setUserData((void*)idx);
					ScrollMenu* help_menu = ScrollMenu::create(help_item, NULL);
					help_menu->setPosition(ccp(180,21));
					cell->addChild(help_menu, kFFC_Z_img);
					help_menu->setTouchPriority(-190+2);
				}
				else
				{
					CCSprite* d_help = CCSprite::create("ending_help_on.png");
					d_help->setColor(ccGRAY);
					d_help->setPosition(ccp(180,21));
					cell->addChild(d_help, kFFC_Z_img);
					
					std::string remainStr = ::getRemainTimeMsg( remain_time );
					CCLabelTTF* remainFnt = CCLabelTTF::create(remainStr.c_str(), mySGD->getFont().c_str(), 12.f);
					remainFnt->setPosition(ccp(d_help->getContentSize().width/2.f, d_help->getContentSize().height/2.f));
					d_help->addChild(remainFnt);
				}
			}
		}
	}
	
	return cell;
}

void FailPopup::scrollViewDidScroll( CCScrollView* view )
{
	
}

void FailPopup::scrollViewDidZoom( CCScrollView* view )
{
	
}

void FailPopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{

	//		CCLog("%s", m_scoreList[cell->getIdx()]["user_id"].asString().c_str());
}

CCSize FailPopup::cellSizeForTable( CCTableView *table )
{
	return CCSizeMake(210, 45);
}

unsigned int FailPopup::numberOfCellsInTableView( CCTableView *table )
{
	return friend_list.size();
}
