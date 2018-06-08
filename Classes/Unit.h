#ifndef UNIT_H
#define UNIT_H

//Unit.h
//18/6/7
//Jiang


#include <iostream>
#include <cocos2d.h>
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class Unit :public cocos2d::Sprite {
protected:
	int _lifeValue;//the life for a unit
	LoadingBar* _HP;//a unit's hp bar
	float _HPInterval;//Hp's update value
	bool _isMove;//if the unit need move
	int _unitID;//an ID to distinguish  unit
	int _campID;//unit belongs to which side
	bool _isSelected;//unit has been selected?
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

		for (unsigned int i = 1; i <= animNum; i++) {
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

	//set and get the side for the unit
	void setCampID(int ID) { _campID = ID; }
	int getCampID() { return _campID; }

	//set and judge whether the unit has been selected
	void setSelected(bool selected) { _isSelected = selected; }
	bool getSelected() { return _isSelected; }

	//Unit death
	void setDeath() {
		//TODO animation?
		this->getParent()->removeChild(this, true);
	}
};

#endif // !UNIT_H
