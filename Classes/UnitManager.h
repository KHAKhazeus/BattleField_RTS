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
	GameMessageInterface * msgs ;
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

	void playerMoveWithWayPoints(int move_unit_id, std::vector<cocos2d::Vec2> path_points, cocos2d::Vec2 end_point);

	void delay(float);
	//@@param1 the Pointer to the Player
	//@@param2 the Pointer to the Enemy
	void attack(int attacker_id, int under_attack_id, int damage);
	
	void attackEffect(Unit* player, Unit* enemy);


	//the unit destroy effect
	void destroyEffect(Unit *unit,bool type);
	static void Building(int new_building_id, std::string new_building_type, int base_id, int from_building_id,
		cocos2d::Vec2 position);
	static void NewUnitCreate(int new_unit_id, std::string new_unit_type, int base_id, int from_building_id,
		cocos2d::Vec2 position);

	void updateMessage(float delta);
	
	void autoAttack(float dt);

	Vector<Unit*>& getUnitVector() { return _unit_Vector; }

//	void attack(Unit *player,Unit *target);


	std::vector<GameMessage>& getMessages() { return _gameMessages; }
	void addMessages(GameMessage msg) { _gameMessages.push_back(msg); }

	std::shared_ptr<SocketClient>getSocketClient() { return _socket_client; }
	std::shared_ptr<SocketServer>getSocketServer() { return _socket_server; }
	void writeOrders(std::vector<GameMessage> messages) {
		_orders = messages;
	}
private:
	//the count of _building
	int _building;
	//the count of _soider;
	int _soider;
	//the point to TiledMap
	TiledMap * _tiled_Map;
	Base * _base;
	//Vector of the fightUnit created by us
	Vector<Unit*> _unit_Vector;
	std::shared_ptr<SocketClient> _socket_client{ static_cast<SocketClient*>(nullptr),[](SocketClient*) {} };
	std::shared_ptr<SocketServer> _socket_server{ static_cast<SocketServer*>(nullptr),[](SocketServer*) {} };
	std::vector<GameMessage> _gameMessages;
	std::vector<GameMessage>_orders;
};


#endif // !_UNIT_MANAGER

