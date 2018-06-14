#ifndef _UNIT_MANAGER
#define _UNIT_MANAGER
#include "cocos2d.h"
#include "TiledMap.h"
#include "UnitCreate.h"
#include "BuildingBase.h"
#include "MyResources.h"
#include "MouseRect.h"
#include "PathArithmetic.h"
#include "GameMessageInterface.h"
#include <cmath>
#include "SocketClient.h"
#include "SocketServer.h"
USING_NS_CC;

class UnitManager :public cocos2d::Node {
public:
	static GameMessageInterface * msgs ;
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

	void selectUnitsByPoint(Vec2 touch_point);
	void selectUnitsByRect(MouseRect* mouse_rect);

	void playerMoveWithWayPoints(Unit* player, Vec2 position, std::vector<Vec2> path);

	void delay(float);
	//@@param1 the Pointer to the Player
	//@@param2 the Pointer to the Enemy
	void attack(Unit* player, Unit* enemy);
	
	void attackEffect(Unit* player, Unit* enemy);

	//the unit destroy effect
	void destroyEffect(Unit *unit,bool type);
	static void Building(int new_building_id, std::string new_building_type, int base_id, int from_building_id,
		cocos2d::Vec2 position);
	static void NewUnitCreate(int new_unit_id, std::string new_unit_type, int base_id, int from_building_id,
		cocos2d::Vec2 position);
private:
	//the count of _building
	int _building;
	//the count of _soider;
	int _soider;
	//the point to TiledMap
	TiledMap * _tiled_Map;
	Base * _base;
	std::shared_ptr<SocketClient> _socketClient;
	std::shared_ptr<SocketServer> _socketServer;
};


#endif // !_UNIT_MANAGER

