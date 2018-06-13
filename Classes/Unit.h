#ifndef UNIT_H
#define UNIT_H

//Unit.h
//18/6/7
//Jiang


#include <iostream>
#include <cocos2d.h>
#include "ui/CocosGUI.h"

USING_NS_CC;

#define CONSTRUCTION "construction.mp3"
#define DOG "dogFinished.mp3"
#define SOLDIER "soldierFinished.mp3"
#define TANK "tankFinished.mp3"
#define BUILD "Building.mp3"
#define FIGHT "gun.mp3"
#define LOST "unitLost.mp3"
#define TANKBULLET "tankbullet.mp3"
#define EXPLODE "explode.mp3"

using namespace ui;

class Unit :public cocos2d::Sprite {
private:
	static int _id; //the id count of the whole building group
protected:
	int _lifeValue;//the life for a unit
	LoadingBar* _HP;//a unit's hp bar
	float _HPInterval;//Hp's update value
	bool _isMove;//if the unit need move
	int _unitID;//an ID to distinguish  unit
	int _campID;//unit belongs to which side
	bool _isSelected;//unit has been selected?
	bool _isBuilding;//if it's a building
	std::string _type;//the type of the unit
	int _range; //the lenth/2 of the building
	int _fixModel;
	bool _isProgressed;
public:
	// Get the aniamtion
	static Unit* create(const std::string& filename) {
		Unit *sprite = new Unit();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	Animate * getAnimateByName(std::string animName, float delay, int animNum) {
		Animation* animation = Animation::create();

		for (unsigned int i = 0; i <= animNum; i++) {
			// get the picture name
			std::string frameName = animName;
			frameName.append(StringUtils::format("%d", i)).append(".png");
			// add the picture to spriteframe
			animation->addSpriteFrameWithFile(frameName.c_str());
		}
		// set the properties of the animation
		animation->setDelayPerUnit(delay);
		// reset the animate
		animation->setRestoreOriginalFrame(true);
		// return the animate
		Animate* animate = Animate::create(animation);
		return animate;
	}

	//set and get the unit's lifevalue
	void setLifeValue(int lifeValue) { _lifeValue = lifeValue; }
	int getLifeValue() { return _lifeValue; }

	//set and get the unit's life bar
	void setHP(LoadingBar* HP) { _HP = HP; }
	LoadingBar* getHP() { return _HP; }

	void setProgressed(bool progressed) { _isProgressed = progressed; }
	bool getProgressed() { return _isProgressed; }

	//set and get the Id for the building
	int getIdCount() { return _id; }
	void addIdCount() { _id++; }
	virtual bool isBuilding() { return _isBuilding; }
	virtual void setIsBuilding(bool character = false) { _isBuilding = character; }

	virtual bool judgeAttack(Vec2 pos) { return false; }

	virtual void setType(std::string type) { _type = type; }
	virtual std::string getType() { return _type; }

	virtual void moveTo(Vec2 pos) {}
	virtual Vec2 getTiledPosition() { return { 0,0 }; }
	virtual void setTiledPosition(Vec2 pos) {}

	virtual int getAttack() { return 0; }
	virtual int getAttackInterval() { return 100; }
	
	//get the range of the building
	int getRange() { return _range; }
	void setRange(int range) { _range = range; }


	//set and get the HPInterval
	void setHPInterval(float HPInterval) {
		_HPInterval = HPInterval;
	}
	float getHPInterval() { return _HPInterval; }


	//set and judge whether the unit can move
	void setMove(bool move) { _isMove = move; }
	bool isMove() { return _isMove; }

	//set and get the ID for the unit
	void setUnitID(int ID) { _unitID = ID; }
	int getUnitID() { return _unitID; }

	//set and get the side for the unit
	void setCampID(int ID) { _campID = ID; }
	int getCampID() { return _campID; }

	//set and judge whether the unit has been selected
	void setSelected(bool selected) { _isSelected = selected; }
	bool getSelected() { return _isSelected; }

	void setFixModel(int model) { _fixModel = model; }
	int getFixModel() { return _fixModel; }
	//Unit death
	void setDeath() {
		//TODO animation?
		this->getParent()->removeChild(this, true);
	}
	
};

#endif // !UNIT_H
