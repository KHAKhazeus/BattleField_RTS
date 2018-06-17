//BuildingBase.cpp
//Jiang
//18/5/30

#include "BuildingBase.h"
#include "FighterUnitBase.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

bool BuildingBase::_isbuilt = false;

bool BuildingBase::getIsBuilt() {
	return _isbuilt;
}

void BuildingBase::setIsBuilt(bool judge) {
	_isbuilt = judge;
}



Vec2 SoldierBase::RandomPosition() {
	int sign = random() % 3 - 1;
	int sign1 = random() % 3 - 1;
	while (sign == 0 && sign1 == 0) {
		sign = random() % 3 - 1;
		sign1 = random() % 3 - 1;
	}
	float randX = this->getPosition().x + random() % 100 * sign;
	float randY = this->getPosition().y + random() % 100 * sign1;
	Vec2 position = Vec2(randX, randY);
	// soldier's position transfered to tilemap position
	Vec2 tileCoord = static_cast<TiledMap*>(this->getParent()->getParent())->tileCoordForPosition(position);

	// if position is collidable,recall
	if (!TiledMap::checkCreate(tileCoord)) {
		return RandomPosition();
	}
	// if valid , return position
	return position;
}

Vec2 WarFactory::RandomPosition() {
	int sign = random() % 3 - 1;
	int sign1 = random() % 3 - 1;
	while (sign == 0 && sign1 == 0) {
		sign = random() % 3 - 1;
		sign1 = random() % 3 - 1;
	}
	float randX = this->getPosition().x + random() % 150*  sign;
	float randY = this->getPosition().y + random() % 150 * sign1;
	Vec2 position = Vec2(randX, randY);
	// tank's position transfered to tilemap position
	Vec2 tileCoord = static_cast<TiledMap*>(this->getParent()->getParent())->tileCoordForPosition(position);
	
	// if position is collidable,recall
	if (!TiledMap::checkCreate(tileCoord)) {
		return RandomPosition();
	}
	
	// if valid , return position
	return position;
}

//Build the building
void SoldierBase::Build() {
	auto animate =BuildingBase::getAnimateByName("soldierBase", 0.1f, 23);
	auto barSprite = Sprite::create("bar/loadingbar.png");
	this->setProgressed(false);
	ProgressTimer* progress = ProgressTimer::create(barSprite);
	progress->setPercentage(0.0f);
	progress->setScale(0.3f);
	progress->setMidpoint(Vec2(0.0f, 0.5f));
	progress->setBarChangeRate(Vec2(1.0f, 0.f));
	progress->setType(ProgressTimer::Type::BAR);
	Vec2 pos = Vec2(this->getPosition().x, this->getPosition().y);
	progress->setPosition(Vec2(120, 200));
	this->addChild(progress);
	auto pft = ProgressFromTo::create(2.3f, progress->getPercentage(), 100);
	this->runAction(animate);
	auto sequence = Sequence::create(pft,CallFunc::create([=] {
		this->removeChild(progress,true);
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(SoldierBase::onTouchBegan, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		this->setProgressed(true);
		SimpleAudioEngine::getInstance()->playEffect(CONSTRUCTION, false);
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
	loadingBar->setPosition(Vec2(140, -5));
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

	if (rect.containsPoint(locationInNode) && !Soldier::getIsCreated() && !Dog::getIsCreated()) {	//if click is valid
		if (getSelected() || getCreated()) {
			return false;
		}
		setSelected(true);
		//TODO change this to vector
		auto height = this->getContentSize().height ;
		auto cache = Director::getInstance()->getTextureCache();
		std::vector<Sprite*> temp_sprite;
		int size = 2;
		for (int i = 0; i < size; i++) {
			auto temp = Sprite::createWithTexture(cache->addImage(StringUtils::format("unit/FighterUnit_%d.png", i + 1)));
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
			listener->onTouchBegan = [temp, temp_sprite, this, i,cache](Touch *touch, Event *event) {
				auto target = static_cast<Sprite*>(event->getCurrentTarget());
				auto locationInNode = target->convertToNodeSpace(touch->getLocation());
				Size s = target->getContentSize();
				auto rect = Rect(0, 0, s.width, s.height);
				if (rect.containsPoint(locationInNode)) {
					if (this->getCreated()) {
						return false;
					}
					this->setCreated(true);
					auto temp_building = Sprite::createWithTexture(cache->addImage(StringUtils::format("unit/FighterUnit_%d.png", i + 1)));
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
						auto tempTiledMap = static_cast<TiledMap*>(this->getParent()->getParent());
						if (temp_building->getTag() == 1) {
							Dog* dog = Dog::create("dogRun/dog0.png");
							auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
							auto tempManager = tempScene->getUnitManager();
							if (tempScene->getMoney()->checkMoney(dog->getGold())) {
								auto id = dog->getIdCount();
								dog->setUnitID(id);
								Vec2 nodeLocation = this->RandomPosition();
								tempManager->addMessages(tempManager->msgs->newCreateUnitMessage(dog->getUnitID(), dog->getType(),this->getCampID(),
									this->getUnitID(), nodeLocation));
								/*UnitManager::NewUnitCreate(dog->getUnitID(), dog->getType(), this->getCampID(),
									this->getUnitID(), nodeLocation);*/
							}
							//delete dog;
						}
						else if (temp_building->getTag() == 2) {
							Soldier* soldier = Soldier::create("soldierRun/soldierstand.png");
							auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
							auto tempManager = tempScene->getUnitManager();
							if (tempScene->getMoney()->checkMoney(soldier->getGold())) {
								Vec2 nodeLocation = this->RandomPosition();
								auto id = soldier->getIdCount();
								soldier->setUnitID(id);
								tempManager->addMessages(tempManager->msgs->newCreateUnitMessage(soldier->getUnitID(), soldier->getType(), this->getCampID(),
									this->getUnitID(), nodeLocation));
								/*UnitManager::NewUnitCreate(soldier->getUnitID(), soldier->getType(), this->getCampID(),
									this->getUnitID(), nodeLocation);*/
							}
							//delete soldier;
						}
						auto tempNode = this->getParent()->getParent()->getParent();
						tempNode->removeChild(temp_building, true);
						this->setCreated(false);
					};
					Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener1, temp_building);
					return true;
				}
				else {
					this->removeChild(temp, true);
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
	if (rect.containsPoint(locationInNode) && !Tank::getIsCreated()) {	//if click is valid
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
					auto tempTiledMap = static_cast<TiledMap*>(this->getParent()->getParent());
					Tank* tank = Tank::create("tank/tank0.png");
					auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
					auto tempManager = tempScene->getUnitManager();
					if (tempScene->getMoney()->checkMoney(tank->getGold())) {
						Vec2 nodeLocation = this->RandomPosition();
						auto id =tank->getIdCount();
						tank->setUnitID(id);
						tempManager->addMessages(tempManager->msgs->newCreateUnitMessage(tank->getUnitID(), tank->getType(), this->getCampID(),
							this->getUnitID(), nodeLocation));
						/*UnitManager::NewUnitCreate(tank->getUnitID(), tank->getType(), this->getCampID(),
							this->getUnitID(), nodeLocation);*/
					}
					///delete tank;
					auto tempNode = this->getParent()->getParent()->getParent();
					tempNode->removeChild(temp_building, true);
					this->setCreated(false);
				};
				Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener1, temp_building);
				return true;
			}
			else {
				this->removeChild(temp, true);
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
	auto animate = BuildingBase::getAnimateByName("moneyMine", 0.2f, 24);
	auto barSprite = Sprite::create("bar/loadingbar.png");
	this->setProgressed(false);
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
		this->setProgressed(true);
		SimpleAudioEngine::getInstance()->playEffect(CONSTRUCTION, false);
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
	auto animate = BuildingBase::getAnimateByName("tankBase", 0.1f, 23);
	auto barSprite = Sprite::create("bar/loadingbar.png");
	this->setProgressed(false);
	ProgressTimer* progress = ProgressTimer::create(barSprite);
	progress->setPercentage(0.0f);
	progress->setScaleX(0.5f);
	progress->setScaleY(0.3f);
	progress->setMidpoint(Vec2(0.0f, 0.5f));
	progress->setBarChangeRate(Vec2(1.0f, 0.f));
	progress->setType(ProgressTimer::Type::BAR);
	Vec2 pos = Vec2(this->getPosition().x, this->getPosition().y);
	progress->setPosition(Vec2(120, 160));
	this->addChild(progress);
	auto pft = ProgressFromTo::create(2.3f, progress->getPercentage(), 100);
	this->runAction(animate);
	auto sequence = Sequence::create(pft, CallFunc::create([=] {
		this->removeChild(progress, true);
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(WarFactory::onTouchBegan,this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		this->setProgressed(true);
		SimpleAudioEngine::getInstance()->playEffect(CONSTRUCTION, false);
	}), CallFunc::create([]{BuildingBase::setIsBuilt(false); }), nullptr);
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
	loadingBar->setPosition(Vec2(120, -5));
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
	auto animate = PowerPlant::getAnimateByName("powerPlant", 0.1f, 23);
	auto barSprite = Sprite::create("bar/loadingbar.png");
	this->setProgressed(false);
	ProgressTimer* progress = ProgressTimer::create(barSprite);
	progress->setPercentage(0.0f);
	progress->setScale(0.3f);
	progress->setMidpoint(Vec2(0.0f, 0.5f));
	progress->setBarChangeRate(Vec2(1.0f, 0.f));
	progress->setType(ProgressTimer::Type::BAR);
	Vec2 pos = Vec2(this->getPosition().x, this->getPosition().y);
	progress->setPosition(Vec2(60, 100));
	this->addChild(progress);
	auto pft = ProgressFromTo::create(2.3f, progress->getPercentage(), 100);
	this->runAction(animate);
	auto sequence = Sequence::create(pft, CallFunc::create([=] {
		this->removeChild(progress, true);
		this->setProgressed(true);
		SimpleAudioEngine::getInstance()->playEffect(CONSTRUCTION, false);
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
	loadingBar->setPosition(Vec2(80, 80));
	// set Hp bar for soldier
	this->setHP(loadingBar);
	// set the HP bar as a child
	this->addChild(this->getHP(), 210);
	// set the HP interval
	this->setHPInterval(this->getHP()->getPercent() / static_cast<float>(this->getLifeValue()));
	loadingBar->setVisible(false);
}



