//
//  PuzzlePiece.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 19..
//
//

#ifndef __DGproto__PuzzlePiece__
#define __DGproto__PuzzlePiece__

#include "cocos2d.h"
#include "SelectorDefine.h"
#include "GraySprite.h"
#include "DataStorageHub.h"
#include "ServerDataSave.h"
#include "StageImgLoader.h"
#include "StarGoldData.h"
#include "hspConnector.h"
#include "GDWebSprite.h"

USING_NS_CC;
using namespace std;

enum PieceZorder{
	kPieceZorder_level = 1,
	kPieceZorder_menu,
	kPieceZorder_stroke,
	kPieceZorder_haveInfo,
	kPieceZorder_nick
};

enum PieceMode{
	kPieceMode_default = 0,
	kPieceMode_thumb,
	kPieceMode_ranker
};

enum PieceType{
	kPieceType_empty = 0,
	kPieceType_grayStroke,
	kPieceType_colorStroke,
	kPieceType_color,
	kPieceType_buy,
	kPieceType_lock
};

class PuzzlePiece : public CCNode
{
public:
	static PuzzlePiece* create(int t_stage_number, int t_stage_level, CCObject* t_clicked, SEL_CallFuncI d_clicked)
	{
		PuzzlePiece* n_pp = new PuzzlePiece();
		n_pp->myInit(t_stage_number, t_stage_level, t_clicked, d_clicked);
		n_pp->autorelease();
		return n_pp;
	}
	
	bool turnPiece(PieceMode t_pm)
	{
		if((!is_turnable && t_pm == kPieceMode_thumb) || piece_mode == t_pm)
			return false;
		
		piece_mode = t_pm;
		
		setPieceImg();
		
		is_menu_enable = false;
		endTurnPiece();
		return true;
	}
	
	void startGetPieceAnimation (CCObject * t_create_particle, SEL_CallFuncCCp d_create_particle)
	{
		target_create_particle = t_create_particle;
		delegate_create_particle = d_create_particle;
		get_animation_mode = (PieceMode)myDSH->getIntegerForKey(kDSH_Key_puzzleMode);
		
		CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.05f, 1.f, 0, 0, 90, 0, 0);
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(PuzzlePiece::myChangeAction));
		CCOrbitCamera* t_orbit2 = CCOrbitCamera::create(0.05f, 1.f, 0, -90, 90, 0, 0);
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(PuzzlePiece::myChangeAction));
		CCSequence* t_seq1 = CCSequence::create(t_orbit1, t_call1, t_orbit2, t_call2, NULL);
		CCRepeat* t_repeat1 = CCRepeat::create(t_seq1, 10);
		CCCallFunc* t_call4 = CCCallFunc::create(this, callfunc_selector(PuzzlePiece::originalMode));
		CCSequence* t_seq4 = CCSequence::createWithTwoActions(t_repeat1, t_call4);
		
		CCDelayTime* t_delay1 = CCDelayTime::create(0.2f);
		CCCallFunc* t_call3 = CCCallFunc::create(this, callfunc_selector(PuzzlePiece::showParticle));
		CCSequence* t_seq2 = CCSequence::create(t_delay1, t_call3, NULL);
		CCRepeat* t_repeat2 = CCRepeat::create(t_seq2, 5);
		CCSequence* t_seq3 = CCSequence::create(t_seq4, t_repeat2, NULL);
		runAction(t_seq3);
	}
	
	void startGetStarAnimation(int t_star, CCObject* t_end, SEL_CallFunc d_end)
	{
		if(t_star == 1)
		{
			if(star1)
			{
				CCScaleTo* t_scale1 = CCScaleTo::create(0.3f, 2.f);
				CCScaleTo* t_scale2 = CCScaleTo::create(0.15f, 1.f);
				CCCallFunc* t_call = CCCallFunc::create(t_end, d_end);
				CCSequence* t_seq = CCSequence::create(t_scale1, t_scale2, t_call, NULL);
				star1->runAction(t_seq);
			}
			else
				(t_end->*d_end)();
		}
		else if(t_star == 2)
		{
			if(star2)
			{
				CCScaleTo* t_scale1 = CCScaleTo::create(0.3f, 2.f);
				CCScaleTo* t_scale2 = CCScaleTo::create(0.15f, 1.f);
				CCCallFunc* t_call = CCCallFunc::create(t_end, d_end);
				CCSequence* t_seq = CCSequence::create(t_scale1, t_scale2, t_call, NULL);
				star2->runAction(t_seq);
			}
			else
				(t_end->*d_end)();
		}
		else if(t_star == 3)
		{
			if(star3)
			{
				CCScaleTo* t_scale1 = CCScaleTo::create(0.3f, 2.f);
				CCScaleTo* t_scale2 = CCScaleTo::create(0.15f, 1.f);
				CCCallFunc* t_call = CCCallFunc::create(t_end, d_end);
				CCSequence* t_seq = CCSequence::create(t_scale1, t_scale2, t_call, NULL);
				star3->runAction(t_seq);
			}
			else
				(t_end->*d_end)();
		}
		else
			(t_end->*d_end)();
	}
	
	string getWorH()
	{
		return WorH;
	}
	
	int getLevel()
	{
		return stage_level;
	}
	
	void setTurnInfo(bool have_card1, bool have_card2, bool have_card3)
	{
		is_have_card[0] = have_card1;
		is_have_card[1] = have_card2;
		is_have_card[2] = have_card3;
		is_turnable = true;
	}
	
	void initWithPieceInfo(PieceMode t_pm, PieceType t_pt, string t_WorH)
	{
		piece_mode = t_pm;
		piece_type = t_pt;
		WorH = t_WorH;
		
		setPieceImg();
	}
	
private:
	
	CCObject* target_create_particle;
	SEL_CallFuncCCp delegate_create_particle;
	PieceMode get_animation_mode;
	
	CCSprite* star1;
	CCSprite* star2;
	CCSprite* star3;
	
	void originalMode ()
	{
//		setPuzzleMode((PieceMode)myDSH->getIntegerForKey(kDSH_Key_puzzleMode));
	}
	void showParticle ()
	{
		(target_create_particle->*delegate_create_particle)(getPosition());
	}
	void myChangeAction ()
	{
		get_animation_mode = (PieceMode)((get_animation_mode + 1)%(kPieceMode_thumb+1));
//		setPuzzleMode(get_animation_mode);
	}
	
	/*
	 3단계 -> 클릭 가능, 턴 가능 / stage number
	 2단계 -> 클릭 가능, 턴 가능 / stage number
	 1단계 -> 클릭 가능, 턴 가능 / stage number
	 없음  -> 클릭 가능, 턴 불가 / stage number, stage level
	 buy  -> 클릭 가능(buy 함수로 연결), 턴 불가 / stage number, stage level(open시 필요)
	 lock -> 클릭 가능(lock 함수로 연결), 턴 불가 / stage number
	 */
	
	int stage_number; // menu enable
	int stage_level; // empty
	string WorH;
	
	bool is_have_card[3];
	
	bool is_menu_enable;
	bool is_turnable;
	
	bool is_clicked;
	
	PieceMode piece_mode;
	PieceType piece_type;
	
	CCMenu* piece_menu;
	
	void setPieceImg()
	{
		removeAllChildren();
		star1 = NULL;
		star2 = NULL;
		star3 = NULL;
//		if(piece_menu)
//			piece_menu->removeFromParent();
		
		int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
		
		if(piece_type == kPieceType_color)
		{
			if(piece_mode == kPieceMode_default)
			{
				GraySprite* n_piece = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_original_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				CCSprite* n_embo = CCSprite::create(CCString::createWithFormat("piece_embo_%s.png", WorH.c_str())->getCString());
				n_embo->setPosition(ccp(n_piece->getContentSize().width/2.f, n_piece->getContentSize().height/2.f));
				n_piece->addChild(n_embo);
				setStageLevel(n_embo, false);
				GraySprite* s_piece = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_original_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				CCSprite* s_embo = CCSprite::create(CCString::createWithFormat("piece_embo_%s.png", WorH.c_str())->getCString());
				s_embo->setPosition(ccp(s_piece->getContentSize().width/2.f, s_piece->getContentSize().height/2.f));
				s_piece->addChild(s_embo);
				setStageLevel(s_embo, true);
				s_piece->setColor(ccGRAY);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
				piece_menu = CCMenu::createWithItem(piece_item);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu, kPieceZorder_menu);
			}
			else if(piece_mode == kPieceMode_thumb)
			{
				GraySprite* n_piece = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				CCSprite* n_embo = CCSprite::create(CCString::createWithFormat("piece_embo_%s.png", WorH.c_str())->getCString());
				n_embo->setPosition(ccp(n_piece->getContentSize().width/2.f, n_piece->getContentSize().height/2.f));
				n_piece->addChild(n_embo);
//				setHaveCardBase(n_piece, false);
				GraySprite* s_piece = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				s_piece->setColor(ccGRAY);
				CCSprite* s_embo = CCSprite::create(CCString::createWithFormat("piece_embo_%s.png", WorH.c_str())->getCString());
				s_embo->setPosition(ccp(s_piece->getContentSize().width/2.f, s_piece->getContentSize().height/2.f));
				s_piece->addChild(s_embo);
//				setHaveCardBase(s_piece, true);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
				piece_menu = CCMenu::createWithItem(piece_item);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu, kPieceZorder_menu);
			}
			else if(piece_mode == kPieceMode_ranker)
			{
				createRankImg();
			}
		}
//		else if(piece_type == kPieceType_colorStroke)
//		{
//			createBasicPieceMenu(false);
//			
//			CCSprite* stroke_img = CCSprite::create(("piece_stroke_" + WorH + ".png").c_str());
//			stroke_img->setPosition(CCPointZero);
//			addChild(stroke_img, kPieceZorder_stroke);
//		}
//		else if(piece_type == kPieceType_grayStroke)
//		{
//			createBasicPieceMenu(true);
//			
//			CCSprite* stroke_img = CCSprite::create(("piece_stroke_" + WorH + ".png").c_str());
//			stroke_img->setPosition(CCPointZero);
//			addChild(stroke_img, kPieceZorder_stroke);
//		}
		else if(piece_type == kPieceType_empty)
		{
			if(piece_mode == kPieceMode_default)
			{
				CCSprite* n_piece = CCSprite::create(("piece_embo_" + WorH + ".png").c_str());
				n_piece->setOpacity(0);
				setStageLevel(n_piece, false);
				CCSprite* s_piece = CCSprite::create(("piece_embo_" + WorH + ".png").c_str());
				s_piece->setOpacity(0);
				setStageLevel(s_piece, true);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
				
				piece_menu = CCMenu::createWithItem(piece_item);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu);
			}
			else if(piece_mode == kPieceMode_ranker)
			{
				createRankImg();
			}
			else if(piece_mode == kPieceMode_thumb)
			{
				GraySprite* n_piece = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				n_piece->setGray(true);
				CCSprite* n_embo = CCSprite::create(CCString::createWithFormat("piece_embo_%s.png", WorH.c_str())->getCString());
				n_embo->setPosition(ccp(n_piece->getContentSize().width/2.f, n_piece->getContentSize().height/2.f));
				n_piece->addChild(n_embo);
				//				setHaveCardBase(n_piece, false);
				GraySprite* s_piece = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				s_piece->setColor(ccc3(80, 80, 80));
				CCSprite* s_embo = CCSprite::create(CCString::createWithFormat("piece_embo_%s.png", WorH.c_str())->getCString());
				s_embo->setPosition(ccp(s_piece->getContentSize().width/2.f, s_piece->getContentSize().height/2.f));
				s_piece->addChild(s_embo);
				//				setHaveCardBase(s_piece, true);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
				piece_menu = CCMenu::createWithItem(piece_item);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu, kPieceZorder_menu);
			}
		}
		else if(piece_type == kPieceType_buy)
		{
			if(piece_mode == kPieceMode_default)
			{
				CCSprite* n_piece = CCSprite::create(("piece_buy_" + WorH + ".png").c_str());
				CCSprite* n_type = CCSprite::create("price_gold_img.png");
				n_type->setScale(0.6f);
				n_type->setPosition(ccp(n_piece->getContentSize().width/2.f-13, n_piece->getContentSize().height/2.f-12));
				n_piece->addChild(n_type);
				CCLabelTTF* n_quantity_label = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number))->getCString(), mySGD->getFont().c_str(), 8);
				n_quantity_label->setAnchorPoint(ccp(0.5,0.5));
				n_quantity_label->setPosition(ccp(n_piece->getContentSize().width/2.f+6, n_piece->getContentSize().height/2.f-13));
				n_piece->addChild(n_quantity_label);
				
				CCSprite* s_piece = CCSprite::create(("piece_buy_" + WorH + ".png").c_str());
				s_piece->setColor(ccGRAY);
				CCSprite* s_type = CCSprite::create("price_gold_img.png");
				s_type->setScale(0.6f);
				s_type->setColor(ccGRAY);
				s_type->setPosition(ccp(s_piece->getContentSize().width/2.f-13, s_piece->getContentSize().height/2.f-12));
				s_piece->addChild(s_type);
				CCLabelTTF* s_quantity_label = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number))->getCString(), mySGD->getFont().c_str(), 8);
				s_quantity_label->setColor(ccGRAY);
				s_quantity_label->setAnchorPoint(ccp(0.5,0.5));
				s_quantity_label->setPosition(ccp(s_piece->getContentSize().width/2.f+6, s_piece->getContentSize().height/2.f-13));
				s_piece->addChild(s_quantity_label);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
				
				piece_menu = CCMenu::createWithItem(piece_item);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu);
			}
			else if(piece_mode == kPieceMode_ranker)
			{
				createRankImg();
			}
			else if(piece_mode == kPieceMode_thumb)
			{
				GraySprite* n_piece = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				n_piece->setGray(true);
				CCSprite* n_embo = CCSprite::create(CCString::createWithFormat("piece_embo_%s.png", WorH.c_str())->getCString());
				n_embo->setPosition(ccp(n_piece->getContentSize().width/2.f, n_piece->getContentSize().height/2.f));
				n_piece->addChild(n_embo);
				//				setHaveCardBase(n_piece, false);
				GraySprite* s_piece = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				s_piece->setColor(ccc3(80, 80, 80));
				CCSprite* s_embo = CCSprite::create(CCString::createWithFormat("piece_embo_%s.png", WorH.c_str())->getCString());
				s_embo->setPosition(ccp(s_piece->getContentSize().width/2.f, s_piece->getContentSize().height/2.f));
				s_piece->addChild(s_embo);
				//				setHaveCardBase(s_piece, true);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
				piece_menu = CCMenu::createWithItem(piece_item);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu, kPieceZorder_menu);
			}
		}
		else if(piece_type == kPieceType_lock)
		{
			if(piece_mode == kPieceMode_default)
			{
				CCSprite* n_piece = CCSprite::create(("piece_lock_" + WorH + ".png").c_str());
				CCSprite* s_piece = CCSprite::create(("piece_lock_" + WorH + ".png").c_str());
				s_piece->setColor(ccGRAY);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
				
				piece_menu = CCMenu::createWithItem(piece_item);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu);
			}
			else if(piece_mode == kPieceMode_ranker)
			{
				createRankImg();
			}
			else if(piece_mode == kPieceMode_thumb)
			{
				GraySprite* n_piece = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				n_piece->setGray(true);
				CCSprite* n_embo = CCSprite::create(CCString::createWithFormat("piece_embo_%s.png", WorH.c_str())->getCString());
				n_embo->setPosition(ccp(n_piece->getContentSize().width/2.f, n_piece->getContentSize().height/2.f));
				n_piece->addChild(n_embo);
				//				setHaveCardBase(n_piece, false);
				GraySprite* s_piece = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_face_piece%d.png", puzzle_number,
																											   NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number)-1)->getCString()));
				s_piece->setColor(ccc3(80, 80, 80));
				CCSprite* s_embo = CCSprite::create(CCString::createWithFormat("piece_embo_%s.png", WorH.c_str())->getCString());
				s_embo->setPosition(ccp(s_piece->getContentSize().width/2.f, s_piece->getContentSize().height/2.f));
				s_piece->addChild(s_embo);
				//				setHaveCardBase(s_piece, true);
				
				CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, s_piece, this, menu_selector(PuzzlePiece::menuAction));
				piece_menu = CCMenu::createWithItem(piece_item);
				piece_menu->setPosition(CCPointZero);
				addChild(piece_menu, kPieceZorder_menu);
			}
		}
	}
	
	void endTurnPiece()
	{
		is_menu_enable = true;
	}
	
	CCObject* target_clicked;
	SEL_CallFuncI delegate_clicked;
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		is_menu_enable = false;
		
		(target_clicked->*delegate_clicked)(stage_number);
		
		is_menu_enable = true;
	}
	
	void myInit(int t_stage_number, int t_stage_level, CCObject* t_clicked, SEL_CallFuncI d_clicked)
	{
		star1 = NULL;
		star2 = NULL;
		star3 = NULL;
		
		stage_number = t_stage_number;
		stage_level = t_stage_level;
		target_clicked = t_clicked;
		delegate_clicked = d_clicked;
		
		
		
		is_menu_enable = true;
	}
	
	void setStageLevel(CCSprite* piece_img, bool is_gray)
	{
		CCSprite* level_label = CCSprite::create("level_label.png");
		level_label->setPosition(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f));
		piece_img->addChild(level_label);
		
		CCLabelBMFont* level_count = CCLabelBMFont::create(CCString::createWithFormat("%d", stage_level)->getCString(), "allfont.fnt");
		level_count->setAnchorPoint(ccp(0.5,0.5));
		level_count->setPosition(ccp(piece_img->getContentSize().width/2.f-9, piece_img->getContentSize().height/2.f-10));
		piece_img->addChild(level_count);
		
		if(!is_gray)
		{
			ccColor3B font_color = ccc3(0,0,0);
			if(stage_level <= 1)
				font_color = ccc3(255,255,255);
			else if(stage_level == 2)
				font_color = ccc3(255,255,102);
			else if(stage_level == 3)
				font_color = ccc3(255,204,102);
			else if(stage_level == 4)
				font_color = ccc3(204,255,102);
			else if(stage_level == 5)
				font_color = ccc3(102,255,204);
			else if(stage_level == 6)
				font_color = ccc3(102,204,255);
			else if(stage_level == 7)
				font_color = ccc3(204,102,255);
			else if(stage_level == 8)
				font_color = ccc3(255,111,207);
			else if(stage_level == 9)
				font_color = ccc3(255,0,128);
			else if(stage_level == 10)
				font_color = ccc3(128,0,128);
			else if(stage_level == 11)
				font_color = ccc3(128,0,0);
			else if(stage_level == 12)
				font_color = ccc3(128,64,0);
			else if(stage_level == 13)
				font_color = ccc3(128,128,0);
			else if(stage_level == 14)
				font_color = ccc3(0,128,128);
			else if(stage_level == 15)
				font_color = ccc3(0,64,128);
			else if(stage_level == 16)
				font_color = ccc3(64,0,128);
			else if(stage_level >= 17)
				font_color = ccc3(0,0,0);
			
//			level_label->setColor(font_color);
			level_count->setColor(font_color);
		}
		else
		{
			ccColor3B font_color = ccc3(100,100,100);
//			level_label->setColor(font_color);
			level_count->setColor(font_color);
		}
		
		if(is_have_card[0])
		{
			CCSprite* star_img = CCSprite::create("piece_star_bronze.png");
			star_img->setPosition(ccpAdd(ccp(level_label->getContentSize().width/2.f, level_label->getContentSize().height/2.f), ccp(-13,4)));
			level_label->addChild(star_img);
			
			if(!is_gray)
				star1 = star_img;
		}
		if(is_have_card[1])
		{
			CCSprite* star_img = CCSprite::create("piece_star_silver.png");
			star_img->setPosition(ccpAdd(ccp(level_label->getContentSize().width/2.f, level_label->getContentSize().height/2.f), ccp(13,4)));
			level_label->addChild(star_img);
			
			if(!is_gray)
				star2 = star_img;
		}
		if(is_have_card[2])
		{
			CCSprite* star_img = CCSprite::create("piece_star_gold.png");
			star_img->setPosition(ccpAdd(ccp(level_label->getContentSize().width/2.f, level_label->getContentSize().height/2.f), ccp(0,8)));
			level_label->addChild(star_img);
			
			if(!is_gray)
				star3 = star_img;
		}
	}
	
	void createRankImg()
	{
		if(mySGD->temp_stage_ranker_list["result"]["code"].asInt() == GDSUCCESS)
		{
			string stage_ranker_user_id;
			for(int i=0;i<mySGD->temp_stage_ranker_list["list"].size();i++)
			{
				if(mySGD->temp_stage_ranker_list["list"][i][0]["stageNo"].asInt() == stage_number)
				{
					stage_ranker_user_id = mySGD->temp_stage_ranker_list["list"][i][0]["memberID"].asString();
					break;
				}
			}
			
			if(!stage_ranker_user_id.empty())
			{
				bool is_found = false;
				string found_nick;
				string found_url;
				
				if(stage_ranker_user_id == hspConnector::get()->myKakaoInfo["user_id"].asString())
				{
					is_found = true;
					found_nick = hspConnector::get()->myKakaoInfo["nickname"].asString();
					found_url = hspConnector::get()->myKakaoInfo["profile_image_url"].asString();
				}
				
				for(auto i : KnownFriends::getInstance()->getFriends())
				{
					if(!is_found)
					{
						if(i.userId == stage_ranker_user_id)
						{
							is_found = true;
							found_nick = i.nick;
							found_url = i.profileUrl;
							break;
						}
					}
					else
						break;
				}
				
				for(auto i : UnknownFriends::getInstance()->getFriends())
				{
					if(!is_found)
					{
						if(i.userId == stage_ranker_user_id)
						{
							is_found = true;
							found_nick = i.nick;
							found_url = i.profileUrl;
							break;
						}
					}
					else
						break;
				}
				
				if(is_found)
				{
					CCSprite* n_piece = GDWebSprite::create(found_url.c_str(), "piece_noimg.png");
					n_piece->setAnchorPoint(ccp(0.5,0.5));
					//						CCSprite* s_piece = GDWebSprite::create(found_url.c_str(), "piece_ranker_noimg.png");
					//						s_piece->setScale(58.f / s_piece->getContentSize().width);
					//						s_piece->setColor(ccGRAY);
					
					
					CCNode *abc = CCNode::create();
					CCMenuItem* piece_item = CCMenuItemSprite::create(n_piece, abc, this, menu_selector(PuzzlePiece::menuAction));
					piece_menu = CCMenu::createWithItem(piece_item);
					piece_menu->setPosition(CCPointZero);
					addChild(piece_menu, kPieceZorder_menu);
					
					CCLabelTTF* nick_label = CCLabelTTF::create(found_nick.c_str(), mySGD->getFont().c_str(), 15);
					nick_label->setColor(ccBLACK);
					nick_label->setPosition(CCPointZero);
					addChild(nick_label, kPieceZorder_nick);
				}
			}
		}
	}
};

#endif /* defined(__DGproto__PuzzlePiece__) */
