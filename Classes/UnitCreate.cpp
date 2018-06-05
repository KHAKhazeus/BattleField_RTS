#include "UnitCreate.h"
#include "GameScene.h"
Base* Base::create() {
	auto base = new Base();
	if (base && base->init()) {
		base->autorelease();
		return base;
	}
	CC_SAFE_DELETE(base);
	return NULL;
}

bool Base::init() {
	if(!Sprite::init()) {
		return false;
	}
	_base = Sprite::create("unit/base_28.png");
	_base->setPosition(0,0);
	_min_range = 3;
	_max_range = 30;
	isselected = false;
	iscreated = false;
	_isbuilt = false;
	//temp_sprite->setPosition()
	this->addChild(_base);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Base::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(Base::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _base);

	return true;
}
//Click the Base building
//TODO judge if can create it (money,power,grid)
//TODO if can't send out warning
//TODO the animation of creating


bool Base::onTouchBegan(Touch *touch, Event *event) {
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	auto rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode) && !BuildingBase::getIsBuilt()) {	//if click is valid
		if (isselected || iscreated) { 
			return false;
		}
		isselected = true;
		auto height = this->getPosition().y * 0.3;
		std::vector<Sprite *> temp_sprite;
		int size = 4;
		for (int i = 0; i < size; i++) {
			auto temp = Sprite::create(StringUtils::format("unit/building_%d.png",i+1));
			//make notes: do a little change to the position of the small icons
			auto width = -temp->getContentSize().width/2;
			temp->setScale(0.3);
			//devided it into size(a num) pieces
			temp->setPosition( width - width* 2 * static_cast<float> (i) / size,
				height);
			temp_sprite.push_back(temp);
			this->addChild(temp);
		}
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		//Click the small icon
		for (auto i = 0; i < temp_sprite.size(); i++) {
			auto temp = temp_sprite.at(i);
			listener->onTouchBegan = [temp,temp_sprite,this,i](Touch *touch, Event *event) {
				auto target = static_cast<Sprite*>(event->getCurrentTarget());
				auto locationInNode = target->convertToNodeSpace(touch->getLocation());
				
				Size s = target->getContentSize();
				auto rect = Rect(0, 0, s.width, s.height);
				if (rect.containsPoint(locationInNode)) {
					if (iscreated) {
						return false;
					}
					iscreated = true;
					auto temp_building = Sprite::create(StringUtils::format("unit/building_%d.png", i + 1));
					temp_building->setTag(i + 1);
					/*
						make notes:
						tempNode below is a pointer to the TiledMap Layer
						get the world coordination of temp to set the Positon of the temp_building
					*/
					auto tempNode = _base->getParent()->getParent()->getParent();
					auto pos = this->convertToWorldSpace(temp->getPosition());
					temp_building->setOpacity(50);
					temp_building->setPosition(pos);
					tempNode->addChild(temp_building,200);
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
					listener1->onTouchMoved = [temp_building,this](Touch *touch, Event *event) {
						
						auto target = static_cast<Sprite*>(event->getCurrentTarget());
						auto pos = target->getPosition() + touch->getDelta();
						target->setPosition(pos);
						Vec2 nodeLocation = this->getParent()->convertToNodeSpace(pos);
						auto nodeBase = this->getPosition();
						auto tiledLocation = static_cast<TiledMap*>(this->getParent()->getParent())->tileCoordForPosition(nodeLocation);
						temp_building->setOpacity(75);
						//get the tiled coordinate of Base
						auto tiledBase = static_cast<TiledMap*>(this->getParent()->getParent())->tileCoordForPosition(nodeBase);
						//DEBUG 越界问题
						bool judgeBoundry = true;
						if (tiledLocation.x < tiledBase.x - this->getMaxRange() ||
							tiledLocation.x >= tiledBase.x + this->getMaxRange() ||
							tiledLocation.y < tiledBase.y - this->getMaxRange()||
							tiledLocation.y >= tiledBase.y + this->getMaxRange()) {
							judgeBoundry = false;
						}
						if (TiledMap::checkBuilt(tiledLocation, 3) && judgeBoundry) {
							temp_building->setColor(Color3B(152, 251, 152));
							this->setBuilt(true);

						}
						else {
							temp_building->setColor(Color3B(255, 99, 71));
							this->setBuilt(false);
						}

					};
					listener1->onTouchEnded = [this, temp_building](Touch *touch, Event *event) {
						auto touchLocation = touch->getLocation();
						Vec2 nodeLocation = this->getParent()->convertToNodeSpace(touchLocation);
						auto tiledLocation = static_cast<TiledMap*>(this->getParent()->getParent())->tileCoordForPosition(nodeLocation);
						// if the field isn't occupied
						if (this->getBuilt()) {
							this->setBuilt(false);
							if (temp_building->getTag() == 1) {
								MoneyMine* moneyMine = MoneyMine::create("moneyMine/MinetoMoney_24.png");
								if (static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getMoney()->checkMoney(moneyMine->getGold()) &&
									static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getPower()->checkPower(moneyMine->getElect())) {
									moneyMine->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
									BuildingBase::setIsBuilt(true);
									moneyMine->Build();
									TiledMap::setUnpass(tiledLocation, moneyMine->getRange());
									static_cast<TMXTiledMap*>(this->getParent())->addChild(moneyMine, 50);
									static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getVectorMine().pushBack(moneyMine);
									static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getPower()->spendPower(moneyMine->getElect());
									static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getMoney()->spendMoney(moneyMine->getGold());
								}
								else {
									delete moneyMine;
								}
							}
							else if (temp_building->getTag() == 2) {
								PowerPlant* powerPlant = PowerPlant::create("powerPlant/PowerBuilt_24.png");
								if (static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getMoney()->checkMoney(powerPlant->getGold())) {
									powerPlant->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
									BuildingBase::setIsBuilt(true);
									powerPlant->Build();
									TiledMap::setUnpass(tiledLocation, powerPlant->getRange());
									static_cast<TMXTiledMap*>(this->getParent())->addChild(powerPlant, 50);
									static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getVectorPower().pushBack(powerPlant);
									static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getPower()->increasePower((powerPlant->getElect()));
									static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getMoney()->spendMoney(powerPlant->getGold());
								}
								else {
									delete powerPlant;
								}
							}
							else if (temp_building->getTag() == 3) {
								SoldierBase* soldierBase = SoldierBase::create("soldierBase/soldierBase_23.png");
								if (static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getMoney()->checkMoney(soldierBase->getGold()) &&
									static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getPower()->checkPower(soldierBase->getElect())) {
									BuildingBase::setIsBuilt(true);
									soldierBase->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
									soldierBase->Build();
									TiledMap::setUnpass(tiledLocation, soldierBase->getRange());
									static_cast<TMXTiledMap*>(this->getParent())->addChild(soldierBase, 50);
									static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getVectorSoldier().pushBack(soldierBase);
									static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getPower()->spendPower(soldierBase->getElect());
									static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getMoney()->spendMoney(soldierBase->getGold());
								}
								else {
									delete soldierBase;
								}
							}
							else if (temp_building->getTag() == 4) {
								WarFactory* warFactory = WarFactory::create("tankBase/tankbuilding_23.png");
								if (static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getMoney()->checkMoney(warFactory->getGold()) &&
									static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getPower()->checkPower(warFactory->getElect())) {
									BuildingBase::setIsBuilt(true);
									warFactory->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
									warFactory->Build();
									TiledMap::setUnpass(tiledLocation, warFactory->getRange());
									static_cast<TMXTiledMap*>(this->getParent())->addChild(warFactory, 50);
									static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getVectorFactory().pushBack(warFactory);
									static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getPower()->spendPower(warFactory->getElect());
									static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getMoney()->spendMoney(warFactory->getGold());
								}
								else {
									delete warFactory;
								}
							}
						}
						iscreated = false;
						auto tempNode = _base->getParent()->getParent()->getParent();
						tempNode->removeChild(temp_building, true);
					};
					Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener1, temp_building);
					return true;
				}
				return false;
			};
			//if click is ended,remove the sprite created
			listener->onTouchEnded = [this, temp_sprite](Touch *touch, Event *event) {
				for (int i = 0; i < temp_sprite.size(); i++) {
					this->removeChild(temp_sprite.at(i),true);
					
			//Debug		log("%d", temp_sprite.size());
				}
				isselected = false;
			};
			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(),temp);
		}
		
		
		return true;
	}
	return false;
}

void Base::onTouchEnded(Touch *touch, Event *event) {

}

Sprite* Base::getSprite() {
	return _base;
}

int Base::getMinRange() {
	return _min_range;
}

int Base::getMaxRange() {
	return _max_range;
}

void Base::setBuilt(bool judge) {
	_isbuilt = judge;
}

bool Base::getBuilt() {
	return _isbuilt;
}