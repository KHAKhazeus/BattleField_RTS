//BuildingBase.h
//Jiang
//18/5/25

#ifndef _BATTLEFIELD_BUILDINGBASE
#define _BATTLEFIELD_BUILDINGBASE

#include <iostream>
#include <cocos2d.h>
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

// a base class for buildngs
class BuildingBase :public cocos2d::Sprite
{
protected:
	int _lifeValue;//life value for the building
	LoadingBar* _HP;//hp bar of the building
	float _HPInterval;//interval of the building's hp
	int _buildID;//ID of the building
	int _campID;//building belongs to which side
	int _gold;//expence to build the building
	float _buildTime;//time needed to build the building
	int _elect;//electricity spent to build the building
	LoadingBar* _buildBar;// the bar diaplayed during building
public:

	//set and get the building's lifevalue
	void setLifeValue(int lifeValue) { _lifeValue = lifeValue; }
	int getLifeValue() { return _lifeValue; }

	//set and get the building's life bar
	void setHP(LoadingBar* HP) { _HP = HP; }
	LoadingBar* getHP() { return _HP; }

	//set and get the HPInterval
	void setHPInterval(float HPInterval) { _HPInterval = HPInterval; }
	int getHPInterval() { return _HPInterval; }

	//set and get the cost for the building
	void setGold(int gold) { _gold = gold; }
	int getGold() { return _gold; }

	//set and get the ID for the building
	void setBuildID(int ID) { _buildID = ID; }
	int getBuildID() { return _buildID; }

	//set and get the building time for the building
	void setBuildTime(float time) { _buildTime = time; }
	int getBuildTime() { return _buildTime; }

	//set and get the building time for the building
	void setElect(float elect) { _elect = elect; }
	int getElect() { return _elect; }

	//set and get the building's building bar
	void setBuildBar(LoadingBar* buildBar) { _buildBar = buildBar; }
	LoadingBar* getBuildBar() { return _buildBar; }

	//set and get the side for the building
	void setCampID(int ID) { _campID = ID; }
	int getCampID() { return _campID; }


	
	// Get the aniamtion
	Animate* getAnimateByName(std::string animName, float delay, int animNum);
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
	}

	void Build();
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
	}

	void Build();
};
#endif