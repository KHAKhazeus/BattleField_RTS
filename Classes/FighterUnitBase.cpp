//FighterUnitBase.cpp
//Jiang
//18/6/1

#include "FighterUnitBase.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
#include "GameScene.h"

bool Soldier::_redIsBeingCreated = false;
bool Soldier::_blueIsBeingCreated = false;
bool Soldier::getRedIsCreated() {
	return _redIsBeingCreated;
}

void Soldier::setRedIsCreated(bool judge) {
	_redIsBeingCreated = judge;
}

bool Soldier::getBlueIsCreated() {
	return _blueIsBeingCreated;
}

void Soldier::setBlueIsCreated(bool judge) {
	_blueIsBeingCreated = judge;
}

bool Dog::_redIsBeingCreated = false;
bool Dog::_blueIsBeingCreated = false;
bool Dog::getRedIsCreated() {
	return _redIsBeingCreated;
}

void Dog::setRedIsCreated(bool judge) {
	_redIsBeingCreated = judge;
}

bool Dog::getBlueIsCreated() {
	return _blueIsBeingCreated;
}

void Dog::setBlueIsCreated(bool judge) {
	_blueIsBeingCreated = judge;
}

bool Tank::_redIsBeingCreated = false;
bool Tank::_blueIsBeingCreated = false;
bool Tank::getRedIsCreated() {
	return _redIsBeingCreated;
}

void Tank::setRedIsCreated(bool judge) {
	_redIsBeingCreated = judge;
}

bool Tank::getBlueIsCreated() {
	return _blueIsBeingCreated;
}

void Tank::setBlueIsCreated(bool judge) {
	_blueIsBeingCreated = judge;
}

void Soldier::Create(Unit* soldierBase) {
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
	auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
	auto tiledLocation = tempScene->tileCoordForPosition(this->getPosition());
	TiledMap::newMapGrid(tiledLocation, this->getUnitID());
	TiledMap::newMapId(this->getUnitID(), this);
	auto pft = ProgressFromTo::create(2.4f, progress->getPercentage(), 100);
	auto sequence = Sequence::create(pft, CallFunc::create([=] {
		soldierBase->removeChild(progress, true);
		this->setVisible(true);
		if (this->getCampID() == REDCAMP) {
			Soldier::setRedIsCreated(false);
		}
		else {
			Soldier::setBlueIsCreated(false);
		}
		auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
		auto tempManager = tempScene->getUnitManager();
		if (this->getCampID() == tempManager->_myCamp) {
			SimpleAudioEngine::getInstance()->playEffect(SOLDIER, false);
		}
		
		if (this->getCampID() == tempManager->_myCamp) {
			this->schedule(schedule_selector(FighterUnitBase::autoAttack), 1);
		}
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

void Dog::Create(Unit* soldierBase) {
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
	auto tempScene = static_cast<TiledMap*>(this->getParent()->getParent());
	auto tiledLocation = tempScene->tileCoordForPosition(this->getPosition());
	TiledMap::newMapGrid(tiledLocation, this->getUnitID());
	TiledMap::newMapId(this->getUnitID(), this);
	auto sequence = Sequence::create(pft, CallFunc::create([=] {
		soldierBase->removeChild(progress, true);
		this->setVisible(true);
		if (this->getCampID() == REDCAMP) {
			Dog::setRedIsCreated(false);
		}
		else {
			Dog::setBlueIsCreated(false);
		}
		auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
		auto tempManager = tempScene->getUnitManager();
		if (this->getCampID() == tempManager->_myCamp) {
			SimpleAudioEngine::getInstance()->playEffect(DOG, false);
		}
		
		if (this->getCampID() == tempManager->_myCamp) {
			this->schedule(schedule_selector(FighterUnitBase::autoAttack), 1);
		}
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

void Tank::Create(Unit* warFactory) {
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
	auto tempScene = static_cast<TiledMap*>(this->getParent()->getParent());
	auto tiledLocation = tempScene->tileCoordForPosition(this->getPosition());
	TiledMap::newMapGrid(tiledLocation, this->getUnitID());
	TiledMap::newMapId(this->getUnitID(), this);
	auto sequence = Sequence::create(pft, CallFunc::create([=] {
		warFactory->removeChild(progress, true);
		this->setVisible(true);
		if (this->getCampID() == REDCAMP) {
			Tank::setRedIsCreated(false);
		}
		else {
			Tank::setBlueIsCreated(false);
		}
		auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
		auto tempManager = tempScene->getUnitManager();
		if (this->getCampID() == tempManager->_myCamp) {
			SimpleAudioEngine::getInstance()->playEffect(TANK, false);
		}
		
		if(this->getCampID() == tempManager->_myCamp) {
			this->schedule(schedule_selector(FighterUnitBase::autoAttack), 1);
		}
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
	if (this->getCampID() == REDCAMP) {
		loadingBar->setPosition(Vec2(90, 150));
	}
	else {
		loadingBar->setPosition(Vec2(40, 70));
	}
	// set Hp bar for soldier
	this->setHP(loadingBar);
	// set the HP bar as a child
	this->addChild(this->getHP(), 210);
	// set the HP interval
	this->setHPInterval(this->getHP()->getPercent() / (float)this->getLifeValue());
	loadingBar->setVisible(false);
}

bool FighterUnitBase::judgeAttack(Vec2 myPos,Vec2 pos) {
	auto distance = sqrt(pow(myPos.x - pos.x, 2) + pow(myPos.y - pos.y, 2));
	if (distance < this->getAttackRange()) {
		return true;
	}
	return false;
}

Vec2 FighterUnitBase::searchEnemy() {
	auto curpos = getTiledPosition();
	auto range = getAttackRange();
	auto rect = GridRect::create(curpos.x - range/2 , curpos.y - range/2 , range, range);
	log("%f %f %d %d", curpos.x - range / 2, curpos.y - range / 2, range, range);
	for (auto i = rect->getX(); i <= rect->getX() + rect->getWidth(); i++) {
		for (auto j = rect->getY(); j <= rect->getY() + rect->getHeight(); j++) {
			auto vecPos = Vec2(i, j);
			if (TiledMap::checkBoundary(vecPos)) {
				if (TiledMap::checkMapGrid(vecPos)) {
					auto id = TiledMap::getUnitIdByPosition(vecPos);
					if (id == this->getUnitID()) {
						continue;
					}
					if (!TiledMap::checkUnitId(id)) {
						continue;
					}
					auto temp = TiledMap::getUnitById(id);
					if (temp->getTiledPosition() != vecPos) {
						auto tempPos = Vec2(temp->getTiledPosition().x, temp->getTiledPosition().y);
						if (!TiledMap::checkMapGrid(tempPos)) {
							TiledMap::updateMapGrid(vecPos, tempPos);
						}
						else {
							continue;
						}
					}
					if (temp->getCampID() != this->getCampID()) {
						return vecPos;
					}
				}
			}
		}
	}
	return Vec2(-1,-1);
}

bool FighterUnitBase::judgeTarPos() {
	auto tarPos = this->getTargetPos();
	auto enemyId = this->getTargetID();
	auto enemy = TiledMap::getUnitById(enemyId);
	auto enemyPos = enemy->getTiledPosition();
	auto distance = sqrt(pow(tarPos.x - enemyPos.x, 2) + pow(tarPos.y - enemyPos.y, 2));
	if (distance < this->getAttackRange()) {
		return true;
	}
	return false;
}



void FighterUnitBase::autoAttack(float dt) {
	if (isAttack()) {
		auto m = this;
		this->setAutoAttack(false);
		if (!TiledMap::checkUnitId(this->getTargetID())) {
			this->clearAllType();
			this->setAutoAttack(true);
			return;
		}
		auto enemy = TiledMap::getUnitById(this->getTargetID());
		auto pos = enemy->getTiledPosition();
		auto tempNode = this->getParent()->getParent()->getParent();
		auto tempScene = static_cast<GameScene*>(tempNode);
		auto tempManager = tempScene->getUnitManager();
		//judge if the enemy is in the range
		//Attack
		if (this->judgeAttack(this->getTiledPosition(), pos)) {
			//send attack message
			tempManager->addMessages(tempManager->msgs->newAttackMessage(this->getUnitID(), enemy->getUnitID(), this->getAttack()));
		}
		else {
				if (this->getType() == "d" && enemy->isBuilding()) {
					return;
				}
				if (this->isMove()) {
					return;
				}
				std::vector<Vec2> path = { pos };
				//send tracing message
				tempManager->addMessages(tempManager->msgs->newMoveMessage(this->getUnitID(), path, pos));
			}
		}
	else if (isAutoAttack()) {
		auto pos = searchEnemy();
		if (pos.x != -1) {
			this->setMove(false);
			auto tempNode = this->getParent()->getParent()->getParent();
			auto tempScene = static_cast<GameScene*>(tempNode);
			auto tempManager = tempScene->getUnitManager();
			auto id = TiledMap::getUnitIdByPosition(pos);
			if (!TiledMap::checkUnitId(id)) {
				return;
			}
			auto enemy = TiledMap::getUnitById(id);
			//send attack message
			tempManager->addMessages(tempManager->msgs->newAttackMessage(this->getUnitID(), enemy->getUnitID(), this->getAttack()));
		}
	}	
}

