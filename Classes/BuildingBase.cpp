//BuildingBase.cpp
//Jiang
//18/5/30

#include "BuildingBase.h"

bool BuildingBase::_isbuilt = false;

bool BuildingBase::getIsBuilt() {
	return _isbuilt;
}

void BuildingBase::setIsBuilt(bool judge) {
	_isbuilt = judge;
}

Animate* BuildingBase:: getAnimateByName(std::string animName, float delay, int animNum) {
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

//Build the building
void SoldierBase::Build() {
	auto animate =BuildingBase::getAnimateByName("soldierBase/soldierbase_", 0.1f, 23);
	auto barSprite = Sprite::create("bar/loadingbar.png");
	ProgressTimer* progress = ProgressTimer::create(barSprite);
	progress->setPercentage(0.0f);
	progress->setScale(0.3f);
	progress->setMidpoint(Vec2(0.0f, 0.5f));
	progress->setBarChangeRate(Vec2(1.0f, 0.f));
	progress->setType(ProgressTimer::Type::BAR);
	Vec2 pos = Vec2(this->getPosition().x, this->getPosition().y);
	progress->setPosition(Vec2(140, 140));
	this->addChild(progress);
	auto pft = ProgressFromTo::create(2.3f, progress->getPercentage(), 100);
	this->runAction(animate);
	auto sequence = Sequence::create(pft,CallFunc::create([=] {
		this->removeChild(progress,true);
	}), CallFunc::create([] {BuildingBase::setIsBuilt(false); }), nullptr);
	progress->runAction(sequence);

}

//Build the building
void MoneyMine::Build() {
	auto animate = BuildingBase::getAnimateByName("moneyMine/MinetoMoney_", 0.2f, 24);
	auto barSprite = Sprite::create("bar/loadingbar.png");
	ProgressTimer* progress = ProgressTimer::create(barSprite);
	progress->setPercentage(0.0f);
	progress->setScale(0.3f);
	progress->setMidpoint(Vec2(0.0f, 0.5f));
	progress->setBarChangeRate(Vec2(1.0f, 0.f));
	progress->setType(ProgressTimer::Type::BAR);
	Vec2 pos = Vec2(this->getPosition().x, this->getPosition().y);
	progress->setPosition(Vec2(120, 160));
	this->addChild(progress);
	auto pft = ProgressFromTo::create(4.6f, progress->getPercentage(), 100);
	this->runAction(animate);
	auto sequence = Sequence::create(pft, CallFunc::create([=] {
		this->removeChild(progress, true);
	}), CallFunc::create([] {BuildingBase::setIsBuilt(false); }), nullptr);
	progress->runAction(sequence);
}

//Build the building
void WarFactory::Build() {
	auto animate = BuildingBase::getAnimateByName("tankBase/tankbuilding_", 0.1f, 23);
	auto barSprite = Sprite::create("bar/loadingbar.png");
	ProgressTimer* progress = ProgressTimer::create(barSprite);
	progress->setPercentage(0.0f);
	progress->setScale(0.3f);
	progress->setMidpoint(Vec2(0.0f, 0.5f));
	progress->setBarChangeRate(Vec2(1.0f, 0.f));
	progress->setType(ProgressTimer::Type::BAR);
	Vec2 pos = Vec2(this->getPosition().x, this->getPosition().y);
	progress->setPosition(Vec2(160, 160));
	this->addChild(progress);
	auto pft = ProgressFromTo::create(2.3f, progress->getPercentage(), 100);
	this->runAction(animate);
	auto sequence = Sequence::create(pft, CallFunc::create([=] {
		this->removeChild(progress, true);
	}), CallFunc::create([] {BuildingBase::setIsBuilt(false); }), nullptr);
	progress->runAction(sequence);
}

//Build the building
void PowerPlant::Build() {
	
	auto animate = PowerPlant::getAnimateByName("powerPlant/PowerBuilt_", 0.1f, 23);
	auto barSprite = Sprite::create("bar/loadingbar.png");
	ProgressTimer* progress = ProgressTimer::create(barSprite);
	progress->setPercentage(0.0f);
	progress->setScale(0.3f);
	progress->setMidpoint(Vec2(0.0f, 0.5f));
	progress->setBarChangeRate(Vec2(1.0f, 0.f));
	progress->setType(ProgressTimer::Type::BAR);
	Vec2 pos = Vec2(this->getPosition().x, this->getPosition().y);
	progress->setPosition(Vec2(140, 140));
	this->addChild(progress);
	auto pft = ProgressFromTo::create(2.3f, progress->getPercentage(), 100);
	this->runAction(animate);
	auto sequence = Sequence::create(pft, CallFunc::create([=] {
		this->removeChild(progress, true);
	}), CallFunc::create([] {BuildingBase::setIsBuilt(false); }), nullptr);
	progress->runAction(sequence);
}



