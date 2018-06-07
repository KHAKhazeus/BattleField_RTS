//FighterUnitBase.h
//BatteleField_RTS
//Jiang
//18/5/25


#ifndef _BATTLEFIELD_FIGHTERUNITBASE
#define _BATTLEFIELD_FIGHTERUNITBASE

#include <iostream>
#include <cocos2d.h>
#include "BuildingBase.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

//A base class for fighter unit
class FighterUnitBase :public Unit
{
protected:
	int _lifeValue;//the life for a unit
	LoadingBar* _HP;//a unit's hp bar
	float _HPInterval;//Hp's update value
	bool _isMove;//if the unit need move
	bool _isAttack;//if the unit can attack
	int _attack;//unit's attacking power
	int _unitID;//an ID to distinguish differnt fighter unit
	int _targetID;//the attacking target's ID
	float _moveSpeed;//the speed of the unit
	float _attackInterval;//the interval between attacking
	float _attackRange;//the attacking range of the unit
	int _gold;//the expence of the unit
	bool _isSelected;//whther the unit has been selected;
	int _campID;//unit belongs to which side
	Vec2 _tiledPos; //the tiledPostion of the Unit
public:

	// Get the aniamtion
	Animate * getAnimateByName(std::string animName, float delay, int animNum);
	
	//set and get the unit's lifevalue
	void setLifeValue(int lifeValue) { _lifeValue = lifeValue; }
	int getLifeValue() { return _lifeValue; }

	//set and get the unit's life bar
	void setHP(LoadingBar* HP) { _HP = HP; }
	LoadingBar* getHP() { return _HP; }

	//set and get the HPInterval
	void setHPInterval(float HPInterval) { 
		_HPInterval = HPInterval;
		if (_HPInterval <= 0) {
			setDeath();
		}
	}
	int getHPInterval() { return _HPInterval; }

	//set and judge whether the unit can move
	void setMove(bool move) { _isMove = move; }
	bool isMove() { return _isMove; }

	//set and get the ID for the unit
	void setUnitID(int ID) { _unitID = ID; }
	int getUnitID() { return _unitID; }

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

	//set and get the moving speed for the unit
	void setMoveSpeed(int moveSpeed) { _moveSpeed = moveSpeed; }
	int getMoveSpeed() { return _moveSpeed; }

	//set and get the attacking range for the unit
	void setAttackRange(int attackRange) { _attackRange = attackRange; }
	int getAttackRange() { return _attackRange; }

	//set and get the attacking target for the unit
	void setTargetID(int target) { _targetID = target; }
	int getTargetID() { return _targetID; }


	//set and judge whether the unit can move
	void setSelected(bool selected) { _isSelected = selected; }
	bool isSelected() { return _isSelected; }

	//set and get the side for the unit
	virtual void setCampID(int ID) { _campID = ID; }
	virtual int getCampID() { return _campID; }

	//set and get the tiledPosition of the unit
	void setTiledPositon(Vec2 pos) { _tiledPos = pos; }
	Vec2 getTiledPositon() { return _tiledPos; }
	
	//attack action
	//@@param is the enemy's ID
	void attack(int id);

	//to judge if the enemy is in the attack range
	bool judgeAttack(Vec2 pos);

	//Unit death
	void setDeath() {
		//TODO animation?
		this->getParent()->removeChild(this, true);
	}

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
		//
		setIsBuilding();
		setCampID(RED);
		setUnitID(getIdCount());
		addIdCount();
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
		//
		setIsBuilding();
		setCampID(RED);
		setUnitID(getIdCount());
		addIdCount();
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
		//
		setIsBuilding();
		setCampID(RED);
		setUnitID(getIdCount());
		addIdCount();
		
	}
	void Create(WarFactory*);
};
#endif