#ifndef _UNIT_CREATE
#define _UNIT_CREATE
#include "cocos2d.h"
#include "MyResources.h"
#include "BuildingBase.h"
USING_NS_CC;

class Base :public Sprite {
private:
	Sprite * _base;
	std::vector<Sprite *>_build_child;
	int _min_range;
	int _max_range;
	bool _isbuilt;
	int _lifeValue;//life value for the building
	LoadingBar* _HP;//hp bar of the building
	float _HPInterval;//interval of the building's hp
	int _buildID;//ID of the building
	int _campID;//building belongs to which side
	bool _isCreated;
	bool _isSelected;
public:
	
	static Base* create();
	bool init();
	//TODO get the Sprite . A better Way?
	Sprite * getSprite(); 
	//get the range of the Base
	int getMinRange();
	int getMaxRange();
	//get the boolean if we can build a building
	bool getBuilt();
	//set the boolean if we can build a building
	void setBuilt(bool judge);
	virtual bool onTouchBegan(Touch *touch, Event *event);
	virtual void onTouchEnded(Touch *touch, Event *event);

	//set and get the building's lifevalue
	void setLifeValue(int lifeValue) { _lifeValue = lifeValue; }
	int getLifeValue() { return _lifeValue; }

	//set and get the building's life bar
	void setHP(LoadingBar* HP) { _HP = HP; }
	LoadingBar* getHP() { return _HP; }

	//set and judge whether the unit has been selected
	void setSelected(bool selected) { _isSelected = selected; }
	bool isSelected() { return _isSelected; }


	//set and judge whether the unit has been created
	void setCreated(bool created) { _isCreated = created; }
	bool isCreated() { return _isCreated; }

	//set and get the HPInterval
	void setHPInterval(float HPInterval) { _HPInterval = HPInterval; }
	int getHPInterval() { return _HPInterval; }

	//set and get the side for the building
	void setCampID(int ID) { _campID = ID; }
	int getCampID() { return _campID; }
};


#endif // !_UNIT_IMAGE

