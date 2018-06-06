#ifndef _UNIT_MANAGER
#define _UNIT_MANAGER
#include "cocos2d.h"
#include "TiledMap.h"
#include "UnitCreate.h"
#include "BuildingBase.h"
USING_NS_CC;
class UnitManager :public cocos2d::Node {
public:
	virtual bool init(TiledMap *);
	static UnitManager * create(TiledMap* tiledMap) {
		auto unitManager = new UnitManager();
		if (unitManager && unitManager->init(tiledMap)) {
			unitManager->autorelease();
			return unitManager;
		}
		CC_SAFE_DELETE(unitManager);
		return nullptr;
	}
	//TODO the measure to the layername
	//TODO A campID towards a unique string?
	Vec2 getBasePosition(std::string layername);
	void initBase();
	Base* getBase(){ return _base; }
	//
	void checkWinOrLose();
private:
	//the count of _building
	int _building;
	//the count of _soider;
	int _soider;
	//the point to TiledMap
	TiledMap * _tiled_Map;
	Base * _base;
};


#endif // !_UNIT_MANAGER

