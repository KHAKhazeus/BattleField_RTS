//FighterUnitBase.h
//BatteleField_RTS
//Jiang
//18/5/25


#ifndef _BATTLEFIELD_FIGHTERUNITBASE
#define _BATTLEFIELD_FIGHTERUNITBASE

#include "BuildingBase.h"
#include "Unit.h"

USING_NS_CC;
using namespace ui;

//A base class for fighter unit
class FighterUnitBase :public Unit
{
protected:
	int _attack;//unit's attacking power
	
	float _attackInterval;//the interval between attacking
	float _attackRange;//the attacking range of the unit
	int _gold;//the expence of the unit
public:
	
	
	//set and get the cost for the unit
	void setGold(int gold) { _gold = gold; }
	int getGold() { return _gold; }

	//set and judge whether the unit is attacking
	void setAttack(bool attack) { _isAttack = attack; }
	bool isAttack() { return _isAttack; }

	//set and get the attacking power for the unit
	void setAttackPower(int attack) { _attack = attack; }
	int getAttack() { return _attack; }

	//set and get the interval of attacking for the unit
	void setAttackInterval(int attackInterval) { _attackInterval = attackInterval; }
	int getAttackInterval() { return _attackInterval; }


	//set and get the attacking range for the unit
	void setAttackRange(int attackRange) { _attackRange = attackRange; }
	int getAttackRange() { return _attackRange; }

	//set and get the attacking target for the unit
	void setTargetID(int target) { _targetID = target; }
	int getTargetID() { return _targetID; }



	//set and get the tiledPosition of the unit
	void setTiledPosition(Vec2 pos) { _tiledPos = pos; }
	Vec2 getTiledPosition() { return _tiledPos; }
	
	//attack action
	//@@param is the enemy's ID
	void attack(int id);

	//to judge if the enemy is in the attack range
	bool judgeAttack(Vec2 myPos,Vec2 pos);

	//search for the position of enemy in attackrange
	Vec2 searchEnemy();

	//judge if the targetPos can attack the enemy
	bool judgeTarPos();

	void autoAttack(float dt);
};

class Soldier :public FighterUnitBase
{
public:

	//unit's creating function
	static Soldier* create(const std::string& filename) {
		Soldier *sprite = new Soldier();
		if (sprite && sprite->initWithTexture(Director::getInstance()->getTextureCache()->addImage(filename)))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	Soldier() {
		_lifeValue = 115;
		_gold = 200;
		_attackRange = 10;
		_attack = 15;
		_attackInterval = 0.2;
		//
		setIsBuilding();
		setType(std::string("s"));
		clearAllType();
		setTargetPos(Vec2(-1,-1));
		setTempPos(Vec2(-1, -1));
	}
	void Create(Unit*);
};

class Dog :public FighterUnitBase
{
public:

	//unit's creating function
	static Dog* create(const std::string& filename) {
		Dog *sprite = new Dog();
		if (sprite && sprite->initWithTexture(Director::getInstance()->getTextureCache()->addImage(filename)))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	Dog() {
		_lifeValue = 100;
		_gold = 250;
		_attackRange = 2;
		_attack = 20;
		_attackInterval = 0.2;
		//
		setIsBuilding();
		setType(std::string("d"));
		clearAllType();
		setTargetPos(Vec2(-1, -1));
		setTempPos(Vec2(-1, -1));
	}
	void Create(Unit*);
};

class Tank :public FighterUnitBase
{
public:

	//unit's creating function
	static Tank* create(const std::string& filename) {
		Tank *sprite = new Tank();
		if (sprite && sprite->initWithTexture(Director::getInstance()->getTextureCache()->addImage(filename)))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	Tank() {
		_lifeValue = 440;
		_gold = 600;
		_attackRange = 15;
		_attack = 30;
		_attackInterval = 0.5;
		//
		setIsBuilding();
		setType(std::string("t"));
		clearAllType();
		setTargetPos(Vec2(-1, -1));
		setTempPos(Vec2(-1, -1));
	}
	void Create(Unit*);
};
#endif