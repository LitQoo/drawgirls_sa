//
//  MissileParent.h
//  DrawingJack
//
//  Created by 사원3 on 12. 12. 4..
//
//

#ifndef DrawingJack_MissileParent_h
#define DrawingJack_MissileParent_h

#include "cocos2d.h"
#include "GameData.h"
#include <queue>
#include "AttackPattern.h"
#include "JackMissile.h"
#include "RandomSelector.h"
#include "AudioEngine.h"
#include "ShockWave.h"
#include "KSCumberBase.h"
#include "Charges.h"
#include "StoneMissile.h"
using namespace cocos2d;
using namespace std;






//class CreateSubCumberOtherAction : public CCNode
//{
//public:
	//static CreateSubCumberOtherAction* create(IntPoint c_p,
												//CCObject* t_after, SEL_CallFuncO d_after,
												//CCObject* t_cancel, SEL_CallFuncO d_cancel);
	
	//void afterAction(CCObject* cb);
	
	//void cancelAction(CCObject* cb);
	
//private:
	
	//CCObject* after_target;
	//SEL_CallFuncO after_delegate;
	//CCObject* cancel_target;
	//SEL_CallFuncO cancel_delegate;
	//IntPoint createPoint;
	
	//void myInit(IntPoint c_p, CCObject* t_after, SEL_CallFuncO d_after, CCObject* t_cancel, SEL_CallFuncO d_cancel);
//};

class UM_creator : public CCNode
{
public:
	static UM_creator* create(int t_um_tcnt, int t_create_type, float t_missile_speed);
	static UM_creator* create(int t_um_tcnt, int t_create_type, float t_missile_speed, CCPoint s_p);
	void startCreate();
	void startPetCreate();
private: 
	int ing_frame;
	int um_tcnt;
	int ing_um_cnt;
	int create_type;
	float missile_speed;
	int shoot_frame; 
	CCPoint start_position; 
	void creating(); 
	void petCreating(); 
	void stopCreate(); 
	void stopPetCreate(); 
	void myInit(int t_um_tcnt, int t_create_type, float t_missile_speed); 
	void myInit(int t_um_tcnt, int t_create_type, float t_missile_speed, CCPoint s_p);
};

class MissileParent : public CCNode
{
public:
	static MissileParent* create(CCNode* boss_eye);
	void bombCumber(CCObject* target);
	void createJackMissile(int jm_type, int cmCnt, float missile_speed, CCPoint missile_position);
	// 초기위치, 획득영역, 레벨, 등급, 미사일 타입.
	void createJackMissileWithStone(StoneType stoneType, int grade, int level, float missileNumbers, CCPoint initPosition);
	AttackOption getAttackOption(StoneType st, int grade);
	void subOneDie();
	void endIngActionAP();
	void explosion(CCPoint bombPosition, ccColor4F t_color, float t_angle);
	void createTickingTimeBomb(IntPoint t_point, int t_bombFrameOneTime, int t_bombTimes, int t_rangeCode);
	void resetTickingTimeBomb();
	virtual ~MissileParent()
	{
		//chargeArray->release();
		tickingArray->release();
	}
	void shootPetMissile(int jm_type, int cmCnt, float damage_per, CCPoint s_p);
	enum PATTERN_RET_CODE{INVALID=0, VALID=1, NOCAST=2};
	int attackWithKSCode(CCPoint startPosition, std::string pattern, KSCumberBase* cb, bool exe);
	int getJackMissileCnt();
private:
	
	SW_Parent* mySW;
	
	CCPoint startFirePosition;
	int slowAreaCnt;
	CCArray* tickingArray;
	CCSpriteBatchNode* explosion_node;
	CCNode* jack_missile_node;
	void initParticle(CCPoint startPosition, ccColor4F t_color, float t_angle);
	void removeChargeInArray(CCObject* remove_charge);
	void movingMainCumber();
	void myInit(CCNode* boss_eye);
};

#endif
