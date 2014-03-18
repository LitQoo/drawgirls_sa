//
//  CardSettingPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 20..
//
//

#include "CardSettingPopup.h"
#include "MyLocalization.h"
#include "StageImgLoader.h"
//#include "CollectionBook.h"
#include "CollectionBookPopup.h"
#include "PuzzleMapScene.h"
#include "MainFlowScene.h"
#include "CardCase.h"
#include "GameData.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "CardListViewer.h"
#include "StageSettingPopup.h"
#include "CardStrengthPopup.h"
#include "TutorialFlowStep.h"
#include "TouchSuctionLayer.h"
#include "CommonButton.h"

void CardSettingPopup::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool CardSettingPopup::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	if(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber) > 10000)
	{
		server_puzzle_list_count = NSDS_GI(kSDS_GI_eventListCount_i);
		
		server_puzzle_list_no.push_back(0);
		server_puzzle_stage_count.push_back(0);
		server_puzzle_start_stage.push_back(0);
		for(int i=1;i<=server_puzzle_list_count;i++)
		{
			server_puzzle_list_no.push_back(NSDS_GI(kSDS_GI_eventList_int1_no_i, i));
			server_puzzle_stage_count.push_back(NSDS_GI(server_puzzle_list_no[i], kSDS_PZ_stageCount_i));
			server_puzzle_start_stage.push_back(NSDS_GI(server_puzzle_list_no[i], kSDS_PZ_startStage_i));
		}
	}
	else
	{
		server_puzzle_list_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
		
		server_puzzle_list_no.push_back(0);
		server_puzzle_stage_count.push_back(0);
		server_puzzle_start_stage.push_back(0);
		for(int i=1;i<=server_puzzle_list_count;i++)
		{
			server_puzzle_list_no.push_back(NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i));
			server_puzzle_stage_count.push_back(NSDS_GI(server_puzzle_list_no[i], kSDS_PZ_stageCount_i));
			server_puzzle_start_stage.push_back(NSDS_GI(server_puzzle_list_no[i], kSDS_PZ_startStage_i));
		}
	}
	
	
	recent_sort_type = myDSH->getIntegerForKey(kDSH_Key_cardSortType);
	recent_selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	
	is_menu_enable = false;
	
	setTouchEnabled(true);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kCSS_Z_gray);
	
	main_case = CCSprite::create("cardsetting_back.png");
	main_case->setAnchorPoint(ccp(0.5,0.5));
	main_case->setPosition(ccp(240,160-450));
	addChild(main_case, kCSS_Z_back);
	
	int puzzle_count = server_puzzle_list_count;
	int total_stage_cnt = 0;
	
	if(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber) > 10000)
	{
		for(int i=1;i<=puzzle_count;i++)
		{
			int puzzle_number = NSDS_GI(kSDS_GI_eventList_int1_no_i, i);
			int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
			
			total_stage_cnt += stage_count;
		}
	}
	else
	{
		for(int i=1;i<=puzzle_count;i++)
		{
			int puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i);
			int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
			
			total_stage_cnt += stage_count;
		}
	}
	
	default_align_number_of_cell = (total_stage_cnt-1)/2 + 1;
	
	CCSize table_size = CCSizeMake(410, 180);
	CCPoint table_position = ccp(36, 52);
	
//	CCSprite* temp_table = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//	temp_table->setAnchorPoint(CCPointZero);
//	temp_table->setOpacity(100);
//	temp_table->setPosition(table_position);
//	main_case->addChild(temp_table, kCSS_Z_content);
	
	card_table = CCTableView::create(this, table_size);
	card_table->setAnchorPoint(CCPointZero);
	card_table->setDirection(kCCScrollViewDirectionVertical);
	card_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	card_table->setPosition(table_position);
	card_table->setDelegate(this);
	main_case->addChild(card_table, kCSS_Z_content);
	card_table->setTouchPriority(-180-5);
	
	TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-184);
	t_suction->setNotSwallowRect(CCRectMake(table_position.x, table_position.y, table_size.width, table_size.height));
	t_suction->setTouchEnabled(true);
	addChild(t_suction);
	
	
	CommonButton* close_menu = CommonButton::createCloseButton(-185);
	close_menu->setPosition(getContentPosition(kCSS_MT_close));
	close_menu->setFunction([=](CCObject* sender)
							{
								CCNode* t_node = CCNode::create();
								t_node->setTag(kCSS_MT_close);
								menuAction(t_node);
							});
	main_case->addChild(close_menu, kCSS_Z_content);
	
	
//	CommonButton* strength_menu = CommonButton::create("카드강화", 12, CCSizeMake(100,40), CommonButtonYellow, -185);
//	strength_menu->setTitleColor(ccBLACK);
//	strength_menu->setPosition(ccp(78,31));
//	main_case->addChild(strength_menu, kCSS_Z_content);
//	strength_menu->setFunction([=](CCObject* sender)
//							   {
//								   CCNode* t_node = CCNode::create();
//								   t_node->setTag(kCSS_MT_strength);
//								   menuAction(t_node);
//							   });
	
	
	CommonButton* align_default_menu = CommonButton::create("기본정렬", 12, CCSizeMake(75,42), CommonButtonGreen, -185);
	align_default_menu->setTitleColor(ccBLACK);
	align_default_menu->setPosition(ccp(254,31));
	main_case->addChild(align_default_menu, kCSS_Z_alignButton);
	align_default_menu->setFunction([=](CCObject* sender)
								 {
									 CCNode* t_node = CCNode::create();
									 t_node->setTag(kCSS_MT_alignDefault);
									 menuAction(t_node);
								 });
	
	
	CommonButton* align_take_menu = CommonButton::create("획득순", 12, CCSizeMake(75,42), CommonButtonGreen, -185);
	align_take_menu->setTitleColor(ccBLACK);
	align_take_menu->setPosition(ccp(333,31));
	main_case->addChild(align_take_menu, kCSS_Z_alignButton);
	align_take_menu->setFunction([=](CCObject* sender)
									{
										CCNode* t_node = CCNode::create();
										t_node->setTag(kCSS_MT_alignTake);
										menuAction(t_node);
									});
	
	
	CommonButton* align_rank_menu = CommonButton::create("등급순", 12, CCSizeMake(75,42), CommonButtonGreen, -185);
	align_rank_menu->setTitleColor(ccBLACK);
	align_rank_menu->setPosition(ccp(412,31));
	main_case->addChild(align_rank_menu, kCSS_Z_alignButton);
	align_rank_menu->setFunction([=](CCObject* sender)
								 {
									 CCNode* t_node = CCNode::create();
									 t_node->setTag(kCSS_MT_alignRank);
									 menuAction(t_node);
								 });
	
	
	CCSprite* title_case = CCSprite::create("cardsetting_title.png");
	title_case->setPosition(ccp(240,256));
	main_case->addChild(title_case, kCSS_Z_content);
	
	CCLabelTTF* take_card_count = CCLabelTTF::create(CCString::createWithFormat("%d 장", mySGD->getHasGottenCardsSize())->getCString(), mySGD->getFont().c_str(), 10);
	take_card_count->setPosition(ccp(398,255));
	main_case->addChild(take_card_count, kCSS_Z_content);
	
	
	
	CCSprite* n_tip = CCSprite::create("mainflow_tip.png");
	CCSprite* s_tip = CCSprite::create("mainflow_tip.png");
	s_tip->setColor(ccGRAY);
	
	CCMenuItem* tip_item = CCMenuItemSprite::create(n_tip, s_tip, this, menu_selector(CardSettingPopup::menuAction));
	tip_item->setTag(kCSS_MT_tip);
	
	CCMenu* tip_menu = CCMenu::createWithItem(tip_item);
	tip_menu->setPosition(ccp(465,(myDSH->puzzle_ui_top-320.f)/2.f+320.f-3 -13));
	addChild(tip_menu, kCSS_Z_content);
	
	tip_menu->setTouchPriority(-185);
	
	
    return true;
}

void CardSettingPopup::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
}

void CardSettingPopup::showPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(CardSettingPopup::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void CardSettingPopup::endShowPopup()
{
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_upgradeClick)
	{
		TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
		t_tutorial->initStep(kTutorialFlowStep_upgradeClick);
		addChild(t_tutorial, kCSS_Z_popup);
		
		tutorial_node = t_tutorial;
		
		card_table->setTouchEnabled(false);
	}
	else
	{
//		if(!myDSH->getBoolForKey(kDSH_Key_was_opened_tutorial_dimed_cardsetting))
//		{
//			myDSH->setBoolForKey(kDSH_Key_was_opened_tutorial_dimed_cardsetting, true);
//			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
//			t_suction->target_touch_began = t_suction;
//			t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
//			t_suction->setTouchEnabled(true);
//			
//			CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_cardsetting.png");
//			dimed_tip->setPosition(ccp(240,160));
//			t_suction->addChild(dimed_tip);
//			
//			addChild(t_suction, kCSS_Z_popup);
//		}
	}
	
	is_menu_enable = true;
}

void CardSettingPopup::hidePopup()
{
	is_menu_enable = false;
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160-450));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(CardSettingPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void CardSettingPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

CCPoint CardSettingPopup::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kCSS_MT_close)			return_value = ccp(452,256);
	else if(t_tag == kCSS_MT_cardBase)	return_value = ccp(245,210);
	
	return return_value;
}

void CardSettingPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_upgradeClick)
	{
		int tag = ((CCNode*)pSender)->getTag();
		
		if(tag == kCSS_MT_strength)
		{
			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_upgradeScript);
			is_menu_enable = false;
			CardStrengthPopup* t_popup = CardStrengthPopup::create();
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, kMainFlowZorder_popup);
			
			target_final = NULL;
			hidePopup();
		}
	}
	else
	{
		is_menu_enable = false;
		int tag = ((CCNode*)pSender)->getTag();
		
		if(tag == kCSS_MT_close)
		{
			vector<SaveUserData_Key> save_userdata_list;
			
			save_userdata_list.push_back(kSaveUserData_Key_cardsInfo);
			save_userdata_list.push_back(kSaveUserData_Key_selectedCard);
			
			myDSH->saveUserData(save_userdata_list, nullptr);
			
			
			if(mySGD->before_cardsetting == kSceneCode_PuzzleMapScene)
				hidePopup();
			else if(mySGD->before_cardsetting == kSceneCode_StageSetting)
			{
				StageSettingPopup* t_popup = StageSettingPopup::create();
				t_popup->setHideFinalAction(target_final, delegate_final);
				getParent()->addChild(t_popup, kMainFlowZorder_popup);
				
				target_final = NULL;
				hidePopup();
			}
		}
		else if(tag == kCSS_MT_alignDefault)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) != kCST_default)
			{
				myDSH->setIntegerForKey(kDSH_Key_cardSortType, kCST_default);
				recent_sort_type = kCST_default;
				mySGD->changeSortType(kCST_default);
				alignChange();
			}
			
			is_menu_enable = true;
		}
		else if(tag == kCSS_MT_alignRank)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) != kCST_gradeDown)
			{
				myDSH->setIntegerForKey(kDSH_Key_cardSortType, kCST_gradeDown);
				recent_sort_type = kCST_gradeDown;
				mySGD->changeSortType(kCST_gradeDown);
				alignChange();
			}
			else
			{
				myDSH->setIntegerForKey(kDSH_Key_cardSortType, kCST_gradeUp);
				recent_sort_type = kCST_gradeUp;
				mySGD->changeSortType(kCST_gradeUp);
				alignChange();
			}
			
			is_menu_enable = true;
		}
		else if(tag == kCSS_MT_alignTake)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) != kCST_take)
			{
				myDSH->setIntegerForKey(kDSH_Key_cardSortType, kCST_take);
				recent_sort_type = kCST_take;
				mySGD->changeSortType(kCST_take);
				alignChange();
			}
			else
			{
				myDSH->setIntegerForKey(kDSH_Key_cardSortType, kCST_takeReverse);
				recent_sort_type = kCST_takeReverse;
				mySGD->changeSortType(kCST_takeReverse);
				alignChange();
			}
			
			is_menu_enable = true;
		}
		else if(tag == kCSS_MT_strength)
		{
			mySGD->setStrengthTargetCardNumber(myDSH->getIntegerForKey(kDSH_Key_selectedCard));
			mySGD->setCardStrengthBefore(kCardStrengthBefore_cardSetting);
			CardStrengthPopup* t_popup = CardStrengthPopup::create();
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, kMainFlowZorder_popup);
			
			target_final = NULL;
			hidePopup();
		}
		else if(tag == kCSS_MT_tip)
		{
			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
			t_suction->target_touch_began = t_suction;
			t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
			t_suction->setTouchEnabled(true);
			
			CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_cardsetting.png");
			dimed_tip->setPosition(ccp(240,160));
			t_suction->addChild(dimed_tip);
			
			addChild(t_suction, kCSS_Z_popup);
			
			is_menu_enable = true;
		}
		else if(tag >= kCSS_MT_cardMenuBase && tag < kCSS_MT_noCardBase)
		{
			int clicked_card_number = tag-kCSS_MT_cardMenuBase;
			
			if(clicked_card_number != 0)
			{
				mySGD->selected_collectionbook = clicked_card_number;
				
				CollectionBookPopup* t_popup = CollectionBookPopup::create();
				t_popup->setHideFinalAction(target_final, delegate_final);
				getParent()->addChild(t_popup, kMainFlowZorder_popup);
				
				target_final = NULL;
				hidePopup();
				
				//			CCDirector::sharedDirector()->replaceScene(CollectionBook::scene());
			}
			else
			{
				is_menu_enable = true;
			}
		}
		else if(tag >= kCSS_MT_noCardBase)
		{
			is_menu_enable = true;
		}
	}
}


void CardSettingPopup::alignChange()
{
	card_table->reloadData();
}

void CardSettingPopup::cellAction( CCObject* sender )
{
	
}

CCTableViewCell* CardSettingPopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CardSortType sort_type = (CardSortType)recent_sort_type;
	
	if(sort_type == kCST_default)
	{
		int puzzle_count = server_puzzle_list_count;
		
		int t_idx = idx*2;
		int found_stage1 = -1;
		int selected_cnt = 0;
		for(int i=1;i<=puzzle_count && found_stage1 == -1;i++)
		{
			int stage_count = server_puzzle_stage_count[i];
			
			if(t_idx >= selected_cnt && t_idx < selected_cnt+stage_count)
			{
				int start_stage = server_puzzle_start_stage[i];
				found_stage1 = t_idx-selected_cnt+start_stage;
			}
			else
				selected_cnt += stage_count;
		}
		
//		if(found_stage1 == -1)
//		{
//			int event_stage_count = server_event_list_count;
//			if(t_idx >= selected_cnt && t_idx < selected_cnt+event_stage_count)
//				found_stage1 = t_idx-selected_cnt+event_stage_count;
//			else
//				CCLog("not found stage");
//		}
		
		if(found_stage1 != -1)
		{
			for(int i=1;i<=3;i++)
			{
				int card_number = mySGD->isHasGottenCards(found_stage1, i);
				CCPoint card_position = ccp(32.f + (i-1)*(68.f+1.f), 43.f);
				if(card_number > 0)
				{
					string case_type;
					if(i == 1)
						case_type = "bronze";
					else if(i == 2)
						case_type = "silver";
					else if(i == 3)
						case_type = "gold";
					
					GraySprite* n_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																												  NSDS_GI(found_stage1, kSDS_SI_level_int1_card_i, i))->getCString()));
					n_card->setGray(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) <= 0);
					CCSprite* n_case = CCSprite::create(CCString::createWithFormat("card_case_mini_%s.png", case_type.c_str())->getCString());
					n_case->setPosition(ccp(n_card->getContentSize().width/2.f, n_card->getContentSize().height/2.f));
					n_card->addChild(n_case);
					n_card->setScale(0.85f);
					
					CCSprite* n_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, n_card->getContentSize().width*0.85f, n_card->getContentSize().height*0.85f));
					n_node->setOpacity(0);
					n_card->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
					n_node->addChild(n_card);
					
					GraySprite* s_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																												  NSDS_GI(found_stage1, kSDS_SI_level_int1_card_i, i))->getCString()));
					if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) <= 0)
						s_card->setColor(ccc3(60, 60, 60));
					else
						s_card->setColor(ccGRAY);
					CCSprite* s_case = CCSprite::create(CCString::createWithFormat("card_case_mini_%s.png", case_type.c_str())->getCString());
					s_case->setPosition(ccp(s_card->getContentSize().width/2.f, s_card->getContentSize().height/2.f));
					s_card->addChild(s_case);
					s_card->setScale(0.85f);
					
					CCSprite* s_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, s_card->getContentSize().width*0.85f, s_card->getContentSize().height*0.85f));
					s_node->setOpacity(0);
					s_card->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
					s_node->addChild(s_card);
					
					CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(CardSettingPopup::menuAction));
					t_card_item->setTag(kCSS_MT_cardMenuBase+card_number);
					
					ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
					t_card_menu->setPosition(card_position);
					cell->addChild(t_card_menu);
					t_card_menu->setTouchPriority(-180-3);
					
					CCPoint no_minus_half_size = ccp(-n_card->getContentSize().width/2.f, -n_card->getContentSize().height/2.f);
					
					CCLabelTTF* t_durability = CCLabelTTF::create(CCString::createWithFormat("%d", myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number))->getCString(),
																  mySGD->getFont().c_str(), 7);
					t_durability->setAnchorPoint(ccp(0.5f,0.5f));
					t_durability->setPosition(ccpAdd(card_position, ccp(20, no_minus_half_size.y+14)));
					cell->addChild(t_durability);
					
					CCLabelTTF* t_card_level_label = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, card_number))->getCString(), mySGD->getFont().c_str(), 8);
					t_card_level_label->setPosition(ccpAdd(card_position, ccpAdd(no_minus_half_size, ccp(48,73))));
					cell->addChild(t_card_level_label);
					
					if(recent_selected_card_number == card_number)
					{
						CCSprite* select_img = CCSprite::create("card_check.png");
						select_img->setScale(0.85f);
						select_img->setPosition(card_position);
						cell->addChild(select_img);
					}
				}
				else
				{
					CCSprite* n_back = CCSprite::create("card_case_mini_back.png");
					n_back->setScale(0.85f);
					CCSprite* n_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, n_back->getContentSize().width*0.85f, n_back->getContentSize().height*0.85f));
					n_node->setOpacity(0);
					n_back->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
					n_node->addChild(n_back);
					
					CCSprite* s_back = CCSprite::create("card_case_mini_back.png");
					s_back->setScale(0.85f);
					s_back->setColor(ccGRAY);
					CCSprite* s_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, s_back->getContentSize().width*0.85f, s_back->getContentSize().height*0.85f));
					s_node->setOpacity(0);
					s_back->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
					s_node->addChild(s_back);
					
					CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(CardSettingPopup::menuAction));
					t_card_item->setTag(kCSS_MT_noCardBase+card_number);
					
					ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
					t_card_menu->setPosition(card_position);
					cell->addChild(t_card_menu);
					t_card_menu->setTouchPriority(-180-3);
				}
			}
		}
		
		{ // next stage
			int t_idx = idx*2+1;
			int found_stage1 = -1;
			int selected_cnt = 0;
			for(int i=1;i<=puzzle_count && found_stage1 == -1;i++)
			{
				int stage_count = server_puzzle_stage_count[i];
				
				if(t_idx >= selected_cnt && t_idx < selected_cnt+stage_count)
				{
					int start_stage = server_puzzle_start_stage[i];
					found_stage1 = t_idx-selected_cnt+start_stage;
				}
				else
					selected_cnt += stage_count;
			}
			
//			if(found_stage1 == -1)
//			{
//				int event_stage_count = server_event_list_count;
//				if(t_idx >= selected_cnt && t_idx < selected_cnt+event_stage_count)
//					found_stage1 = t_idx-selected_cnt+event_stage_count;
//				else
//					CCLog("not found stage");
//			}
			
			if(found_stage1 != -1)
			{
				for(int i=1;i<=3;i++)
				{
					int card_number = mySGD->isHasGottenCards(found_stage1, i);
					CCPoint card_position = ccp(32.f + 3*(68.f+1.f) + (i-1)*(68.f+1.f), 43.f);
					if(card_number > 0)
					{
						string case_type;
						if(i == 1)
							case_type = "bronze";
						else if(i == 2)
							case_type = "silver";
						else if(i == 3)
							case_type = "gold";
						
						GraySprite* n_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																													  NSDS_GI(found_stage1, kSDS_SI_level_int1_card_i, i))->getCString()));
						n_card->setGray(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) <= 0);
						CCSprite* n_case = CCSprite::create(CCString::createWithFormat("card_case_mini_%s.png", case_type.c_str())->getCString());
						n_case->setPosition(ccp(n_card->getContentSize().width/2.f, n_card->getContentSize().height/2.f));
						n_card->addChild(n_case);
						n_card->setScale(0.85f);
						
						CCSprite* n_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, n_card->getContentSize().width*0.85f, n_card->getContentSize().height*0.85f));
						n_node->setOpacity(0);
						n_card->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
						n_node->addChild(n_card);
						
						GraySprite* s_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																													  NSDS_GI(found_stage1, kSDS_SI_level_int1_card_i, i))->getCString()));
						if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) <= 0)
							s_card->setColor(ccc3(60, 60, 60));
						else
							s_card->setColor(ccGRAY);
						CCSprite* s_case = CCSprite::create(CCString::createWithFormat("card_case_mini_%s.png", case_type.c_str())->getCString());
						s_case->setPosition(ccp(s_card->getContentSize().width/2.f, s_card->getContentSize().height/2.f));
						s_card->addChild(s_case);
						s_card->setScale(0.85f);
						
						CCSprite* s_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, s_card->getContentSize().width*0.85f, s_card->getContentSize().height*0.85f));
						s_node->setOpacity(0);
						s_card->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
						s_node->addChild(s_card);
						
						CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(CardSettingPopup::menuAction));
						t_card_item->setTag(kCSS_MT_cardMenuBase+card_number);
						
						ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
						t_card_menu->setPosition(card_position);
						cell->addChild(t_card_menu);
						t_card_menu->setTouchPriority(-180-3);
						
						CCPoint no_minus_half_size = ccp(-n_card->getContentSize().width/2.f, -n_card->getContentSize().height/2.f);
						
						CCLabelTTF* t_durability = CCLabelTTF::create(CCString::createWithFormat("%d", myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number))->getCString(),
																	  mySGD->getFont().c_str(), 7);
						t_durability->setAnchorPoint(ccp(0.5f,0.5f));
						t_durability->setPosition(ccpAdd(card_position, ccp(20, no_minus_half_size.y+14)));
						cell->addChild(t_durability);
						
						
						CCLabelTTF* t_card_level_label = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, card_number))->getCString(), mySGD->getFont().c_str(), 8);
						t_card_level_label->setPosition(ccpAdd(card_position, ccpAdd(no_minus_half_size, ccp(48,73))));
						cell->addChild(t_card_level_label);
						
						if(recent_selected_card_number == card_number)
						{
							CCSprite* select_img = CCSprite::create("card_check.png");
							select_img->setScale(0.85f);
							select_img->setPosition(card_position);
							cell->addChild(select_img);
						}
					}
					else
					{
						CCSprite* n_back = CCSprite::create("card_case_mini_back.png");
						n_back->setScale(0.85f);
						CCSprite* n_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, n_back->getContentSize().width*0.85f, n_back->getContentSize().height*0.85f));
						n_node->setOpacity(0);
						n_back->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
						n_node->addChild(n_back);
						
						CCSprite* s_back = CCSprite::create("card_case_mini_back.png");
						s_back->setScale(0.85f);
						s_back->setColor(ccGRAY);
						CCSprite* s_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, s_back->getContentSize().width*0.85f, s_back->getContentSize().height*0.85f));
						s_node->setOpacity(0);
						s_back->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
						s_node->addChild(s_back);
						
						CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(CardSettingPopup::menuAction));
						t_card_item->setTag(kCSS_MT_noCardBase+card_number);
						
						ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
						t_card_menu->setPosition(card_position);
						cell->addChild(t_card_menu);
						t_card_menu->setTouchPriority(-180-3);
					}
				}
			}
		}
	}
	else
	{
		for(int i=idx*6;i<idx*6+6 && i<mySGD->getHasGottenCardsSize();i++)
		{
			int card_number = mySGD->getHasGottenCardsDataCardNumber(i);
			int card_durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number);
			CCPoint card_position = ccp(32.f + (i-idx*6)*(68.f+1.f), 43.f);
			
			int card_grade = NSDS_GI(kSDS_CI_int1_grade_i, card_number);
			string case_type;
			if(card_grade == 1)
				case_type = "bronze";
			else if(card_grade == 2)
				case_type = "silver";
			else if(card_grade == 3)
				case_type = "gold";
			
			GraySprite* n_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																										  card_number)->getCString()));
			n_card->setGray(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) <= 0);
			CCSprite* n_case = CCSprite::create(CCString::createWithFormat("card_case_mini_%s.png", case_type.c_str())->getCString());
			n_case->setPosition(ccp(n_card->getContentSize().width/2.f, n_card->getContentSize().height/2.f));
			n_card->addChild(n_case);
			n_card->setScale(0.85f);
			
			CCSprite* n_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, n_card->getContentSize().width*0.85f, n_card->getContentSize().height*0.85f));
			n_node->setOpacity(0);
			n_card->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
			n_node->addChild(n_card);
			
			GraySprite* s_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																										  card_number)->getCString()));
			if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) <= 0)
				s_card->setColor(ccc3(60, 60, 60));
			else
				s_card->setColor(ccGRAY);
			CCSprite* s_case = CCSprite::create(CCString::createWithFormat("card_case_mini_%s.png", case_type.c_str())->getCString());
			s_case->setPosition(ccp(s_card->getContentSize().width/2.f, s_card->getContentSize().height/2.f));
			s_card->addChild(s_case);
			s_card->setScale(0.85f);
			
			CCSprite* s_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, s_card->getContentSize().width*0.85f, s_card->getContentSize().height*0.85f));
			s_node->setOpacity(0);
			s_card->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
			s_node->addChild(s_card);
			
			CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(CardSettingPopup::menuAction));
			t_card_item->setTag(kCSS_MT_cardMenuBase+card_number);
			
			ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
			t_card_menu->setPosition(card_position);
			cell->addChild(t_card_menu);
			t_card_menu->setTouchPriority(-180-3);
			
			CCPoint no_minus_half_size = ccp(-n_card->getContentSize().width/2.f, -n_card->getContentSize().height/2.f);
			
			CCLabelTTF* t_durability = CCLabelTTF::create(CCString::createWithFormat("%d", card_durability)->getCString(),
														  mySGD->getFont().c_str(), 7);
			t_durability->setAnchorPoint(ccp(0.5f,0.5f));
			t_durability->setPosition(ccpAdd(card_position, ccp(20, no_minus_half_size.y+14)));
			cell->addChild(t_durability);
			
			
			CCLabelTTF* t_card_level_label = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, card_number))->getCString(), mySGD->getFont().c_str(), 8);
			t_card_level_label->setPosition(ccpSub(card_position, ccpAdd(no_minus_half_size, ccp(48,73))));
			cell->addChild(t_card_level_label);
			

			if(recent_selected_card_number == card_number)
			{
				CCSprite* select_img = CCSprite::create("card_check.png");
				select_img->setScale(0.85f);
				select_img->setPosition(card_position);
				cell->addChild(select_img);
			}
		}
	}
	
	return cell;
}

void CardSettingPopup::scrollViewDidScroll( CCScrollView* view )
{
	
}

void CardSettingPopup::scrollViewDidZoom( CCScrollView* view )
{
	
}

void CardSettingPopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	
}

CCSize CardSettingPopup::cellSizeForTable( CCTableView *table )
{
	return CCSizeMake(392, 84);
}

unsigned int CardSettingPopup::numberOfCellsInTableView( CCTableView *table )
{
	CardSortType sort_type = CardSortType(recent_sort_type);
	
	if(sort_type == kCST_default)
	{
		return default_align_number_of_cell;
	}
	else
	{
		return (mySGD->getHasGottenCardsSize()-1)/6+1;
	}
}

bool CardSettingPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	CCLog("touch swallow!!");
	
	return true;
}
void CardSettingPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void CardSettingPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void CardSettingPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void CardSettingPopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}
