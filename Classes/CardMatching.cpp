//
//  CardMatching.cpp
//  DGproto
//
//  Created by ksoo k on 2013. 12. 26..
//
//

#include "CardMatching.h"
#include "PuzzleCache.h"
#include "KSUtil.h"
#include "StarGoldData.h"

void CardMatching::splitImage(CuttingType ct, const std::string& fileName, int cols, int rows, int padding, int margin,
															const std::function<void(CCImage*, int)>& callBack)
{
	std::string filename="test_puzzle7_back.png"; // CCFileUtils::sharedFileUtils()->getWritablePath() + fileName;
	
	//퍼즐이미지를 불러옵니다.
	CCImage *img = new CCImage;
	img->initWithImageFileThreadSafe(filename.c_str());
	
	//피스조각(가로형)을 불러옵니다.
	CCImage *st_w = new CCImage;
	st_w->initWithImageFile("puzzle_stencil_2_pw.png");
	
	//피스조각(세로형)을 불러옵니다.
	CCImage *st_h = new CCImage;
	st_h->initWithImageFile("puzzle_stencil_2_ph.png");
	
	
	//지금부터 퍼즐 조각 잘라 저장하기를 시작하겠습니다.
	
	std::vector<int> pieceIndexs;
	if(ct == CuttingType::kOriginal)
	{
		for(int i=0; i<PUZZLE_COLS * PUZZLE_ROWS; i++)
		{
			pieceIndexs.push_back(i);
		}
	}
	else if(ct == CuttingType::kFace)
	{
		std::vector<int> tempRandom;
		for(int i=0; i<PUZZLE_COLS * PUZZLE_ROWS; i++)
		{
			tempRandom.push_back(i);
		}
		random_shuffle(tempRandom.begin(), tempRandom.end(), [=](int n)
									 {
										 uniform_int_distribution<> dist(0, n-1);
										 return dist(m_rEngine);
									 });
		for(int i=0; i<PUZZLE_COLS * PUZZLE_ROWS / 2; i++)
		{
			pieceIndexs.push_back(tempRandom[i]);
		}
	}


	int loop = 0;
	for(auto i : pieceIndexs){
//	for(int i=0;i<PUZZLE_COLS*PUZZLE_ROWS;i++){
		//피스는 i가 짝수일때 st_w 이미지를 이용하여 자르고 홀수일때 st_h 이미지를 이용하여 자릅니다.
		CCImage *st = st_h;
		if(i%2 == 0)
			st = st_w;
		
		//피스의 좌표를 구합니다. 퍼즐은 5*4 개로 이루어져있습니다.
		int x = i % PUZZLE_COLS;
		int y = i / PUZZLE_COLS;
		
		int cutx;
		int cuty;
		
		//원본파일에서 자를 위치를 계산합니다.
		cutx = x * padding + margin;
		cuty = y * padding + margin;
		
		//자르고 저장합니다.
		PuzzleCache::getInstance()->changeStencilByOrigin(st, img, {cutx,cuty}, true);
		
		callBack(st, loop);
		loop++;
		//		addChild(pps);
	}
	//가로세로 피스 메모리해제
	st_w->release();
	st_h->release();
	
	img->release();
}

bool CardMatching::init(int priority, const std::function<void(CCObject*, SEL_CallFunc)>& hideFunction)
{
	CCLayer::init();
	CCDrawNode* shape = CCDrawNode::create();
	
	CCPoint pts[4];
	pts[0] = ccp(25, 320 - 297);
	pts[1] = ccp(366, 320 - 297);
	pts[2] = ccp(366, 320 - 24);
	pts[3] = ccp(25, 320 - 24);
	shape->drawPolygon(pts, 4, ccc4f(1, 1, 1, 1), 0, ccc4f(1, 0, 0, 1));

	CCSprite* back = CCSprite::create("bonusgame_back.png");
	back->setPosition(ccp(240, 160));
	addChild(back);
	// add shape in stencil
	m_thiz = CCClippingNode::create();
	m_thiz->setAnchorPoint(ccp(0.5, 0.5));
	m_thiz->setStencil(shape);
	this->addChild(m_thiz);
	m_priority = priority;
	m_hideFunction = hideFunction;
	m_correctCount = 0;
	m_timer = 0;
	m_state = MatchingState::kReady;
	std::random_device rd;
	m_rEngine.seed(rd());
	
	m_timeFnt = CCLabelBMFont::create(CCString::createWithFormat("%.1f", m_remainTime)->getCString(), "allfont.fnt");
	m_timeFnt->setPosition(ccp(420, 270));
	addChild(m_timeFnt, 5);
	
	setTouchEnabled(true);
	m_menu = CCMenuLambda::create();
	m_menu->setPosition(ccp(0, 0));
	m_menu->setPropaOnBegan(true);
	m_menu->setTouchPriority(m_priority);
	CCNode* containerNode = CCNode::create();
	containerNode->addChild(m_menu);
	addChild(containerNode);
	splitImage(CuttingType::kFace, "puzzle4_face.png", PUZZLE_COLS, PUZZLE_ROWS, 172, 86,
						 [=](CCImage* st, int loop)
						 {
							 auto tempFunc = [=](int loop, int weight)
							 {
								 //테스트로 한번 붙여봅니다.
								 CCTexture2D* _texture = new CCTexture2D;
//								 _texture->initWithImage(st);
//								 CCSprite* pps = CCSprite::createWithTexture(_texture);
//								 _texture->release();
//								 
//								 _texture = new CCTexture2D;
								 _texture->initWithImage(st);
								 CCSprite* pps2 = CCSprite::createWithTexture(_texture);
								 
								 _texture->release();
								 pps2->setColor(ccc3(166, 166, 166));
								 CCSprite* item = pps2;
//								 CCMenuItemSpriteLambda::create(pps, pps2,
//																																							 [=](CCObject* s)
//																																							 {
//																																								 CCLog("c!");
//																																							 });
								 item->setOpacity(0);
								 item->setUserData((void*)loop); // 짝.
								 containerNode->addChild(item);
								 m_matchCards.push_back(item);
								 loop *= 2;
								 loop += weight;
								 int x = loop % PUZZLE_COLS;
								 int y = loop / PUZZLE_COLS;
								 
								 item->setPosition(ccp((x*116 + 94)/2,(y*116 + 94)/2));
							 };
							 tempFunc(loop, 0);
							 tempFunc(loop, 1);
						 });
	random_shuffle(m_matchCards.begin(), m_matchCards.end(), [=](int n)
								 {
									 uniform_int_distribution<> dist(0, n-1);
									 return dist(m_rEngine);
								 });
	splitImage(CuttingType::kOriginal, "puzzle4_original.png", PUZZLE_COLS, PUZZLE_ROWS, 116, 94,
						 [=](CCImage* st, int loop)
						 {
							 //테스트로 한번 붙여봅니다.
							 CCTexture2D* _texture = new CCTexture2D;
//							 _texture->initWithImage(st);
//							 CCSprite* pps = CCSprite::createWithTexture(_texture);
//							 _texture->release();
//							 
//							 _texture = new CCTexture2D;
							 _texture->initWithImage(st);
							 CCSprite* pps2 = CCSprite::createWithTexture(_texture);
							 
							 _texture->release();
							 pps2->setColor(ccc3(166, 166, 166));
							 
							 
							 CCSprite* ppps = CCSprite::create("bonusgame_samecard_back.png");
							 CCSprite* ppps2 = CCSprite::create("bonusgame_samecard_back.png");
							 CCMenuItemSpriteLambda* item = CCMenuItemSpriteLambda::create(ppps, ppps2,
																																						 [=](CCObject* s)
																																						 {
																																							 CCMenuItemSpriteLambda* menuItem = (CCMenuItemSpriteLambda*)s;
																																							 CCSprite* obj = (CCSprite*)(menuItem->getUserData());
																																							 
																																							 switch(m_state)
																																							 {
																																								 case MatchingState::kReady:
																																									 obj->setOpacity(255);
																																									 m_state = MatchingState::kOneSelect;
																																									 m_firstSelectNum = (int)obj->getUserData();
																																									 m_firstSelectSprite = obj;
																																									 menuItem->setEnabled(false);
																																									 m_firstSelectItem = menuItem;
																																									 break;
																																								 case MatchingState::kOneSelect:
																																									 obj->setOpacity(255);
																																									 m_secondSelectNum = (int)obj->getUserData();
																																									 m_secondSelectSprite = obj;
																																									 menuItem->setEnabled(false);
																																									 m_secondSelectItem = menuItem;
																																									 if(m_firstSelectNum == m_secondSelectNum)
																																									 {
																																										 m_state = MatchingState::kReady;
																																										 m_correctCount++;
																																										 if(m_correctCount == PUZZLE_ROWS * PUZZLE_COLS / 2)
																																										 {
																																											 CCLog("END!!");
																																											 CCSprite* successSprite = CCSprite::create("bonusgame_succes.png");
																																											 successSprite->setPosition(ccp(240, 160));
																																											 addChild(successSprite);
																																											 CCSprite* reward = CCSprite::create("price_ruby_img.png");
																																											 reward->setPosition(ccp(205, 133));
																																											 addChild(reward);
																																											 reward->setScale(2.f);
																																											 CCLog("correct!!");
																																											 m_menu->setTouchEnabled(false);
																																											 unscheduleUpdate();
																																											 mySGD->setStar(mySGD->getStar() + 1);
																																											 myDSH->saveUserData({kSaveUserData_Key_star}, [=](Json::Value v)
																																																					 {
																																																						 addChild(KSTimer::create(3.f, [=](){
																																																							 m_hideFunction(this, callfunc_selector(ThisClassType::removeFromParent));
																																																						 }));
																																																					 });
																																										 }
																																									 }
																																									 else
																																									 {
																																										 m_state = MatchingState::kDiff;
																																										 addChild(KSTimer::create(0.5f, [=]()
																																																							{
																																																								m_state = MatchingState::kReady;
																																																								m_firstSelectSprite->setOpacity(0);
																																																								m_secondSelectSprite->setOpacity(0);
																																																								m_firstSelectItem->setEnabled(true);
																																																								m_secondSelectItem->setEnabled(true);
																																																							}));
																																									 }
																																									 break;
																																								 case MatchingState::kDiff:
																																									 CCLog("no click!");
																																									 break;
																																							 }
																																								 

																																						 });
							 
							 m_menu->addChild(item);
							 
							 //피스의 좌표를 구합니다. 퍼즐은 5*4 개로 이루어져있습니다.
							 int x = loop % PUZZLE_COLS;
							 int y = loop / PUZZLE_COLS;
							 
							 item->setPosition(ccp((x*140 + 94)/2,(y*140 + 94)/2));
							 m_matchCards[loop]->setPosition(item->getPosition());
							 item->setUserData(m_matchCards[loop]);
						 });
	
	containerNode->setPosition(ccp(10, 10));
	auto ready_go = KS::loadCCBI<CCSprite*>(this, "ui_ready.ccbi");
	addChild(ready_go.first);
	ready_go.first->setPosition(ccp(240, 160));
	return true;
}



void CardMatching::update(float dt)
{
	m_timer += dt;
	if(m_remainTime - m_timer <= 0)
	{
		
		CCSprite* failSprite = CCSprite::create("bonusgame_fail.png");
		failSprite->setPosition(ccp(240, 160));
		addChild(failSprite);
		m_menu->setTouchEnabled(false);
		addChild(KSTimer::create(3.f, [=]()
														 {
															 setVisible(false);
															 m_hideFunction(this, callfunc_selector(ThisClassType::removeFromParent));
														 }));
		unscheduleUpdate();
	}
	m_timeFnt->setString(CCString::createWithFormat("%.1f", m_remainTime - m_timer)->getCString());
}