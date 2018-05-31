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

	isselected = false;
	iscreated = false;
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
	if (rect.containsPoint(locationInNode)) {	//if click is valid
		if (isselected || iscreated) { 
			return false;
		}
		isselected = true;
		//TODO change this to vector
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
					listener1->onTouchMoved = [temp_building](Touch *touch, Event *event) {
						
						auto target = static_cast<Sprite*>(event->getCurrentTarget());
						target->setPosition(target->getPosition() + touch->getDelta());
					};
					listener1->onTouchEnded = [this, temp_building](Touch *touch, Event *event) {
						//this->removeChild(temp_building, true);
						if (temp_building->getTag() == 1) {
							MoneyMine* moneyMine = MoneyMine::create("moneyMine/MinetoMoney_24.png");
							if (static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getMoney()->checkMoney(moneyMine->getGold()) &&
								static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getPower()->checkPower(moneyMine->getElect())) {
								auto touchLocation = touch->getLocation();
								Vec2 nodeLocation = this->getParent()->convertToNodeSpace(touchLocation);
								moneyMine->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
								moneyMine->Build();
								static_cast<TMXTiledMap*>(this->getParent())->addChild(moneyMine,50);
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
								auto touchLocation = touch->getLocation();
								Vec2 nodeLocation = this->getParent()->convertToNodeSpace(touchLocation);
								powerPlant->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
								powerPlant->Build();
								static_cast<TMXTiledMap*>(this->getParent())->addChild(powerPlant,50);
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
								auto touchLocation = touch->getLocation();
								Vec2 nodeLocation = this->getParent()->convertToNodeSpace(touchLocation);
								soldierBase->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
								soldierBase->Build();
								static_cast<TMXTiledMap*>(this->getParent())->addChild(soldierBase,50);
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
								auto touchLocation = touch->getLocation();
								Vec2 nodeLocation = this->getParent()->convertToNodeSpace(touchLocation);
								warFactory->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
								warFactory->Build();
								static_cast<TMXTiledMap*>(this->getParent())->addChild(warFactory,50);
								static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getVectorFactory().pushBack(warFactory);
								static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getPower()->spendPower(warFactory->getElect());
								static_cast<GameScene*>(this->getParent()->getParent()->getParent())->getMoney()->spendMoney(warFactory->getGold());
							}
							else {
								delete warFactory;
							}
						}
						auto tempNode = _base->getParent()->getParent()->getParent();
						tempNode->removeChild(temp_building,true);
						iscreated = false;
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

