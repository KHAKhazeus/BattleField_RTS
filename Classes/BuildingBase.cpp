//BuildingBase.cpp
//Jiang
//18/5/30

#include "BuildingBase.h"
#include "FighterUnitBase.h"
#include "GameScene.h"
#include "time.h"

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

Vec2 SoldierBase::RandomPosition() {
	srand((unsigned int)time(NULL));
	int sign = random() % 3 - 1;
	int sign1 = random() % 3 - 1;
	while (sign == 0 && sign1 == 0) {
		sign = random() % 3 - 1;
		sign1 = random() % 3 - 1;
	}
	float randX = this->getPosition().x + (55 + random() % 40)*  sign;
	float randY = this->getPosition().y + (55 + random() % 40) * sign1;
	Vec2 position = Vec2(randX, randY);
	auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
	Vector<Soldier*>soldiers = tempScene->getVectorSoldiers();
	Vector<Dog*>dogs = tempScene->getVectorDogs();
	Vector<Tank*>tanks = tempScene->getVectorTanks();
	for (unsigned int i = 0; i < soldiers.size(); i++) {
		if (abs(position.x - soldiers.at(i)->getPosition().x) < 10&&abs(position.y - soldiers.at(i)->getPosition().y) < 10) {
			return RandomPosition();
		}
	}
	for (unsigned int i = 0; i < dogs.size(); i++) {
		if (abs(position.x - dogs.at(i)->getPosition().x) < 10 && abs(position.y - dogs.at(i)->getPosition().y) < 10) {
			return RandomPosition();
		}
	}
	for (unsigned int i = 0; i < tanks.size(); i++) {
		if (abs(position.x - tanks.at(i)->getPosition().x) < 20 && abs(position.y - tanks.at(i)->getPosition().y) < 20) {
			return RandomPosition();
		}
	}
	// soldier's position transfered to tilemap position
	Vec2 tileCoord = static_cast<GameScene*>(this->getParent()->getParent()->getParent())->tileCoordForPosition(position);
	// if wrong position ,recall
	if (tileCoord.x < 0 || tileCoord.x >= static_cast<TMXTiledMap*>(this->getParent())->getMapSize().width
		|| tileCoord.y < 0 || tileCoord.y >= static_cast<TMXTiledMap*>(this->getParent())->getMapSize().height) {
		return RandomPosition();
	}
	// get TileMap position's GID
	int tileGid = static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getMap()->getCollLayer()->getTileGIDAt(tileCoord);
	// if position is collidable,recall
	if (tileGid) {
		// use GID to find tile's properties
		Value properties = static_cast<TMXTiledMap*>(this->getParent())->getPropertiesForGID(tileGid);
		// return Value is ValueMap
		ValueMap map = properties.asValueMap();
		// search ValueMap£¬judge if there are collidable objects
		std::string value = map.at("collidable").asString();
		if (value.compare("true") == 0) {
			return RandomPosition();
		}
		else {
			return position;
		}
	}
	// if valid , return position
	return position;
}

Vec2 WarFactory::RandomPosition() {
	srand((unsigned int)time(NULL));
	int sign = random() % 3 - 1;
	int sign1 = random() % 3 - 1;
	while (sign == 0 && sign1 == 0) {
		sign = random() % 3 - 1;
		sign1 = random() % 3 - 1;
	}
	float randX = this->getPosition().x + (this->getContentSize().width / 3 + random() % 50)*  sign;
	float randY = this->getPosition().y + (this->getContentSize().height / 3 + random() % 50) * sign1;
	Vec2 position = Vec2(randX, randY);
	auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
	Vector<Soldier*>soldiers = tempScene->getVectorSoldiers();
	Vector<Dog*>dogs = tempScene->getVectorDogs();
	Vector<Tank*>tanks = tempScene->getVectorTanks();
	for (unsigned int i = 0; i < soldiers.size(); i++) {
		if (abs(position.x - soldiers.at(i)->getPosition().x) < 10 && abs(position.y - soldiers.at(i)->getPosition().y) < 10) {
			return RandomPosition();
		}
	}
	for (unsigned int i = 0; i < dogs.size(); i++) {
		if (abs(position.x - dogs.at(i)->getPosition().x) < 10 && abs(position.y - dogs.at(i)->getPosition().y) < 10) {
			return RandomPosition();
		}
	}
	for (unsigned int i = 0; i < tanks.size(); i++) {
		if (abs(position.x - tanks.at(i)->getPosition().x) < 20 && abs(position.y - tanks.at(i)->getPosition().y) < 20) {
			return RandomPosition();
		}
	}
	// soldier's position transfered to tilemap position
	Vec2 tileCoord = static_cast<GameScene*>(this->getParent()->getParent()->getParent())->tileCoordForPosition(position);
	// if wrong position ,recall
	if (tileCoord.x < 0 || tileCoord.x >= static_cast<TMXTiledMap*>(this->getParent())->getMapSize().width
		|| tileCoord.y < 0 || tileCoord.y >= static_cast<TMXTiledMap*>(this->getParent())->getMapSize().height) {
		return RandomPosition();
	}
	// get TileMap position's GID
	int tileGid = static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getMap()->getCollLayer()->getTileGIDAt(tileCoord);
	// if position is collidable,recall
	if (tileGid) {
		// use GID to find tile's properties
		Value properties = static_cast<TMXTiledMap*>(this->getParent())->getPropertiesForGID(tileGid);
		// return Value is ValueMap
		ValueMap map = properties.asValueMap();
		// search ValueMap£¬judge if there are collidable objects
		std::string value = map.at("collidable").asString();
		if (value.compare("true") == 0) {
			return RandomPosition();
		}
		else {
			return position;
		}
	}
	// if valid , return position
	return position;
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

	// create a loading bar
	auto loadingBar = LoadingBar::create("bar/planeHP.png");
	loadingBar->setScaleX(0.25f);
	loadingBar->setScaleY(0.1f);
	// set the percentage
	loadingBar->setPercent(60);
	// set direction
	loadingBar->setDirection(LoadingBar::Direction::LEFT);
	// set position
	loadingBar->setPosition(Vec2(170, -15));
	// set Hp bar for soldier
	this->setHP(loadingBar);
	// set the HP bar as a child
	this->addChild(this->getHP(), 210);
	// set the HP interval
	this->setHPInterval(this->getHP()->getPercent() / (float)this->getLifeValue());
	loadingBar->setVisible(false);

}
bool SoldierBase::onTouchBegan(Touch *touch, Event *event) {
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	auto rect = Rect(0, 0, s.width, s.height);
	this->setSelected(false);
	this->setCreated(false);
	if (rect.containsPoint(locationInNode)) {	//if click is valid
		if (this->getSelected() || this->getCreated()) {
			return false;
		}
		this->setSelected(true);
		//TODO change this to vector
		auto height = this->getContentSize().height ;
		std::vector<Sprite*> temp_sprite;
		int size = 2;
		for (int i = 0; i < size; i++) {
			auto temp = Sprite::create(StringUtils::format("unit/FighterUnit_%d.png", i + 1));
			//make notes: do a little change to the position of the small icons
			auto width = temp->getContentSize().width / 2;
			temp->setScale(1.0);
			temp->setColor(Color3B(0, 255, 0));
			//devided it into size(a num) pieces
			temp->setPosition(width * 3 * static_cast<float> (i) / size+150,
				height-10);
			temp_sprite.push_back(temp);
			this->addChild(temp);
		}
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		//Click the small icon
		for (auto i = 0; i < temp_sprite.size(); i++) {
			auto temp = temp_sprite.at(i);
			listener->onTouchBegan = [temp, temp_sprite, this, i](Touch *touch, Event *event) {
				auto target = static_cast<Sprite*>(event->getCurrentTarget());
				auto locationInNode = target->convertToNodeSpace(touch->getLocation());
				Size s = target->getContentSize();
				auto rect = Rect(0, 0, s.width, s.height);
				if (rect.containsPoint(locationInNode)) {
					if (this->getCreated()) {
						return false;
					}
					this->setCreated(true);
					auto temp_building = Sprite::create(StringUtils::format("unit/FighterUnit_%d.png", i + 1));
					temp_building->setTag(i + 1);
					/*
					make notes:
					tempNode below is a pointer to the TiledMap Layer
					get the world coordination of temp to set the Positon of the temp_building
					*/
					auto tempNode = this->getParent()->getParent()->getParent();
					auto pos = this->convertToWorldSpace(temp->getPosition());
					temp_building->setColor(Color3B(255, 0, 0));
					temp_building->setPosition(pos);
					tempNode->addChild(temp_building, 200);
					auto listener1 = EventListenerTouchOneByOne::create();
					listener1->setSwallowTouches(true);
					listener1->onTouchBegan = [temp_building](Touch *touch, Event *event) {
						auto touchLocation = touch->getLocation();
						auto node = (Node*)temp_building;
						auto rect = Rect(0, 0,
							temp_building->getContentSize().width, temp_building->getContentSize().height);
						if (rect.containsPoint(node->convertToNodeSpace(touchLocation))) {
							return true;
						}
					};
					listener1->onTouchEnded = [this, temp_building](Touch *touch, Event *event) {
						//this->removeChild(temp_building, true);
						if (temp_building->getTag() == 1) {
							Dog* dog = Dog::create("dogRun/dog0.png");
							auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
							if (tempScene->getMoney()->checkMoney(dog->getGold())) {
								Vec2 nodeLocation = this->RandomPosition();
								if (nodeLocation.x < this->getPosition().x) {
									dog->setFlippedX(true);
								}
								dog->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
								dog->Create(this);
								static_cast<TMXTiledMap*>(this->getParent())->addChild(dog, 200);
								tempScene->getVectorDogs().pushBack(dog);
								tempScene->getMoney()->spendMoney(dog->getGold());
							}
							else {
								delete dog;
							}
						}
						else if (temp_building->getTag() == 2) {
							Soldier* soldier = Soldier::create("soldierRun/soldierstand.png");
							auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
							if (tempScene->getMoney()->checkMoney(soldier->getGold())) {
								Vec2 nodeLocation = this->RandomPosition();
								if (nodeLocation.x < this->getPosition().x) {
									soldier->setFlippedX(true);
								}
								soldier->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
								soldier->Create(this);
								static_cast<TMXTiledMap*>(this->getParent())->addChild(soldier, 200);
								tempScene->getVectorSoldiers().pushBack(soldier);
								tempScene->getMoney()->spendMoney(soldier->getGold());

							}
							else {
								delete soldier;
							}
						}
						auto tempNode = this->getParent()->getParent()->getParent();
						tempNode->removeChild(temp_building, true);
						this->setCreated(false);
					};
					Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener1, temp_building);
					return true;
				}
				return false;
			};
			//if click is ended,remove the sprite created
			listener->onTouchEnded = [this, temp_sprite](Touch *touch, Event *event) {
				for (int i = 0; i < temp_sprite.size(); i++) {
					this->removeChild(temp_sprite.at(i), true);

					//Debug		log("%d", temp_sprite.size());
				}
				this->setSelected(false);
			};
			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), temp);
		}


		return true;
	}
	return false;
}

bool WarFactory::onTouchBegan(Touch *touch, Event *event) {
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	auto rect = Rect(0, 0, s.width, s.height);
	this->setSelected(false);
	this->setCreated(false);
	if (rect.containsPoint(locationInNode)) {	//if click is valid
		if (this->getSelected() || this->getCreated()) {
			return false;
		}
		this->setSelected(true);
		//TODO change this to vector
		auto height = this->getContentSize().height;
		auto temp = Sprite::create("unit/FighterUnit.png");
		//make notes: do a little change to the position of the small icons
		auto width = temp->getContentSize().width / 2;
		temp->setScale(0.4f);
		temp->setColor(Color3B(0, 255, 0));
		//devided it into size(a num) pieces
		temp->setPosition(width+100,
			height - 80);
		this->addChild(temp);
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		//Click the small icon
		listener->onTouchBegan = [temp, this](Touch *touch, Event *event) {
			auto target = static_cast<Sprite*>(event->getCurrentTarget());
			auto locationInNode = target->convertToNodeSpace(touch->getLocation());
			Size s = target->getContentSize();
			auto rect = Rect(0, 0, s.width, s.height);
			if (rect.containsPoint(locationInNode)) {
				if (this->getCreated()) {
					return false;
				}
				this->setCreated(true);
				auto temp_building = Sprite::create("unit/FighterUnit.png");
				/*
				make notes:
				tempNode below is a pointer to the TiledMap Layer
				get the world coordination of temp to set the Positon of the temp_building
				*/
				auto tempNode = this->getParent()->getParent()->getParent();
				auto pos = this->convertToWorldSpace(temp->getPosition());
				temp_building->setColor(Color3B(255, 0, 0));
				temp_building->setScale(0.4f);
				temp_building->setPosition(pos);
				tempNode->addChild(temp_building, 200);
				auto listener1 = EventListenerTouchOneByOne::create();
				listener1->setSwallowTouches(true);
				listener1->onTouchBegan = [temp_building](Touch *touch, Event *event) {
					auto touchLocation = touch->getLocation();
					auto node = (Node*)temp_building;
					auto rect = Rect(0, 0,
						temp_building->getContentSize().width, temp_building->getContentSize().height);
					if (rect.containsPoint(node->convertToNodeSpace(touchLocation))) {
						return true;
					}
				};
				listener1->onTouchEnded = [this, temp_building](Touch *touch, Event *event) {
					//this->removeChild(temp_building, true);
					Tank* tank = Tank::create("tank/tank0.png");
					auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
					if (tempScene->getMoney()->checkMoney(tank->getGold())) {
						Vec2 nodeLocation = this->RandomPosition();
						tank->setScale(0.4f);
						tank->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
						tank->Create(this);
						static_cast<TMXTiledMap*>(this->getParent())->addChild(tank, 200);
						tempScene->getVectorTanks().pushBack(tank);
						tempScene->getMoney()->spendMoney(tank->getGold());
					}
					else {
						delete tank;
					}
					auto tempNode = this->getParent()->getParent()->getParent();
					tempNode->removeChild(temp_building, true);
					this->setCreated(false);
				};
				Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener1, temp_building);
				return true;
			}
			return false;
		};
		//if click is ended,remove the sprite created
		listener->onTouchEnded = [this, temp](Touch *touch, Event *event) {
			this->removeChild(temp, true);

			//Debug		log("%d", temp_sprite.size());
			this->setSelected(false);
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), temp);


		return true;
	}
	return false;
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

	// create a loading bar
	auto loadingBar = LoadingBar::create("bar/planeHP.png");
	loadingBar->setScaleX(0.3f);
	loadingBar->setScaleY(0.1f);
	// set the percentage
	loadingBar->setPercent(60);
	// set direction
	loadingBar->setDirection(LoadingBar::Direction::LEFT);
	// set position
	loadingBar->setPosition(Vec2(150, 160));
	// set Hp bar for soldier
	this->setHP(loadingBar);
	// set the HP bar as a child
	this->addChild(this->getHP(), 210);
	// set the HP interval
	this->setHPInterval(this->getHP()->getPercent() / (float)this->getLifeValue());
	loadingBar->setVisible(false);
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
	progress->setPosition(Vec2(200, 160));
	this->addChild(progress);
	auto pft = ProgressFromTo::create(2.3f, progress->getPercentage(), 100);
	this->runAction(animate);
	auto sequence = Sequence::create(pft, CallFunc::create([=] {
		this->removeChild(progress, true);
	}), CallFunc::create([] {BuildingBase::setIsBuilt(false); }), nullptr);
	progress->runAction(sequence);

	// create a loading bar
	auto loadingBar = LoadingBar::create("bar/planeHP.png");
	loadingBar->setScaleX(0.3f);
	loadingBar->setScaleY(0.1f);
	// set the percentage
	loadingBar->setPercent(60);
	// set direction
	loadingBar->setDirection(LoadingBar::Direction::LEFT);
	// set position
	loadingBar->setPosition(Vec2(180, -15));
	// set Hp bar for soldier
	this->setHP(loadingBar);
	// set the HP bar as a child
	this->addChild(this->getHP(), 210);
	// set the HP interval
	this->setHPInterval(this->getHP()->getPercent() / (float)this->getLifeValue());
	loadingBar->setVisible(false);
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
	progress->setPosition(Vec2(130, 120));
	this->addChild(progress);
	auto pft = ProgressFromTo::create(2.3f, progress->getPercentage(), 100);
	this->runAction(animate);
	auto sequence = Sequence::create(pft, CallFunc::create([=] {
		this->removeChild(progress, true);
	}), CallFunc::create([] {BuildingBase::setIsBuilt(false); }), nullptr);
	progress->runAction(sequence);

	// create a loading bar
	auto loadingBar = LoadingBar::create("bar/planeHP.png");
	loadingBar->setScaleX(0.2f);
	loadingBar->setScaleY(0.1f);
	// set the percentage
	loadingBar->setPercent(60);
	// set direction
	loadingBar->setDirection(LoadingBar::Direction::LEFT);
	// set position
	loadingBar->setPosition(Vec2(160, 120));
	// set Hp bar for soldier
	this->setHP(loadingBar);
	// set the HP bar as a child
	this->addChild(this->getHP(), 210);
	// set the HP interval
	this->setHPInterval(this->getHP()->getPercent() / (float)this->getLifeValue());
	loadingBar->setVisible(false);
}



