#include "UnitManager.h"
#include "time.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
using namespace CocosDenshion;

bool UnitManager::init(TiledMap * tiledMap) {
	_building =  1;
	_soider = 0;
	_tiled_Map = tiledMap;
	return true;
}


void UnitManager::initBase() {
	auto pos = getBasePosition("ObjectLayer");

	_base = Base::create();
	_base->setPosition(pos);
	auto vect = _base->getBase()->getContentSize();
	auto range = _base->getRange();
	auto tiledPos = _tiled_Map->tileCoordForPosition(pos); //change the OpenGL coordinate to TiledMap
	_base->setUnitID(_base->getIdCount());
	_base->addIdCount();
	TiledMap::newMapGrid(tiledPos, _base->getUnitID(), _base->getRange());
	TiledMap::newMapId(_base->getUnitID(), _base);
	//TODO set the camera to the Base
	_tiled_Map->getTiledMap()->addChild(_base, 100);
	_tiled_Map->getTiledMap()->setPosition(0 - _base->getPositionX() + vect.width * 2
		, 0 - _base->getPositionY() + vect.height * 1.5);
	// 预加载音效 preloading sound effect
	SimpleAudioEngine::getInstance()->preloadEffect(CONSTRUCTION);
	SimpleAudioEngine::getInstance()->preloadEffect(BUILD);
	SimpleAudioEngine::getInstance()->preloadEffect(SOLDIER);
	SimpleAudioEngine::getInstance()->preloadEffect(TANK);
	SimpleAudioEngine::getInstance()->preloadEffect(DOG);
	SimpleAudioEngine::getInstance()->preloadEffect(FIGHT);
	SimpleAudioEngine::getInstance()->preloadEffect(LOST);
	SimpleAudioEngine::getInstance()->preloadEffect(TANKBULLET);
	SimpleAudioEngine::getInstance()->preloadEffect(EXPLODE);
}

Vec2 UnitManager::getBasePosition(std::string layername) {
	auto group = _tiled_Map->getObjectGroup(layername);
	auto spawnPoint = group->getObject("Base");
	auto x = spawnPoint["x"].asFloat();
	auto y = spawnPoint["y"].asFloat();
	return Vec2(x, y);
}

void UnitManager::selectUnitsByPoint(Vec2 touch_point) {
	auto tiledLocation = _tiled_Map->tileCoordForPosition(touch_point);
	//judge if there is a unit in the Grid  判断该瓦片上是否有单位
	if (TiledMap::checkMapGrid(tiledLocation)) {
		auto id = TiledMap::getUnitIdByPosition(tiledLocation);
		auto tempSprite = TiledMap::getUnitById(id);
		//if the tempSprite is the enemy  如果选中的精灵是敌人
		if (tempSprite->getCampID() != this->getBase()->getCampID()) {
			//if > 1 what in the vector isn't a building   如果SelectVector的容量>1,那么它一定不会包含建筑
			auto enemy = tempSprite;
			enemy->getHP()->setVisible(true);
			if (TiledMap::checkSize() > 1) {
				for (auto temp : *TiledMap::getSelectedVector()) {
					//if the enemy is in the attack range 如果敌人在攻击范围内
					if (temp->judgeAttack(tiledLocation) && TiledMap::checkMapGrid(tiledLocation)) {
						temp->stopAllActions();
						msgs->newAttackMessage(temp->getUnitID(), enemy->getUnitID(), temp->getAttack());
						attack(temp->getUnitID(), enemy->getUnitID(), temp->getAttack());


						if (temp->judgeAttack(tiledLocation)) {
							//TODO function attack
						//	attack(temp, tempSprite);
						}
						else {
							//TODO function tracing
						}
					}
				}
			}
			else {
				//judge the one in vector is building or not 判断在vector里面的唯一一个元素是不是建筑
				if (TiledMap::checkSize() == 1) {
					auto temp = TiledMap::getSelectedVector()->at(0);
					//if not
					if (!temp->isBuilding()) {
						if (temp->judgeAttack(tiledLocation)) {
						//	attack(temp, tempSprite);
							//TODO function attack
							temp->stopAllActions();
							msgs->newAttackMessage(temp->getUnitID(), enemy->getUnitID(), temp->getAttack());
							attack(temp->getUnitID(), enemy->getUnitID(), temp->getAttack());
								
						}
						else {
							//TODO function tracing
						}
					}
				}
			}
		}
		//if not, then clear up the vector and then push the new one
		//如果选中的精灵是己方
		else {
			TiledMap::clearUp();
			TiledMap::newVectorUnit(tempSprite);
		}
	}
	//if not 如果点到的是空地
	else {
		//check the vector and judge if there is a building in it 
		//检查vector,看是否只存了一个建筑
		if (TiledMap::checkPass(tiledLocation)) {
			if (TiledMap::checkSize()) {
				//if yes, clear up
				if (TiledMap::getSelectedVector()->at(0)->isBuilding()) {
					TiledMap::clearUp();
				}
				//if not, call all the unit in the vector to find a path and move to the Position
				else {
					//TODO function to move to the Position
					for (auto temp : *TiledMap::getSelectedVector()) {
						Vec2 from = _tiled_Map->tileCoordForPosition(temp->getPosition());
						if (!_tiled_Map->checkPass(tiledLocation)) {
							tiledLocation = _tiled_Map->findFreeNear(tiledLocation);
						}
						PathArithmetic* path_finder = PathArithmetic::create();
						path_finder->initPathArithmetic(_tiled_Map, from, tiledLocation);
						path_finder->findPath();
						auto path = path_finder->getPath();
						if (temp->getNumberOfRunningActions() == 0) {
							msgs->newMoveMessage(temp->getUnitID(), path, touch_point);
							playerMoveWithWayPoints(temp->getUnitID(), path, touch_point);
						}
						else {
							temp->stopAllActions();
							msgs->newMoveMessage(temp->getUnitID(), path, touch_point);
							playerMoveWithWayPoints(temp->getUnitID(), path, touch_point);
						}
					}
				}
			}
		}
		else {
			auto new_point = _tiled_Map->findFreeNear(tiledLocation);
			Vec2 openGL_point = _tiled_Map->locationForTilePos(new_point);
			selectUnitsByPoint(openGL_point);
		}
	}
}

void UnitManager::selectUnitsByRect(MouseRect* mouse_rect) {
	float rect_x = MIN(mouse_rect->start.x, mouse_rect->end.x);
	float rect_y = MAX(mouse_rect->start.y, mouse_rect->end.y);
	float rect_width = fabs(mouse_rect->start.x - mouse_rect->end.x);
	float rect_height = fabs(mouse_rect->start.y - mouse_rect->end.y);
	TiledMap::clearUp();	//cancel select
	auto tiledPos = _tiled_Map->tileCoordForPosition(Vec2(rect_x, rect_y));
	auto tiledRect = _tiled_Map->tileCoordForPosition(Vec2(rect_width, rect_height));
	tiledRect.y = 127 - tiledRect.y;
	for (auto i = tiledPos.x; i < tiledRect.x + tiledPos.x; i++) {
		for (auto j = tiledPos.y; j < tiledRect.y + tiledPos.y; j++) {
			auto pos = Vec2(i, j);
			//if there is a unit Grid in this pos
			if (TiledMap::checkMapGrid(pos)) {
				auto id = TiledMap::getUnitIdByPosition(pos);
				auto tempSprite = TiledMap::getUnitById(id);
				//if the unit is belonging to us and it isn't a building
				if ((tempSprite->getCampID() == this->getBase()->getCampID())
					&& !tempSprite->isBuilding()) {
					TiledMap::newVectorUnit(tempSprite);
				}
			}
		}
	}
}

void UnitManager::playerMoveWithWayPoints(int move_unit_id, std::vector<cocos2d::Vec2> path_points, cocos2d::Vec2 end_point) {
	Unit* player = TiledMap::getUnitById(move_unit_id);
	player->stopAllActions();
	if (path_points.empty()) {
		return;
	}
	auto tiledLocation = path_points.back();
	/*change the direction of the unit according to the target end_point*/
	Vec2 tarPos = _tiled_Map->locationForTilePos(end_point);
	Vec2 myPos = _tiled_Map->locationForTilePos(player->getPosition());
	float angle = atan2((tarPos.y - myPos.y), (tarPos.x - myPos.x)) * 180 / 3.14159;
	if (player->isFlippedX()) {
		player->setFlippedX(false);
	}
	if (tarPos.x < myPos.x) {
		player->setFlippedX(true);
		player->setRotation(angle - 180);
	}
	else {
		player->setRotation(angle);
	}
	
	Animate* animate;
	float speed;
	switch ((player->getType())[0])
	{
	case 's':
		animate = player->getAnimateByName("soldierRun/soldier", 0.2, 7);
		speed = 0.4f;
		break;
	case 'd':
		animate = player->getAnimateByName("dogRun/dog", 0.2, 5);
		speed = 0.3f;
		break;
	case 't':
		animate = player->getAnimateByName("tank/tank", 0.2, 7);
		speed = 0.5f;
		break;
	default:
		break;
	}
	/*add move animation*/
	auto repeatanimate = RepeatForever::create(animate);
	player->runAction(repeatanimate);
	Vector<FiniteTimeAction*> actionVector;
//Animate* animate;
	auto callfunc = CallFunc::create([=] {
		stopAction(repeatanimate);
		switch ((player->getType())[0]) {
		case 'd':
			player->setTexture("unit/FighterUnit_1.png");
			break;
		case 's':
			player->setTexture("unit/FighterUnit_2.png");
			break;
		case 't':
			player->setTexture("unit/FighterUnit.png");
			break;
		default:
			break;
		}
	});
/*	for (int i = 0; i < path_points.size(); i++) {
		Vec2 openGL_point = _tiled_Map->locationForTilePos(path_points[i]);
		if (TiledMap::checkPass(path_points[i])1) {
			if (i > 0) {
				player->stopActionByTag(i - 1);
			}
			auto moveTo = MoveTo::create(0.4f, openGL_point);
			moveTo->setTag(i);
			TiledMap::updateMapGrid(player->getTiledPosition(), path_points[i]);
			player->setTiledPosition(path_points[i]);
			player->runAction(moveTo);
		}
		else {
			auto path_finder = PathArithmetic::create();
			if (!TiledMap::checkPass(tiledLocation)) {
				tiledLocation = _tiled_Map->findFreeNear(tiledLocation);
			}
			path_finder->initPathArithmetic(_tiled_Map, player->getTiledPosition(), tiledLocation);
			path_finder->findPath();
			auto path_points = path_finder->getPath();
			playerMoveWithWayPoints(player, tiledLocation, path_points);
			return;
		}
	}*/
	for (int i = 0; i < path_points.size(); i++) {
		if (i == 0) {
			TiledMap::setUnpass(tiledLocation);
		}
		Vec2 openGL_point = _tiled_Map->locationForTilePos(path_points[i]);
		//auto moveTo = MoveTo::create(0.4f, openGL_point);
	//	moveTo->setTag(i+1);
		auto callfuncPosition = CallFunc::create([=] {
			TiledMap::updateMapGrid(player->getTiledPosition(), path_points[i]);
			player->setTiledPosition(path_points[i]);
			/*if (i < path_points.size() - 2) {
				int count = 0;
				if (TiledMap::checkPass(path_points[i + 1])){
					TiledMap::setUnpass(path_points[i + 1]);
				}
				if (count == 100000 && !TiledMap::checkPass(path_points[i + 1])) {
					auto location = path_points.back();
					if (!_tiled_Map->checkPass(location)) {
						location = _tiled_Map->findFreeNear(location);
					}
					PathArithmetic* path_finder = PathArithmetic::create();
					path_finder->initPathArithmetic(_tiled_Map, path_points[i], location);
					path_finder->findPath();
					auto path_points = path_finder->getPath();
					playerMoveWithWayPoints(player, location, path_points);
					return;
				}
			}
		});*/
		});
		//callfuncPosition->setTag(200 + i);
			MoveTo* moveTo = MoveTo::create(speed, openGL_point);
			actionVector.pushBack(moveTo);
			actionVector.pushBack(callfuncPosition);
	}

/*	auto callfunc = CallFunc::create([=] {
		player->stopAction(repeatanimate);
		switch (player->getType()) {
		case 'd':
			player->setTexture("unit/FighterUnit_1.png");
			break;
		case 's':
			player->setTexture("unit/FighterUnit_2.png");
			break;
		case 't':
			player->setTexture("unit/FighterUnit.png");
			break;
		default:
			break;
		}
	});*/
	actionVector.pushBack(callfunc);
	auto sequence = Sequence::create(actionVector);
	sequence->setTag(1000);
	player->runAction(sequence);
}


//暂时没用
void UnitManager::delay(float dt) {
	clock_t start_time, cur_time;
	start_time = clock();
	while ((clock() - start_time) < dt * CLOCKS_PER_SEC);
}

void UnitManager::attack(int attacker_id, int under_attack_id, int damage) {
	auto player = TiledMap::getUnitById(attacker_id);
	auto enemy = TiledMap::getUnitById(under_attack_id);
	attackEffect(player, enemy);
	//decrease the Hp
	enemy->setLifeValue(enemy->getLifeValue() - damage);
	if (enemy->getLifeValue() <= 0) {
		if (enemy->isBuilding()) {
			auto callFunc = CallFunc::create([=] {
				destroyEffect(enemy, true);
				auto tiledLocation = _tiled_Map->tileCoordForPosition(enemy->getPosition());
				TiledMap::removeMapGrid(tiledLocation, enemy->getFixModel());
				_tiled_Map->getTiledMap()->removeChild(enemy);
			});
			this->runAction(callFunc);
		}
		else {
			auto callFunc = CallFunc::create([=] {
				destroyEffect(enemy, false);
				TiledMap::removeMapGrid(enemy->getTiledPosition());
				_tiled_Map->getTiledMap()->removeChild(enemy);
			});
			this->runAction(callFunc);
		}
	}
	if (enemy->getHP() != nullptr) {
		enemy->getHP()->setPercent(enemy->getHPInterval() * static_cast<float>(enemy->getLifeValue()));
	}
}

void UnitManager::attackEffect(Unit* player, Unit *enemy) {
	/*change the direction of the unit according to the target position*/
	Vec2 tarPos = _tiled_Map->locationForTilePos(enemy->getPosition());
	Vec2 myPos = _tiled_Map->locationForTilePos(player->getPosition());
	float angle = atan2((tarPos.y - myPos.y), (tarPos.x - myPos.x)) * 180 / 3.14159;
	if (player->isFlippedX()) {
		player->setFlippedX(false);
	}
	if (tarPos.x < myPos.x) {
		player->setFlippedX(true);
		player->setRotation(angle - 180);
	}
	else {
		player->setRotation(angle);
	}
	auto type = (player->getType())[0];
	auto pos = enemy->getPosition();
	bool isDog = false;
	Sprite* bullet;
	Animate* dogAttack;
	switch (type) {
	case 's':
		SimpleAudioEngine::getInstance()->playEffect(FIGHT, false);
		player->setTexture("unit/FighterUnit_2.png");
		bullet = Sprite::create("soldierAttack/bullet.png");
		bullet->setPosition(player->getPosition());
		bullet->setScale(0.2);
		bullet->setFlippedX(true);
		_tiled_Map->getTiledMap()->addChild(bullet, 30);
		break;
	case 'd':
		SimpleAudioEngine::getInstance()->playEffect(DOG, false);
		player->setTexture("unit/FighterUnit_1.png");
		dogAttack = player->getAnimateByName("dogAttack/dog_attack", 0.1, 6);
		isDog = true;
		break;
	case 't':
		SimpleAudioEngine::getInstance()->playEffect(TANKBULLET, false);
		bullet = Sprite::create("tank/tankBullet.png");
		bullet->setPosition(player->getPosition());
		bullet->setScale(0.4);
		_tiled_Map->getTiledMap()->addChild(bullet, 30);
		break;
	default:
		break;
	}
	if (isDog) {
		auto callfuc = CallFunc::create([=] {
			player->stopAction(dogAttack);
			player->setTexture("unit/FighterUnit_1.png");
		});
		auto sequence = Sequence::create(dogAttack, callfuc, NULL);
		player->runAction(sequence);
	}
	else {
		Vec2 shootVector = enemy->getPosition() - player->getPosition();
		Vec2 normalizedVector = ccpNormalize(shootVector);
		float radians = atan2(normalizedVector.y, -normalizedVector.x);
		float degree = CC_RADIANS_TO_DEGREES(radians);
		bullet->setRotation(degree);
		auto moveTo = MoveTo::create(0.5f, enemy->getPosition());
		auto callfuc = CallFunc::create([=] {
			_tiled_Map->getTiledMap()->removeChild(bullet);
		});
		auto sequence = Sequence::create(moveTo, callfuc, NULL);
		bullet->runAction(sequence);
	}
}

void UnitManager::destroyEffect(Unit* unit,bool type) {
	if (type) {
		SimpleAudioEngine::getInstance()->playEffect(EXPLODE, false);
		auto blast = Unit::create("explode1/explode0.png");
		auto animate = blast->getAnimateByName("explode1/explode", 0.1f, 30);
		_tiled_Map->getTiledMap()->addChild(blast, 210);
		blast->setPosition(unit->getPosition());
		auto callfunc = CallFunc::create([=] {
			_tiled_Map->getTiledMap()->removeChild(blast);
		});
		auto action = Sequence::create(animate, callfunc, NULL);
		blast->runAction(action);
	}
	else {
		SimpleAudioEngine::getInstance()->playEffect(LOST, false);
	}
}

void UnitManager::Building(int new_building_id, std::string new_building_type, int base_id, int from_building_id,
	cocos2d::Vec2 nodeLocation) {
	Unit* base = TiledMap::getUnitById(from_building_id);
	auto tiledLocation = static_cast<TiledMap*>(base->getParent()->getParent())->tileCoordForPosition(nodeLocation);
	auto tempScene = static_cast<GameScene*>(base->getParent()->getParent()->getParent());
	if (new_building_type == "M") {
		MoneyMine* moneyMine = MoneyMine::create("moneyMine/MinetoMoney_24.png");
		moneyMine->setUnitID(new_building_id);
		moneyMine->setCampID(base_id);
		if (moneyMine->getCampID() == RED) {
			moneyMine->setColor(Color3B(221, 160, 221));
		}
		else {
			moneyMine->setColor(Color3B(65, 105, 225));
		}
		moneyMine->addIdCount();
		moneyMine->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
		BuildingBase::setIsBuilt(true);
		moneyMine->Build();
		TiledMap::newMapGrid(tiledLocation, new_building_id, moneyMine->getRange());
		TiledMap::newMapId(new_building_id, moneyMine);
		//					TiledMap::setUnpass(tiledLocation, moneyMine->getRange());
		static_cast<TMXTiledMap*>(base->getParent())->addChild(moneyMine, 50);
		tempScene->getVectorMine().pushBack(moneyMine);
		tempScene->getPower()->spendPower(moneyMine->getElect());
		tempScene->getMoney()->spendMoney(moneyMine->getGold());
	}
	else if (new_building_type == "P") {
		PowerPlant* powerPlant = PowerPlant::create("powerPlant/PowerBuilt_24.png");
		powerPlant->setUnitID(new_building_id);
		powerPlant->setCampID(base_id);
		if (powerPlant->getCampID() == RED) {
			powerPlant->setColor(Color3B(221, 160, 221));
		}
		else {
			powerPlant->setColor(Color3B(65, 105, 225));
		}
		powerPlant->addIdCount();
		powerPlant->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
		BuildingBase::setIsBuilt(true);
		powerPlant->Build();
		TiledMap::newMapGrid(tiledLocation, new_building_id, powerPlant->getRange());
		TiledMap::newMapId(new_building_id, powerPlant);
		//							TiledMap::setUnpass(tiledLocation, powerPlant->getRange());
		static_cast<TMXTiledMap*>(base->getParent())->addChild(powerPlant, 40);
		tempScene->getVectorPower().pushBack(powerPlant);
		tempScene->getPower()->increasePower((powerPlant->getElect()));
		tempScene->getMoney()->spendMoney(powerPlant->getGold());
	}
	else if (new_building_type == "S") {
		SoldierBase* soldierBase = SoldierBase::create("soldierBase/soldierBase_23.png");
		soldierBase->setUnitID(new_building_id);
		soldierBase->setCampID(base_id);
		if (soldierBase->getCampID() == RED) {
			soldierBase->setColor(Color3B(221, 160, 221));
		}
		else {
			soldierBase->setColor(Color3B(65, 105, 225));
		}
		soldierBase->addIdCount();
		soldierBase->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
		BuildingBase::setIsBuilt(true);
		soldierBase->Build();
		TiledMap::newMapGrid(tiledLocation, new_building_id, soldierBase->getRange(), FIX_HEIGHT);
		TiledMap::newMapId(new_building_id, soldierBase);
		//							TiledMap::setUnpass(tiledLocation, soldierBase->getRange());
		static_cast<TMXTiledMap*>(base->getParent())->addChild(soldierBase, 50);
		tempScene->getVectorSoldier().pushBack(soldierBase);
		tempScene->getVectorSoldier().pushBack(soldierBase);
		tempScene->getPower()->spendPower(soldierBase->getElect());
		tempScene->getMoney()->spendMoney(soldierBase->getGold());
	}
	else if (new_building_type == "W") {
		WarFactory* warFactory = WarFactory::create("tankBase/tankbuilding_23.png");
		warFactory->setUnitID(new_building_id);
		warFactory->setCampID(base_id);
		if (warFactory->getCampID() == RED) {
			warFactory->setColor(Color3B(221, 160, 221));
		}
		else {
			warFactory->setColor(Color3B(65, 105, 225));
		}
		warFactory->addIdCount();
		warFactory->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
		BuildingBase::setIsBuilt(true);
		warFactory->Build();
		
		TiledMap::newMapGrid(tiledLocation, new_building_id, warFactory->getRange(), FIX_HEIGHT);
		TiledMap::newMapId(new_building_id, warFactory);
		//				TiledMap::setUnpass(tiledLocation, warFactory->getRange());
		static_cast<TMXTiledMap*>(base->getParent())->addChild(warFactory, 50);
		tempScene->getVectorFactory().pushBack(warFactory);
		tempScene->getVectorFactory().pushBack(warFactory);
		tempScene->getPower()->spendPower(warFactory->getElect());
		tempScene->getMoney()->spendMoney(warFactory->getGold());
	}
}

void UnitManager::NewUnitCreate(int new_unit_id, std::string new_unit_type, int base_id, int from_building_id,
	cocos2d::Vec2 nodeLocation) {
	Unit* plant = TiledMap::getUnitById(from_building_id);
	auto tempTiledMap = static_cast<TiledMap*>(plant->getParent()->getParent());
	auto tempScene = static_cast<GameScene*>(plant->getParent()->getParent()->getParent());
	if (new_unit_type == "d") {
		Dog* dog = Dog::create("dogRun/dog0.png");
		dog->setUnitID(new_unit_id);
		dog->setCampID(base_id);
		if (dog->getCampID() == RED) {
			dog->setColor(Color3B(255, 0, 0));
		}
		else {
			dog->setColor(Color3B(65, 105, 225));
		}
		dog->addIdCount();
		if (nodeLocation.x < plant->getPosition().x) {
			dog->setFlippedX(true);
		}
		auto tiledLocation = tempTiledMap->tileCoordForPosition(nodeLocation);
		dog->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
		Dog::setIsCreated(true);
		dog->Create(plant);
		//		TiledMap::setUnpass(tiledLocation);
		//		auto tiledLocation = tempScene->tileCoordForPosition(nodeLocation);
		TiledMap::newMapGrid(tiledLocation, dog->getUnitID());
		TiledMap::newMapId(dog->getUnitID(), dog);
		dog->setTiledPosition(tiledLocation);
		static_cast<TMXTiledMap*>(plant->getParent())->addChild(dog, 200);
		//tempScene->getVectorDogs().pushBack(dog);
		tempScene->getMoney()->spendMoney(dog->getGold());
	}
	else if (new_unit_type == "s") {
		Soldier* soldier = Soldier::create("soldierRun/soldierstand.png");
		soldier->setUnitID(new_unit_id);
		soldier->setCampID(base_id);
		if (soldier->getCampID() == RED) {
			soldier->setColor(Color3B(255, 0, 0));
		}
		else {
			soldier->setColor(Color3B(65, 105, 225));
		}
		soldier->addIdCount();
		if (nodeLocation.x < plant->getPosition().x) {
			soldier->setFlippedX(true);
		}
		auto tiledLocation = tempTiledMap->tileCoordForPosition(nodeLocation);
		soldier->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
		Soldier::setIsCreated(true);
		soldier->Create(plant);
		//		TiledMap::setUnpass(tiledLocation);
		static_cast<TMXTiledMap*>(plant->getParent())->addChild(soldier, 200);
		//		auto tiledLocation = tempScene->tileCoordForPosition(nodeLocation);
		TiledMap::newMapGrid(tiledLocation, soldier->getUnitID());
		TiledMap::newMapId(soldier->getUnitID(), soldier);
		soldier->setTiledPosition(tiledLocation);
		//	tempScene->getVectorSoldiers().pushBack(soldier);
		tempScene->getMoney()->spendMoney(soldier->getGold());
	}
	else if (new_unit_type == "t") {
		Tank* tank = Tank::create("tank/tank0.png");
		tank->setUnitID(new_unit_id);
		tank->setCampID(base_id);
		if (tank->getCampID() == RED) {
			tank->setColor(Color3B(221, 160, 221));
		}
		else {
			tank->setColor(Color3B(65, 105, 225));
		}
		tank->addIdCount();
		tank->setScale(0.4f);
		if (nodeLocation.x < plant->getPosition().x) {
			tank->setFlippedX(true);
		}
		auto tiledLocation = tempTiledMap->tileCoordForPosition(nodeLocation);
		tank->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
		Tank::setIsCreated(true);
		tank->Create(plant);
		//		TiledMap::setUnpass(tiledLocation);
		static_cast<TMXTiledMap*>(plant->getParent())->addChild(tank, 200);
		//		auto tiledLocation = tempScene->tileCoordForPosition(nodeLocation);
		TiledMap::newMapGrid(tiledLocation, tank->getUnitID());
		TiledMap::newMapId(tank->getUnitID(), tank);
		tank->setTiledPosition(tiledLocation);
		//	tempScene->getVectorSoldiers().pushBack(soldier);
		tempScene->getMoney()->spendMoney(tank->getGold());
	}
}