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
public:
	bool iscreated;
	bool isselected;
	static Base* create();
	bool init();
	//TODO get the Sprite . A better Way?
	Sprite * getSprite(); 
	int getMinRange();
	int getMaxRange();
	virtual bool onTouchBegan(Touch *touch, Event *event);
	virtual void onTouchEnded(Touch *touch, Event *event);
};


#endif // !_UNIT_IMAGE

