#include "UnitCreate.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

Base* Base::create(int campID) {
	auto base = new Base();
	if (base && base->init(campID)) {
		base->autorelease();
		return base;
	}
	CC_SAFE_DELETE(base);
	return NULL;
}

bool Base::init(int campID) {
	if (!Sprite::init()) {
		return false;
	}
	setCampID(campID);
	auto cache = Director::getInstance()->getTextureCache();
	_base = Unit::create("unit/base_28.png");
	// create a loading bar
	if (this->getCampID() == REDCAMP) {
		_base->setColor(Color3B(221, 160, 221));
	}
	else {
		_base->setColor(Color3B(65, 105, 225));
	}
	auto loadingBar = LoadingBar::create("bar/planeHP.png");
	loadingBar->setScaleX(0.4f);
	loadingBar->setScaleY(0.1f);
	// set the percentage
	loadingBar->setPercent(60);
	// set direction
	loadingBar->setDirection(LoadingBar::Direction::LEFT);
	// set position
	loadingBar->setPosition(Vec2(90, -100));
	// set Hp bar for soldier
	this->setHP(loadingBar);
	// set the HP bar as a child
	this->addChild(this->getHP(), 210);
	// set the HP interval
	this->setHPInterval(this->getHP()->getPercent() / (float)this->getLifeValue());
	loadingBar->setVisible(false);
	_base->setPosition(0, 0);
	//
	setIsBuilding(true);


	setRange(2);
	setFixModel(FIX_SQUARE);
	_max_range = 30;
	setBuilt(false);
	setSelected(false);
	setCreated(false);
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
	auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
	auto tempManager = tempScene->getUnitManager();
	if (this->getCampID() != tempManager->_myCamp) {
		return false;
	}
	auto cache = Director::getInstance()->getTextureCache();
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	auto rect = Rect(s.width / 4, s.height / 4, s.width / 2, s.height / 2);

	if (rect.containsPoint(locationInNode)) {	//if click is valid
		if (tempManager->_myCamp == REDCAMP) {
			if (BuildingBase::getRedIsBuilt()) {
				return false;
			}
		}
		else {
			if (BuildingBase::getBlueIsBuilt()) {
				return false;
			}
		}
		if (getSelected() || isCreated()) { 
			return false;
		}
		setSelected(true);

		//TODO change this to vector
		auto height = this->getContentSize().height;
		//auto height = this->getPosition().y * 0.3;
		std::vector<Sprite *> temp_sprite;
		std::vector<bool> boolean_tag;
		int size = 4;
		for (int i = 0; i < size; i++) {
			auto temp = Sprite::createWithTexture(cache->addImage(StringUtils::format("unit/building_%d.png",i+1)));
			//make notes: do a little change to the position of the small icons
			auto width = -temp->getContentSize().width/2;
			temp->setScale(0.3);
			//devided it into size(a num) pieces
			/*temp->setPosition(width - width* 3 * static_cast<float> (i) / size,
				height);*/
			temp->setPosition(width - width * 3 * static_cast<float> (i) / size,
				height+100);
			temp_sprite.push_back(temp);
			this->addChild(temp);
		}
		auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
		auto money = tempScene->getMoney()->getMoney();
		auto elect = tempScene->getPower()->getPower();
		if (money >= MONEYMINE_GOLD && elect >= MONEYMINE_ELECT) {
			boolean_tag.push_back(true);

		}
		else {
			boolean_tag.push_back(false);
			temp_sprite.at(0)->setColor(Color3B(255, 0, 0));
			temp_sprite.at(0)->setOpacity(50);
		}
		if (money >= POWERPLANT_GOLD) {
			boolean_tag.push_back(true);
		}
		else {
			boolean_tag.push_back(false);
			temp_sprite.at(1)->setColor(Color3B(255, 0, 0));
			temp_sprite.at(1)->setOpacity(50);
		}
		if (money >= SOLDIERBASE_GOLD && elect >= SOLDIERBASE_ELECT) {
			boolean_tag.push_back(true);
		}
		else {
			boolean_tag.push_back(false);
			temp_sprite.at(2)->setColor(Color3B(255, 0, 0));
			temp_sprite.at(2)->setOpacity(50);
		}
		if (money >= WARFACTORY_GOLD && elect >= WARFACTORY_ELECT) {
			boolean_tag.push_back(true);
		}
		else {
			boolean_tag.push_back(false);
			temp_sprite.at(3)->setColor(Color3B(255, 0, 0));
			temp_sprite.at(3)->setOpacity(50);
		}
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		//Click the small icon
		for (auto i = 0; i < temp_sprite.size(); i++) {
			auto temp = temp_sprite.at(i);
			listener->onTouchBegan = [boolean_tag,temp,temp_sprite,this,i,cache](Touch *touch, Event *event) {
				SimpleAudioEngine::getInstance()->playEffect(BUILD, false);
				auto target = static_cast<Sprite*>(event->getCurrentTarget());
				auto locationInNode = target->convertToNodeSpace(touch->getLocation());
				Size s = target->getContentSize();
				auto rect = Rect(0, 0, s.width, s.height);
				if (rect.containsPoint(locationInNode)) {
					if (!boolean_tag.at(i)) {
						//TODO warning sound
						for (auto temp : temp_sprite) {
							this->removeChild(temp, true);
						}
						return false;
					}
					if (isCreated()) {
						return false;
					}
					setCreated(true);
					auto temp_building = Sprite::createWithTexture(cache->addImage(StringUtils::format("unit/building_%d.png", i + 1)));
					temp_building->setTag(i + 1);
					/*
						make notes:
						tempNode below is a pointer to the TiledMap Layer
						get the world coordination of temp to set the Positon of the temp_building
					*/
					auto tempNode = this->getParent()->getParent()->getParent();
					auto pos = this->convertToWorldSpace(temp->getPosition());
					temp_building->setOpacity(75);
					temp_building->setPosition(pos);
					tempNode->addChild(temp_building,200);
					auto listener1 = EventListenerTouchOneByOne::create();
					listener1->setSwallowTouches(true);
					listener1->onTouchBegan = [this,temp_sprite,temp_building](Touch *touch, Event *event) {
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
						bool judgeBoundry = true;
						if (tiledLocation.x < tiledBase.x - this->getMaxRange() ||
							tiledLocation.x >= tiledBase.x + this->getMaxRange() ||
							tiledLocation.y < tiledBase.y - this->getMaxRange()||
							tiledLocation.y >= tiledBase.y + this->getMaxRange()) {
							judgeBoundry = false;
						}
						bool judgeCollidable = false;
						if (temp_building->getTag() == 2) {
							judgeCollidable = TiledMap::checkBuilt(tiledLocation, 1);
						}
						else {
							judgeCollidable = TiledMap::checkBuilt(tiledLocation, 2);
						}
						if (judgeCollidable && judgeBoundry) {
							temp_building->setColor(Color3B(152, 251, 152));
							this->setBuilt(true);

						}
						else {
							temp_building->setColor(Color3B(255,0,0));
							this->setBuilt(false);
						}

					};
					listener1->onTouchEnded = [this, temp_building](Touch *touch, Event *event) {
						auto touchLocation = touch->getLocation();
						Vec2 nodeLocation = this->getParent()->convertToNodeSpace(touchLocation);
						// if the field isn't occupied
						if (this->getBuilt()) {
							this->setBuilt(false);
							if (temp_building->getTag() == 1) {
								auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
								auto tempManager = tempScene->getUnitManager();
								MoneyMine* moneyMine = MoneyMine::create("moneyMine/MinetoMoney_24.png");
								if (tempScene->getMoney()->checkMoney(moneyMine->getGold()) &&
									tempScene->getPower()->checkPower(moneyMine->getElect())) {
									auto id = moneyMine->getIdCount();
									moneyMine->setUnitID(id);
									//send building message
									tempManager->addMessages(tempManager->msgs->newCreateBuildingMessage(moneyMine->getUnitID(), moneyMine->getType(),
										this->getCampID(), this->getUnitID(),nodeLocation));
								}
								//delete moneyMine;
							}
							else if (temp_building->getTag() == 2) {
								PowerPlant* powerPlant = PowerPlant::create("powerPlant/PowerBuilt_24.png");
								auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
								auto tempManager = tempScene->getUnitManager();
								if (tempScene->getMoney()->checkMoney(powerPlant->getGold())) {
									auto id = powerPlant->getIdCount();
									powerPlant->setUnitID(id);
									//send building message
									tempManager->addMessages(tempManager->msgs->newCreateBuildingMessage(powerPlant->getUnitID(), powerPlant->getType(), this->getCampID(),
										this->getUnitID(), nodeLocation));								
								}
								//delete powerPlant;
							}
							else if (temp_building->getTag() == 3) {
								SoldierBase* soldierBase = SoldierBase::create("soldierBase/soldierBase_23.png");
								auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
								auto tempManager = tempScene->getUnitManager();
								if (tempScene->getMoney()->checkMoney(soldierBase->getGold()) &&
									tempScene->getPower()->checkPower(soldierBase->getElect())) {
									auto id = soldierBase->getIdCount();
									soldierBase->setUnitID(id);
									//send building message
									tempManager->addMessages(tempManager->msgs->newCreateBuildingMessage(soldierBase->getUnitID(), soldierBase->getType(), this->getCampID(), this->getUnitID()
										, nodeLocation));
								}
								//delete soldierBase;
							}
							else if (temp_building->getTag() == 4) {
								WarFactory* warFactory = WarFactory::create("tankBase/tankbuilding_23.png");
								auto tempScene = static_cast<GameScene*>(this->getParent()->getParent()->getParent());
								auto tempManager = tempScene->getUnitManager();
								if (tempScene->getMoney()->checkMoney(warFactory->getGold()) &&
									tempScene->getPower()->checkPower(warFactory->getElect())) {
									auto id = warFactory->getIdCount();
									warFactory->setUnitID(id);
									//send building message
									tempManager->addMessages(tempManager->msgs->newCreateBuildingMessage(warFactory->getUnitID(), warFactory->getType(), this->getCampID(),
										this->getUnitID(),nodeLocation));
									
								}
								//delete warFactory;
							}
						}
						auto tempNode = this->getParent()->getParent()->getParent();
						tempNode->removeChild(temp_building,true);
						setCreated(false);
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
			listener->onTouchEnded = [this,temp_sprite](Touch *touch, Event *event) {
				for (auto temp : temp_sprite) {
					this->removeChild(temp, true);
				}
				setSelected(false);
			};
			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(),temp);
		}
		
		
		return true;
	}
	else {
		this->setSelected(false);
		this->getHP()->setVisible(false);
		
	}
	return false;
}

void Base::onTouchEnded(Touch *touch, Event *event) {
	this->setSelected(true);
}

Unit* Base::getBase() {
	return _base;
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