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
public:
	bool iscreated;
	bool isselected;
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
};


#endif // !_UNIT_IMAGE

