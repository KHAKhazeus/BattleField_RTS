//BuildingBase.h
//Jiang
//18/5/25

#ifndef _BATTLEFIELD_BUILDINGBASE
#define _BATTLEFIELD_BUILDINGBASE

#include <iostream>
#include <cocos2d.h>
#include "ui/CocosGUI.h"
#include "Grid.h"
#include "Unit.h"
#include "BattleField_RTS.h"

USING_NS_CC;
using namespace ui;

// a base class for buildngs
class BuildingBase :public Unit
{
protected:
	int _gold;//expence to build the building
	int _elect;//electricity spent to build the building
	LoadingBar* _buildBar;// the bar diaplayed during building
	static bool _isbuilt; //to judge whether there is builiding built
	bool _isCreated;
public:

	//set and get the cost for the building
	void setGold(int gold) { _gold = gold; }
	int getGold() { return _gold; }

	//set and get the building time for the building
	void setElect(float elect) { _elect = elect; }
	int getElect() { return _elect; }
	
	void setCreated(bool created) { _isCreated = created; }
	bool getCreated() { return _isCreated; }

	

	//set the boolean ...
	//get the boolean to judge if there is a builing built
	static bool getIsBuilt();
	static void setIsBuilt(bool judge);

	virtual bool judgeAttack(Vec2 pos) {
		return false;
	}

	int getAttack() { return 0; }
	
};

//the derived class for SoldierBase
class SoldierBase :public BuildingBase {
public:
	static SoldierBase* create(const std::string& filename) {
		SoldierBase *sprite = new SoldierBase();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	SoldierBase() {
		_gold = 500;
		_elect = 10;
		_lifeValue = 900;
		setRange(2);
		setFixModel(FIX_HEIGHT);
		//
		setCampID(RED);
		setIsBuilding(true);
	}

	Vec2 RandomPosition();
	void Build();
	bool onTouchBegan(Touch *touch, Event *event);
	
};

//the derived class for SoldierBase
class MoneyMine :public BuildingBase {
public:
	
	static MoneyMine* create(const std::string& filename) {
		MoneyMine *sprite = new MoneyMine();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	MoneyMine() {
		_gold = 2000;
		_elect = 40;
		_lifeValue = 1000;
		setRange(2);
		setFixModel(FIX_SQUARE);
		//
		setCampID(RED);
		setIsBuilding(true);
	}

	void Build();
};

//the derived class for SoldierBase
class PowerPlant :public BuildingBase {
public:
	static PowerPlant* create(const std::string& filename) {
		PowerPlant *sprite = new PowerPlant();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	PowerPlant() {
		_gold = 600;
		_elect = 150;
		_lifeValue = 750;
		setRange(1);
		setFixModel(FIX_SQUARE);
		//
		setCampID(BLUE);
		setIsBuilding(true);
	}

	void Build();
};

class WarFactory :public BuildingBase {
public:
	static WarFactory* create(const std::string& filename) {
		WarFactory *sprite = new WarFactory();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	WarFactory() {
		_gold = 2000;
		_elect = 50;
		_lifeValue = 1200;
		setRange(2);
		setFixModel(FIX_HEIGHT);
		//
		setCampID(RED);
		setIsBuilding(true);
	}

	Vec2 RandomPosition();
	void Build();
	bool onTouchBegan(Touch *touch, Event *event);
};
#endif