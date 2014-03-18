// MapFragment.cpp
//

#include "MapFragment.h"
#define LZZ_INLINE inline
MapFragment * MapFragment::create (CCPoint s_p)
        {
		MapFragment* t_mf = new MapFragment();
		if(t_mf && t_mf->initWithFile("mapFragment.png"))
		{
			t_mf->myInit(s_p);
			t_mf->autorelease();
			return t_mf;
		}
		CC_SAFE_DELETE(t_mf);
		return NULL;
	}
void MapFragment::startFalling ()
        {
		schedule(schedule_selector(MapFragment::falling));
	}
void MapFragment::falling ()
        {
		CCPoint selfPosition = getPosition();
		if(selfPosition.x < -5 || selfPosition.x > 325 || selfPosition.y < -5)
		{
			unschedule(schedule_selector(MapFragment::falling));
			removeFromParentAndCleanup(true);
			return;
		}
		setRotation(getRotation() + changeRotateVal);
		setPosition(ccpAdd(getPosition(), dv));
		dv.y += -0.3f;
	}
void MapFragment::myInit (CCPoint s_p)
        {
		changeRotateVal = rand()%11 - 5;
		dv.x = (rand()%51 - 25.f)/10.f;
		dv.y = 3.f;
		
		int randColorVal = rand()%9;
		if(randColorVal == 0)			setColor(ccRED);
		else if(randColorVal == 1)		setColor(ccORANGE);
		else if(randColorVal == 2)		setColor(ccYELLOW);
		else if(randColorVal == 3)		setColor(ccGREEN);
		else if(randColorVal == 4)		setColor(ccBLUE);
		else if(randColorVal == 5)		setColor(ccMAGENTA);
		else if(randColorVal == 6)		setColor(ccBLACK);
		else if(randColorVal == 7)		setColor(ccGRAY);
		else							setColor(ccWHITE);
		
		setPosition(s_p);
	}
MapFragmentParent * MapFragmentParent::create ()
        {
		MapFragmentParent* t_MFP = new MapFragmentParent();
		t_MFP->myInit();
		t_MFP->autorelease();
		return t_MFP;
	}
void MapFragmentParent::createNewFragment (IntPoint s_p)
        {
		isCreateNewFragment = true;
		
		createPoint = s_p;
		
		if(!createFragmenting)
		{
			createFragmenting = true;
			schedule(schedule_selector(MapFragmentParent::createFragment));
		}
	}
void MapFragmentParent::createFragment ()
        {
		if(isCreateNewFragment)
		{
			AudioEngine::sharedInstance()->playEffect("sound_crashed_map.mp3", false);
			CCPoint c_p = ccp((createPoint.x-1)*pixelSize+1, (createPoint.y-1)*pixelSize+1);
			
			MapFragment* t_mf = MapFragment::create(c_p);
			addChild(t_mf);
			t_mf->startFalling();
			isCreateNewFragment = false;
		}
		else
		{
			unschedule(schedule_selector(MapFragmentParent::createFragment));
			createFragmenting = false;
		}
	}
void MapFragmentParent::myInit ()
        {
		CCSprite* t_texture = CCSprite::create("mapFragment.png");
		initWithTexture(t_texture->getTexture(), kDefaultSpriteBatchCapacity);
		isCreateNewFragment = false;
		createFragmenting = false;
		
//		myGD->regMFP(this, callfuncIp_selector(MapFragmentParent::createNewFragment));
		myGD->V_Ip["MFP_createNewFragment"] = std::bind(&MapFragmentParent::createNewFragment, this, _1);
	}
#undef LZZ_INLINE
