#ifndef _UNIT_CREATE
#define _UNIT_CREATE
#include "cocos2d.h"
#include "MyResources.h"
#include "BuildingBase.h"
#include "Unit.h"
USING_NS_CC;

class Base :public Unit {
private:
	Unit * _base;
	std::vector<Sprite *>_build_child;
	int _max_range;
	bool _isbuilt;
	bool _isCreated;
public:
	
	static Base* create();
	bool init();
	//TODO get the Sprite . A better Way?
	Unit * getBase(); 
	//get the range of the Base
	int getMaxRange();
	//get the boolean if we can build a building
	bool getBuilt();
	//set the boolean if we can build a building
	void setBuilt(bool judge);
	virtual bool onTouchBegan(Touch *touch, Event *event);
	virtual void onTouchEnded(Touch *touch, Event *event);

	//set and judge whether the unit has been created
	void setCreated(bool created) { _isCreated = created; }
	bool isCreated() { return _isCreated; }
};


#endif // !_UNIT_IMAGE

