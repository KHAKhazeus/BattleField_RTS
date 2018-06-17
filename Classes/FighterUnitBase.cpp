//FighterUnitBase.cpp
//Jiang
//18/6/1

#include "FighterUnitBase.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
#include "GameScene.h"

bool Soldier::_isBeingCreated = false;

bool Soldier::getIsCreated() {
	return _isBeingCreated;
}

void Soldier::setIsCreated(bool judge) {
	_isBeingCreated = judge;
}

bool Dog::_isBeingCreated = false;

bool Dog::getIsCreated() {
	return _isBeingCreated;
}

void Dog::setIsCreated(bool judge) {
	_isBeingCreated = judge;
}

bool Tank::_isBeingCreated = false;

bool Tank::getIsCreated() {
	return _isBeingCreated;
}

void Tank::setIsCreated(bool judge) {
	_isBeingCreated = judge;
}
void Soldier::Create(SoldierBase* soldierBase) {
	this->setVisible(false);
	auto barSprite = Sprite::create("bar/loadingbar.png");
	ProgressTimer* progress = ProgressTimer::create(barSprite);
	progress->setPercentage(0.0f);
	progress->setScale(0.3f);
	progress->setMidpoint(Vec2(0.0f, 0.5f));
	progress->setBarChangeRate(Vec2(1.0f, 0.f));
	progress->setType(ProgressTimer::Type::BAR);
	Vec2 pos = Vec2(this->getPosition().x, this->getPosition().y);
	progress->setPosition(Vec2(120, 200));
	soldierBase->addChild(progress);
	auto pft = ProgressFromTo::create(2.4f, progress->getPercentage(), 100);
	auto sequence = Sequence::create(pft, CallFunc::create([=] {
		soldierBase->removeChild(progress, true);
		this->setVisible(true);
		Soldier::setIsCreated(false);
		SimpleAudioEngine::getInstance()->playEffect(SOLDIER, false);
	}), nullptr);
	progress->runAction(sequence);
	// create a loading bar
	auto loadingBar = LoadingBar::create("bar/planeHP.png");
	loadingBar->setScale(0.04f);
	// set the percentage
	loadingBar->setPercent(60);
	// set direction
	loadingBar->setDirection(LoadingBar::Direction::LEFT);
	// set position
	loadingBar->setPosition(Vec2(25, 35));
	// set Hp bar for soldier
	this->setHP(loadingBar);
	// set the HP bar as a child
	this->addChild(this->getHP(),210);
	// set the HP interval
	this->setHPInterval(this->getHP()->getPercent() / (float)this->getLifeValue());
	loadingBar->setVisible(false);
}

void Dog::Create(SoldierBase* soldierBase) {
	this->setVisible(false);
	auto barSprite = Sprite::create("bar/loadingbar.png");
	ProgressTimer* progress = ProgressTimer::create(barSprite);
	progress->setPercentage(0.0f);
	progress->setScale(0.3f);
	progress->setMidpoint(Vec2(0.0f, 0.5f));
	progress->setBarChangeRate(Vec2(1.0f, 0.f));
	progress->setType(ProgressTimer::Type::BAR);
	Vec2 pos = Vec2(this->getPosition().x, this->getPosition().y);
	progress->setPosition(Vec2(120, 200));
	soldierBase->addChild(progress);
	auto pft = ProgressFromTo::create(2.4f, progress->getPercentage(), 100);
	auto sequence = Sequence::create(pft, CallFunc::create([=] {
		soldierBase->removeChild(progress, true);
		this->setVisible(true);
		Dog::setIsCreated(false);
		SimpleAudioEngine::getInstance()->playEffect(DOG, false);
	}), nullptr);
	progress->runAction(sequence);
	// create a loading bar
	auto loadingBar = LoadingBar::create("bar/planeHP.png");
	loadingBar->setScale(0.04f);
	// set the percentage
	loadingBar->setPercent(60);
	// set direction
	loadingBar->setDirection(LoadingBar::Direction::LEFT);
	// set position
	loadingBar->setPosition(Vec2(25,25));
	// set Hp bar for soldier
	this->setHP(loadingBar);
	// set the HP bar as a child
	this->addChild(this->getHP(), 210);
	// set the HP interval
	this->setHPInterval(this->getHP()->getPercent() / (float)this->getLifeValue());
	loadingBar->setVisible(false);
}

void Tank::Create(WarFactory* warFactory) {
	this->setVisible(false);
	auto barSprite = Sprite::create("bar/loadingbar.png");
	ProgressTimer* progress = ProgressTimer::create(barSprite);
	progress->setPercentage(0.0f);
	progress->setScaleX(0.5f);
	progress->setScaleY(0.3f);
	progress->setMidpoint(Vec2(0.0f, 0.5f));
	progress->setBarChangeRate(Vec2(1.0f, 0.f));
	progress->setType(ProgressTimer::Type::BAR);
	Vec2 pos = Vec2(this->getPosition().x, this->getPosition().y);
	progress->setPosition(Vec2(120, 160));
	warFactory->addChild(progress);
	auto pft = ProgressFromTo::create(2.4f, progress->getPercentage(), 100);
	auto sequence = Sequence::create(pft, CallFunc::create([=] {
		warFactory->removeChild(progress, true);
		this->setVisible(true);
		Tank::setIsCreated(false);
		SimpleAudioEngine::getInstance()->playEffect(TANK, false);
	}), nullptr);
	progress->runAction(sequence);
	// create a loading bar
	auto loadingBar = LoadingBar::create("bar/planeHP.png");
	loadingBar->setScale(0.1f);
	// set the percentage
	loadingBar->setPercent(60);
	// set direction
	loadingBar->setDirection(LoadingBar::Direction::LEFT);
	// set position
	loadingBar->setPosition(Vec2(90, 150));
	// set Hp bar for soldier
	this->setHP(loadingBar);
	// set the HP bar as a child
	this->addChild(this->getHP(), 210);
	// set the HP interval
	this->setHPInterval(this->getHP()->getPercent() / (float)this->getLifeValue());
	loadingBar->setVisible(false);
}

bool FighterUnitBase::judgeAttack(Vec2 pos) {
	auto myLocation = this->getPosition();
	auto myTiledLocaiton = getTiledPosition();
	auto distance = sqrt(pow(myTiledLocaiton.x - pos.x, 2) + pow(myTiledLocaiton.y - pos.y, 2));
	if (distance < this->getAttackRange()) {
		return true;
	}
	return false;
}

Vec2 FighterUnitBase::searchEnemy() {
	auto curpos = getTiledPosition();
	auto range = getAttackRange();
	auto rect = GridRect::create(curpos.x - range / 2, curpos.y - range / 2, range, range);
	for (auto i = rect->getX(); i <= rect->getX() + rect->getWidth(); i++) {
		for (auto j = rect->getY(); j <= rect->getY() + rect->getHeight(); j++) {
			auto vecPos = Vec2(i, j);
			if (TiledMap::checkBoundary(vecPos)) {
				if (TiledMap::checkMapGrid(vecPos)) {
					if (TiledMap::getUnitIdByPosition(vecPos) != getCampID()) {
						return vecPos;
					}
				}
			}
		}
	}
	return Vec2(-1,-1);
}


void FighterUnitBase::autoAttack(float dt) {
	if (isAutoAttack()) {
		auto pos = searchEnemy();
		if (pos.x != -1) {
			auto tempNode = this->getParent()->getParent()->getParent();
			auto tempScene = static_cast<GameScene*>(tempNode);
			auto id = TiledMap::getUnitIdByPosition(pos);
			auto enemy = TiledMap::getUnitById(id);
			tempScene->getUnitManager()->msgs->newAttackMessage(this->getUnitID(), enemy->getUnitID(), this->getAttack());
			tempScene->getUnitManager()->attack(this->getUnitID(), enemy->getUnitID(), this->getAttack());
			tempScene->getUnitManager()->attackEffect(this->getUnitID(), enemy->getUnitID());
		}
	}
	if (isAttack()) {
		auto m = this;
		if(!TiledMap::checkUnitId(this->getTargetID())){
			return;
		}
		auto enemy = TiledMap::getUnitById(this->getTargetID());
		auto pos = enemy->getTiledPosition();
		auto tempNode = this->getParent()->getParent()->getParent();
		auto tempScene = static_cast<GameScene*>(tempNode);
		//judge if the enemy is in the range
		//Attack
		if (this->judgeAttack(pos)) {
			this->stopAllActions();
			tempScene->getUnitManager()->msgs->newAttackMessage(this->getUnitID(), enemy->getUnitID(), this->getAttack());
			tempScene->getUnitManager()->attack(this->getUnitID(), enemy->getUnitID(), this->getAttack());
			tempScene->getUnitManager()->attackEffect(this->getUnitID(), enemy->getUnitID());
		}
		else {
			PathArithmetic* path_finder = PathArithmetic::create();
			auto tempMap = static_cast<TiledMap*>(this->getParent()->getParent());
			if (!TiledMap::checkPass(pos)) {
				pos = tempMap->findFreeNear(pos);
			}
			auto temp_pos = TiledMap::getUnitById(this->getUnitID())->getPosition();
			auto tiled_pos = tempMap->tileCoordForPosition(temp_pos);
			path_finder->initPathArithmetic(tempMap, tiled_pos, pos);
			path_finder->findPath();
			auto path = path_finder->getPath();
			tempScene->getUnitManager()->msgs->newMoveMessage(this->getUnitID(), path, pos);
			tempScene->getUnitManager()->playerMoveWithWayPoints(this->getUnitID(), path, pos);
		}
	}
}


/*
void FighterUnitBase::moveTo(Vec2 pos) {
	this->stopAllActions();
/*void FighterUnitBase::moveTo(Vec2 pos) {
	Animate* animate;
	switch (getType())
	{
		case 's':
			animate = getAnimateByName("soldierRun/soldier", 0.2f, 7);
			break;
		case 'd':
			animate = getAnimateByName("dogRun/dog", 0.2f, 10);
			break;
		case 't':
			animate = getAnimateByName("tank/tank", 0.2f, 7);
		default:
			break;
	}
	auto repeatanimate = RepeatForever::create(animate);
	auto moveto = MoveTo::create(4.0f, pos);
	runAction(repeatanimate);
	auto callfunc = CallFunc::create([=] {
		stopAction(repeatanimate);
		switch (getType()) {
			case 'd':
				setTexture("unit/FighterUnit_1.png");
				break;
			case 's':
				setTexture("unit/FighterUnit_2.png");
				break;
			case 't':
				setTexture("unit/FighterUnit.png");
				break;
			default:
				break;
		}
	});
	auto sequence = Sequence::create(moveto, callfunc, NULL);
	runAction(sequence);
}*/
/*
void FighterUnitBase::attack(int id) {
	auto tempSprtie = TiledMap::getUnitById(id);
	auto tempAttack = this->getAttack();
	//TODO 新增一个基类
}*/