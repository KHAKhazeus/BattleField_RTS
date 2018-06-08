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
	bool _isAttack;//if the unit can attack
	int _attack;//unit's attacking power
	int _targetID;//the attacking target's ID
	float _moveSpeed;//the speed of the unit
	float _attackInterval;//the interval between attacking
	float _attackRange;//the attacking range of the unit
	int _gold;//the expence of the unit
	static bool _isBeingCreated;//find if there is a unit being created
public:

	//set and get the cost for the unit
	void setGold(int gold) { _gold = gold; }
	int getGold() { return _gold; }

	//set and judge whether the unit is attacking
	void setAttack(bool attack) { _isAttack = attack; }
	bool isAttack() { return _isAttack; }

	//set and judge whether there is a unit being created
	static void setIsCreated(bool created);
	static bool getIsCreated();

	//set and get the attacking power for the unit
	void setAttackPower(int attack) { _attack = attack; }
	int getAttack() { return _attack; }

	//set and get the interval of attacking for the unit
	void setAttackInterval(int attackInterval) { _attackInterval = attackInterval; }
	int getAttackInterval() { return _attackInterval; }

	//set and get the moving speed for the unit
	void setMoveSpeed(int moveSpeed) { _moveSpeed = moveSpeed; }
	int getMoveSpeed() { return _moveSpeed; }

	//set and get the attacking range for the unit
	void setAttackRange(int attackRange) { _attackRange = attackRange; }
	int getAttackRange() { return _attackRange; }

	//set and get the attacking target for the unit
	void setTargetID(int target) { _targetID = target; }
	int getTargetID() { return _targetID; }


	//attack action
	//@@param is the enemy's ID
	void attack(int id);


};

class Soldier :public FighterUnitBase
{
public:
	//unit's creating function
	static Soldier* create(const std::string& filename) {
		Soldier *sprite = new Soldier();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	Soldier() {
		_lifeValue = 115;
		_moveSpeed = 6;
		_gold = 80;
		_attackRange = 70;
		_attack = 15;
		_attackInterval = 0.2;
	}
	void Create(SoldierBase*);
};

class Dog :public FighterUnitBase
{
public:
	//unit's creating function
	static Dog* create(const std::string& filename) {
		Dog *sprite = new Dog();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	Dog() {
		_lifeValue = 100;
		_moveSpeed = 9;
		_gold = 150;
		_attackRange = 0;
		_attack = 20;
		_attackInterval = 0.2;
	}
	void Create(SoldierBase*);
};

class Tank :public FighterUnitBase
{
public:
	//unit's creating function
	static Tank* create(const std::string& filename) {
		Tank *sprite = new Tank();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	Tank() {
		_lifeValue = 440;
		_moveSpeed = 8;
		_gold = 500;
		_attackRange = 100;
		_attack = 30;
		_attackInterval = 0.5;
	}
	void Create(WarFactory*);
};
#endif