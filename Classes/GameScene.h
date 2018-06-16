#ifndef _GameScene
#define _GameScene
#include "cocos2d.h"
#include "MyResources.h"
#include "UnitManager.h"
#include "BuildingBase.h"
#include "FighterUnitBase.h"
#include "MouseRect.h"
USING_NS_CC;

//Main Class of Game Scene
class GameScene :public cocos2d::Scene {
private:

	//The Size of Screen
	int _screen_width;
	int _screen_height;

	//Cursor point
	float _cursorX, _cursorY;


	//Pointer to class needed instance
	Sprite* _money_Image;
	Sprite* _power_Image;
	Money* _money;
	Power* _power;
	//Can create the base and check something(TODO)
	UnitManager * _unit_Manager;
	//The Map class contains _collidabel_Vector and gridAndId_Map (Static)
	TiledMap *_tiled_Map;

	//vector for unit
	Vector<MoneyMine*> _moneyMine;
	Vector<PowerPlant*> _powerPlant;
	Vector<WarFactory*>_warFactory;
	Vector<SoldierBase*>_soldierBase;
	Vector<Soldier*>_soldiers;
	Vector<Dog*>_dogs;
	Vector<Tank*>_tanks;

public:
	
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void update(float dt) override;
	
	Vector<MoneyMine*>& getVectorMine() { return _moneyMine; }
	Vector<PowerPlant*>& getVectorPower() { return _powerPlant; }
	Vector<SoldierBase*>& getVectorSoldier() { return _soldierBase; }
	Vector<WarFactory*>& getVectorFactory() { return _warFactory; }
	Vector<Soldier*>& getVectorSoldiers() { return _soldiers; }
	Vector<Dog*>& getVectorDogs() { return _dogs; }
	Vector<Tank*>& getVectorTanks() { return _tanks; }

	Money* getMoney() { return _money; }

	Power* getPower() { return _power; }

	TiledMap* getMap() { return _tiled_Map; }

	UnitManager* getUnitManager() { return _unit_Manager; }

	CREATE_FUNC(GameScene);
	//turn the coordinate of OpenGL to TileMap
	//The return value is the coordinate of tho position in TileMap
	Vec2 tileCoordForPosition(Vec2 position);
	
	//The function of Scene Transition
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();

	//Create the KeyBoard CallBack();
	void onKeyPressed(EventKeyboard::KeyCode, Event * event) ;
	//If you win,then ?
	//TODO
//	void win();
//	void lose();

	void resetCursor();

	//Scroll map
	void mapScroll();


	//Pass data from listener
	MouseRect* mouse_rect;

	//Touch listener
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	//Mouse listener
	void onMouseMove(Event* event);

	//The function of Touch
	
	

};


#endif _GameScene