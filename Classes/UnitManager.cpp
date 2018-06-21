#include "UnitManager.h"
#include "time.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
using namespace CocosDenshion;

bool UnitManager::init(TiledMap * tiledMap, std::shared_ptr<SocketServer> spserver,
	std::shared_ptr<SocketClient> spclient) {
	_building = 1;
	_soider = 0;
	_tiled_Map = tiledMap;
	_socket_server = spserver;
	_socket_client = spclient;
	_myCamp = _socket_client->camp();
	if (_myCamp == REDCAMP) {
		_enCamp = BLUECAMP;
	}
	else {
		_enCamp = REDCAMP;
	}

	//this->schedule(schedule_selector(updateMessage), 5.0f / 60);
	return true;
}


void UnitManager::initBase() {
	auto myPos = getBasePosition("ObjectLayer", _myCamp);
	_base_me = Base::create(_myCamp);
	_base_me->setPosition(myPos);
	auto vect = _base_me->getBase()->getContentSize();
	auto range = _base_me->getRange();
	auto tiledPos = _tiled_Map->tileCoordForPosition(myPos); //change the OpenGL coordinate to TiledMap
	_base_me->setTiledPosition(tiledPos);
	_base_me->setType("B");
	_base_me->setProgressed(true);
	//TODO set the camera to the Base
	
	if (_myCamp == BLUECAMP) {
		_tiled_Map->getTiledMap()->setPosition(0 - _base_me->getPositionX() + vect.width * 2
			, 0 - _base_me->getPositionY() + vect.height * 2.0);
	}
	else {
		_tiled_Map->getTiledMap()->setPosition(0 - _base_me->getPositionX() + vect.width * 2
			, 0 - _base_me->getPositionY() + vect.height * 1.5);
	}



	auto enPos = getBasePosition("ObjectLayer", _enCamp);
	_base_en = Base::create(_enCamp);
	_base_en->setPosition(enPos);
	vect = _base_en->getBase()->getContentSize();
	range = _base_en->getRange();
	auto tiledPos1 = _tiled_Map->tileCoordForPosition(enPos); //change the OpenGL coordinate to TiledMap
	_base_en->setTiledPosition(tiledPos1);
	_base_en->setProgressed(true);
	_base_en->setType("B");
	

	if (_myCamp == BLUECAMP) {
		_base_en->setUnitID(_base_en->getIdCount());
		_base_en->addIdCount();
		_base_me->setUnitID(_base_me->getIdCount());
		_base_me->addIdCount();
	}
	else {
		_base_me->setUnitID(_base_me->getIdCount());
		_base_me->addIdCount();
		_base_en->setUnitID(_base_en->getIdCount());
		_base_en->addIdCount();
	}

	TiledMap::newMapGrid(tiledPos1, _base_en->getUnitID(), _base_en->getRange());
	TiledMap::newMapId(_base_en->getUnitID(), _base_en);
	//TODO set the camera to the Base
	_tiled_Map->getTiledMap()->addChild(_base_en, 100);

	TiledMap::newMapGrid(tiledPos, _base_me->getUnitID(), _base_me->getRange());
	TiledMap::newMapId(_base_me->getUnitID(), _base_me);
	_tiled_Map->getTiledMap()->addChild(_base_me, 100);

}

Vec2 UnitManager::getBasePosition(std::string layername, int campId) {
	auto group = _tiled_Map->getObjectGroup(layername);
	ValueMap spawnPoint;
	if (campId == REDCAMP) {
		spawnPoint = group->getObject("BaseRed");
	}
	else {
		spawnPoint = group->getObject("BaseBlue");
	}

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
					if (enemy->getUnitID() == temp->getTargetID() && temp->isAttack()) {
						return;
					}
					//Set the Type to Attack
					temp->stopAllActions();
					temp->clearAllType();
					temp->setAttack(true);
					temp->setTargetID(enemy->getUnitID());

				}
			}
			else {
				//judge the one in vector is building or not 判断在vector里面的唯一一个元素是不是建筑
				if (TiledMap::checkSize() == 1) {
					auto temp = TiledMap::getSelectedVector()->at(0);
					//if not
					if (enemy->getUnitID() == temp->getTargetID() && temp->isAttack()) {
						return;
					}
					if (!temp->isBuilding()) {

						//Set the Type to Attack
						temp->stopAllActions();
						temp->clearAllType();
						temp->setAttack(true);
						auto id = enemy->getUnitID();
						temp->setTargetID(id);
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
						std::vector<Vec2> path = { tiledLocation };
						if (temp->getNumberOfRunningActions() == 0) {
							temp->clearAllType();
							temp->setMove(true);
							UnitManager::addMessages(msgs->newMoveMessage(temp->getUnitID(), path, tiledLocation));
						}
						else {
							temp->stopAllActions();
							temp->clearAllType();
							temp->setMove(true);
							//send moving message
							UnitManager::addMessages(msgs->newMoveMessage(temp->getUnitID(), path, tiledLocation));
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
	auto player = TiledMap::getUnitById(move_unit_id);
	player->stopAllActions();
	if (path_points.empty()) {
		return;
	}
	auto tiledLocation = path_points.back();

	/*change the direction of the unit according to the target end_point*/
	Vec2 tarPos = _tiled_Map->locationForTilePos(_tiled_Map->locationForTilePos(end_point));
	Vec2 myPos = _tiled_Map->locationForTilePos(player->getPosition());
	float angle = atan2((tarPos.y - myPos.y), (tarPos.x - myPos.x)) * 180 / 3.14159;
	if (player->getType().at(0) != 't') {
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
	}
	Animate* animate;
	float speed;
	switch ((player->getType())[0])
	{
	case 's':
		animate = player->getAnimateByName("soldierRun", 0.2, 7);
		speed = SOIDIER_SPEED;
		break;
	case 'd':
		animate = player->getAnimateByName("dogRun", 0.2, 5);
		speed = DOG_SPEED;
		break;
	case 't':
		animate = player->getAnimateByName("tankRun", 0.2, 7);
		speed = TANK_SPEED;
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
		TiledMap::updateMapGrid(player->getTiledPosition(), tiledLocation);
	//	log("SetPass %f,%f\n", player->getTiledPosition().x, player->getTiledPosition().y);
		player->setTiledPosition(tiledLocation);
		switch ((player->getType())[0]) {
			//	log("%f,%f  %f,%f", player->getTiledPosition().x, player->getTiledPosition().y,
			//		tiledLocation.x, tiledLocation.y);
		case 'd':
			player->setTexture("unit/FighterUnit_1.png");
			break;
		case 's':
			player->setTexture("unit/FighterUnit_2.png");
			break;
		case 't':
		//	player->setTexture("unit/FighterUnit.png");
			break;
		default:
			break;
		}
		log("%s", "Done!");
		if (player->isMove()) {
			player->setMove(false);
			player->setAutoAttack(true);
		}
	});
	Sequence *sequence;
	for (int i = 0; i < path_points.size(); i++) {
		Vec2 openGL_point = _tiled_Map->locationForTilePos(path_points[i]);
		MoveTo* moveTo = MoveTo::create(speed, openGL_point);
		auto callfunc = CallFunc::create([=] {
			player->setTempPos(path_points[i]);
//			log("player->set  %f,%f", player->getTempPos().x, player->getTempPos().y);
		});
		auto action = Spawn::create(moveTo, callfunc, NULL);
		actionVector.pushBack(action);
	}

	actionVector.pushBack(callfunc);
	sequence = Sequence::create(actionVector);
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
	if (!TiledMap::checkUnitId(attacker_id)) {
		return;
	}
	auto player = TiledMap::getUnitById(attacker_id);
	if (!TiledMap::checkUnitId(under_attack_id)) {
		player->clearAllType();
		player->setAutoAttack(true);
		return;
	}
	auto enemy = TiledMap::getUnitById(under_attack_id);
	auto attackNumber = player->getAttack();
	//decrease the Hp
	enemy->setLifeValue(enemy->getLifeValue() - attackNumber);
	if (enemy->getLifeValue() <= 0 && TiledMap::checkMapGrid(enemy->getTiledPosition())) {
		if (!TiledMap::getSelectedVector()->empty()) {
			auto selectedVector = TiledMap::getSelectedVector();
			for (auto i = selectedVector->begin(); i != selectedVector->end(); i++) {
				if (enemy->getUnitID() == (*i)->getUnitID()) {
					selectedVector->erase(i);
					break;
				}
			}

			auto callFunc = CallFunc::create([=] {
				destroyEffect(enemy, true);
				auto tiledLocation = _tiled_Map->tileCoordForPosition(enemy->getPosition());
				if (!TiledMap::checkUnitId(under_attack_id)) {
					return;
				}
				TiledMap::removeMapGrid(tiledLocation, enemy->getFixModel());
				TiledMap::removeMapId(enemy->getUnitID());
				_tiled_Map->getTiledMap()->removeChild(enemy);
			});
			this->runAction(callFunc);
		}
		else {
			auto callFunc = CallFunc::create([=] {
				destroyEffect(enemy, false);
				if (!TiledMap::checkUnitId(under_attack_id)) {
					return;
				}
				TiledMap::removeMapGrid(enemy->getTiledPosition());
				TiledMap::removeMapId(enemy->getUnitID());
				if (!enemy->isBuilding()) {
					enemy->unscheduleAllSelectors();
					if (enemy->getTargetPos().x != -1) {
						TiledMap::setPass(enemy->getTargetPos());
					}
				}
				_tiled_Map->getTiledMap()->removeChild(enemy);
			});
			this->runAction(callFunc);
		}
	}
	if (enemy->getHP() != nullptr) {
		enemy->getHP()->setVisible(true);
		enemy->getHP()->setPercent(enemy->getHPInterval() * static_cast<float>(enemy->getLifeValue()));
	}

}


void UnitManager::attackEffect(int attacker_id, int under_attack_id) {
	if (!TiledMap::checkUnitId(attacker_id)) {
		return;
	}
	auto player = TiledMap::getUnitById(attacker_id);
	if (!TiledMap::checkUnitId(under_attack_id)) {
		return;
	}
	auto enemy = TiledMap::getUnitById(under_attack_id);
	/*change the direction of the unit according to the target position*/
	Vec2 tarPos = _tiled_Map->locationForTilePos(enemy->getPosition());
	auto Pos111 = player->getPosition();
	Vec2 myPos = _tiled_Map->locationForTilePos(Pos111);
	float angle = atan2((tarPos.y - myPos.y), (tarPos.x - myPos.x)) * 180 / 3.14159;
	if (player->getType().at(0) != 't') {
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
		bullet = Sprite::createWithTexture
		(Director::getInstance()->getTextureCache()->addImage("soldierAttack/bullet.png"));
		bullet->setPosition(player->getPosition());
		bullet->setScale(0.2);
		bullet->setFlippedX(true);
		_tiled_Map->getTiledMap()->addChild(bullet, 30);
		break;
	case 'd':
		SimpleAudioEngine::getInstance()->playEffect(DOG, false);
	//	player->setTexture("unit/FighterUnit_1.png");
		dogAttack = player->getAnimateByName("dogAttack", 0.1, 6);
		isDog = true;
		break;
	case 't':
		//SimpleAudioEngine::getInstance()->playEffect(TANKBULLET, false);
		bullet = Sprite::createWithTexture
		(Director::getInstance()->getTextureCache()->addImage("tank/tankBullet.png"));
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



void UnitManager::destroyEffect(Unit* unit, bool type) {
	if (type) {
		SimpleAudioEngine::getInstance()->playEffect(EXPLODE, false);
		auto blast = Unit::create("explode1/explode0.png");
		auto animate = blast->getAnimateByName("explode", 0.1f, 30);
		_tiled_Map->getTiledMap()->addChild(blast, 210);
		blast->setPosition(unit->getPosition());
		auto buildingType = unit->getType();
		auto campID = unit->getCampID();
		if (buildingType == "B") {
			if (campID == this->_myCamp) {//my base destroyed
				this->checkWinOrLose(0);
				return;
			}
			else {
				this->checkWinOrLose(1);
				return;
			}
		}
		auto callfunc = CallFunc::create([=] {
				
			if (buildingType == "M") {
				if (campID == this->_myCamp) {
					static_cast<GameScene*>(this->getParent())->getVectorMine().popBack();
				}
			}
			else if (buildingType == "P") {
				if (campID == this->_myCamp) {
					auto tempScene = static_cast<GameScene*>(this->getParent());
					int power = tempScene->getPower()->getPower();
					if (power <= 150) {
						tempScene->getPower()->spendPower(power);
					}
					else {
						tempScene->getPower()->spendPower(150);
					}
				}
			}
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
		if (moneyMine->getCampID() == REDCAMP) {
			moneyMine->setColor(Color3B(221, 160, 221));
		}
		else {
			moneyMine->setColor(Color3B(65, 105, 225));
		}
		moneyMine->addIdCount();
		moneyMine->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
		if (base_id == REDCAMP) {
			BuildingBase::setRedIsBuilt(true);
		}
		else {
			BuildingBase::setBlueIsBuilt(true);
		}
		moneyMine->Build();
		TiledMap::newMapGrid(tiledLocation, new_building_id, moneyMine->getRange());
		TiledMap::newMapId(new_building_id, moneyMine);
		moneyMine->setTiledPosition(tiledLocation);
		//					TiledMap::setUnpass(tiledLocation, moneyMine->getRange());
		static_cast<TMXTiledMap*>(base->getParent())->addChild(moneyMine, 50);
		if (base_id == this->_myCamp) {
			tempScene->getVectorMine().pushBack(moneyMine);
			tempScene->getPower()->spendPower(moneyMine->getElect());
			tempScene->getMoney()->spendMoney(moneyMine->getGold());
		}
	}
	else if (new_building_type == "P") {
		PowerPlant* powerPlant = PowerPlant::create("powerPlant/PowerBuilt_24.png");
		powerPlant->setUnitID(new_building_id);
		powerPlant->setCampID(base_id);
		if (powerPlant->getCampID() == REDCAMP) {
			powerPlant->setColor(Color3B(221, 160, 221));
		}
		else {
			powerPlant->setColor(Color3B(65, 105, 225));
		}
		powerPlant->addIdCount();
		powerPlant->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
		if (base_id == REDCAMP) {
			BuildingBase::setRedIsBuilt(true);
		}
		else {
			BuildingBase::setBlueIsBuilt(true);
		}
		powerPlant->Build();
		TiledMap::newMapGrid(tiledLocation, new_building_id, powerPlant->getRange());
		TiledMap::newMapId(new_building_id, powerPlant);
		powerPlant->setTiledPosition(tiledLocation);
		//							TiledMap::setUnpass(tiledLocation, powerPlant->getRange());
		static_cast<TMXTiledMap*>(base->getParent())->addChild(powerPlant, 50);
		if (base_id == this->_myCamp) {
			tempScene->getPower()->increasePower((powerPlant->getElect()));
			tempScene->getMoney()->spendMoney(powerPlant->getGold());
		}
	}
	else if (new_building_type == "S") {
		SoldierBase* soldierBase = SoldierBase::create("soldierBase/soldierBase_23.png");
		soldierBase->setUnitID(new_building_id);
		soldierBase->setCampID(base_id);
		if (soldierBase->getCampID() == REDCAMP) {
			soldierBase->setColor(Color3B(221, 160, 221));
		}
		else {
			soldierBase->setColor(Color3B(65, 105, 225));
		}
		soldierBase->addIdCount();
		soldierBase->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
		if (base_id == REDCAMP) {
			BuildingBase::setRedIsBuilt(true);
		}
		else {
			BuildingBase::setBlueIsBuilt(true);
		}
		soldierBase->Build();
		TiledMap::newMapGrid(tiledLocation, new_building_id, soldierBase->getRange(), FIX_HEIGHT);
		TiledMap::newMapId(new_building_id, soldierBase);
		soldierBase->setTiledPosition(tiledLocation);
		//							TiledMap::setUnpass(tiledLocation, soldierBase->getRange());
		static_cast<TMXTiledMap*>(base->getParent())->addChild(soldierBase, 50);
		if (base_id == this->_myCamp) {
			tempScene->getPower()->spendPower(soldierBase->getElect());
			tempScene->getMoney()->spendMoney(soldierBase->getGold());
		}
	}
	else if (new_building_type == "W") {
		WarFactory* warFactory = WarFactory::create("tankBase/tankbuilding_23.png");
		warFactory->setUnitID(new_building_id);
		warFactory->setCampID(base_id);
		if (warFactory->getCampID() == REDCAMP) {
			warFactory->setColor(Color3B(221, 160, 221));
		}
		else {
			warFactory->setColor(Color3B(65, 105, 225));
		}
		warFactory->addIdCount();
		warFactory->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
		if (base_id == REDCAMP) {
			BuildingBase::setRedIsBuilt(true);
		}
		else {
			BuildingBase::setBlueIsBuilt(true);
		}
		warFactory->Build();

		TiledMap::newMapGrid(tiledLocation, new_building_id, warFactory->getRange(), FIX_HEIGHT);
		TiledMap::newMapId(new_building_id, warFactory);
		warFactory->setTiledPosition(tiledLocation);
		//				TiledMap::setUnpass(tiledLocation, warFactory->getRange());
		static_cast<TMXTiledMap*>(base->getParent())->addChild(warFactory, 50);
		if (base_id == this->_myCamp) {
			tempScene->getPower()->spendPower(warFactory->getElect());
			tempScene->getMoney()->spendMoney(warFactory->getGold());
		}
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
		if (dog->getCampID() == REDCAMP) {
			dog->setColor(Color3B(255, 0, 0));
		}
		else {
			dog->setColor(Color3B(65, 105, 225));
		}
		dog->addIdCount();
		dog->setAutoAttack(true);
		if (nodeLocation.x < plant->getPosition().x) {
			dog->setFlippedX(true);
		}
		auto tiledLocation = tempTiledMap->tileCoordForPosition(nodeLocation);
		dog->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
		if (base_id == REDCAMP) {
			Dog::setRedIsCreated(true);
		}
		else {
			Dog::setBlueIsCreated(true);
		}
		dog->Create(plant);
		dog->schedule(schedule_selector(FighterUnitBase::autoAttack), 1);
		//		TiledMap::setUnpass(tiledLocation);
		//		auto tiledLocation = tempScene->tileCoordForPosition(nodeLocation);
		TiledMap::newMapGrid(tiledLocation, dog->getUnitID());
		TiledMap::newMapId(dog->getUnitID(), dog);
		dog->setAnchorPoint(Vec2(0, 0.5));
		dog->setTiledPosition(tiledLocation);
		this->getUnitVector().push_back(dog);
		static_cast<TMXTiledMap*>(plant->getParent())->addChild(dog, 200);
		//tempScene->getVectorDogs().pushBack(dog);
		if (base_id == this->_myCamp) {
			tempScene->getMoney()->spendMoney(dog->getGold());
		}
	}
	else if (new_unit_type == "s") {
		Soldier* soldier = Soldier::create("soldierRun/soldierstand.png");
		soldier->setUnitID(new_unit_id);
		soldier->setCampID(base_id);
		if (soldier->getCampID() == REDCAMP) {
			soldier->setColor(Color3B(255, 0, 0));
		}
		else {
			soldier->setColor(Color3B(65, 105, 225));
		}
		soldier->addIdCount();
		soldier->setAutoAttack(true);
		if (nodeLocation.x < plant->getPosition().x) {
			soldier->setFlippedX(true);
		}
		auto tiledLocation = tempTiledMap->tileCoordForPosition(nodeLocation);
		soldier->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
		if (base_id == REDCAMP) {
			Soldier::setRedIsCreated(true);
		}
		else {
			Soldier::setBlueIsCreated(true);
		}
		soldier->Create(plant);
		soldier->schedule(schedule_selector(FighterUnitBase::autoAttack), 1);
		//		TiledMap::setUnpass(tiledLocation);
		static_cast<TMXTiledMap*>(plant->getParent())->addChild(soldier, 200);
		//		auto tiledLocation = tempScene->tileCoordForPosition(nodeLocation);
		TiledMap::newMapGrid(tiledLocation, soldier->getUnitID());
		TiledMap::newMapId(soldier->getUnitID(), soldier);
		soldier->setAnchorPoint(Vec2(0, 0.5));
		this->getUnitVector().push_back(soldier);
		soldier->setTiledPosition(tiledLocation);
		//	tempScene->getVectorSoldiers().pushBack(soldier);
		if (base_id == this->_myCamp) {
			tempScene->getMoney()->spendMoney(soldier->getGold());
		}
	}
	else if (new_unit_type == "t") {
		Tank* tank = Tank::create("tank/tank0.png");
		tank->setUnitID(new_unit_id);
		tank->setCampID(base_id);
		if (tank->getCampID() == REDCAMP) {
			tank->setColor(Color3B(221, 160, 221));
		}
		else {
			tank->setColor(Color3B(65, 105, 225));
		}
		tank->addIdCount();
		tank->setAutoAttack(true);
		tank->setScale(0.4f);
		if (nodeLocation.x < plant->getPosition().x) {
			tank->setFlippedX(true);
		}
		auto tiledLocation = tempTiledMap->tileCoordForPosition(nodeLocation);
		tank->setPosition(Vec2(nodeLocation.x, nodeLocation.y));
		if (base_id == REDCAMP) {
			Tank::setRedIsCreated(true);
		}
		else {
			Tank::setBlueIsCreated(true);
		}
		tank->Create(plant);
		tank->schedule(schedule_selector(FighterUnitBase::autoAttack), 1);
		//		TiledMap::setUnpass(tiledLocation);
		static_cast<TMXTiledMap*>(plant->getParent())->addChild(tank, 200);
		//		auto tiledLocation = tempScene->tileCoordForPosition(nodeLocation);
		TiledMap::newMapGrid(tiledLocation, tank->getUnitID());
		TiledMap::newMapId(tank->getUnitID(), tank);
		tank->setAnchorPoint(Vec2(0, 1.0));
		this->getUnitVector().push_back(tank);
		tank->setTiledPosition(tiledLocation);
		//	tempScene->getVectorSoldiers().pushBack(soldier);
		if (base_id == this->_myCamp) {
			tempScene->getMoney()->spendMoney(tank->getGold());
		}
	}
}

void UnitManager::updateMessage(float delta) {
	std::vector<GameMessage> orders;
	std::string packages;
	//package all the message
	packages = msgs->combineMessagesToString(this->getMessages());
	if (packages == CLIENT_ERROR || packages == CLIENT_CANCEL) {
		//To do
	}
	getSocketClient()->send_game_message(this->getMessages());
	//unpackage the received message
	orders = getSocketClient()->get_game_messages();
	//match the message and implement functions
	for (unsigned int i = 0; i < orders.size(); i++) {
		//read the path_points
		std::vector<cocos2d::Vec2> path_points;
		for (int j = 0; j < orders[i].mutable_grid_path()->grid_point_size(); j++) {
			int x = orders[i].mutable_grid_path()->mutable_grid_point(j)->x();
			int y = orders[i].mutable_grid_path()->mutable_grid_point(j)->y();
			path_points.push_back(Vec2(x, y));
		}
		//if need to create new fighter unit
		if (orders[i].cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_CRTBU) {
			UnitManager::Building(orders[i].unit_0(), orders[i].create_type(), orders[i].base(), orders[i].building(),
				path_points[0]);
		}
		//if attacking happens
		else if (orders[i].cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_ATK) {
			auto attackerId = orders[i].unit_0();
			if (!TiledMap::checkUnitId(attackerId)) {
				continue;
			}
			auto attacker = TiledMap::getUnitById(attackerId);
			if (attacker->isMove()) {
				TiledMap::updateMapGrid(attacker->getTiledPosition(), attacker->getTempPos());
				attacker->setTiledPosition(attacker->getTempPos());
				attacker->stopAllActions();
				attacker->clearAllType();
				attacker->setAttack(true);
			}
			UnitManager::attack(orders[i].unit_0(), orders[i].unit_1(), orders[i].damage());
			UnitManager::attackEffect(orders[i].unit_0(), orders[i].unit_1());
		}
		//if need to build new building
		else if (orders[i].cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_CRTBD) {
			UnitManager::NewUnitCreate(orders[i].unit_0(), orders[i].create_type(), orders[i].base(), orders[i].building(),
				path_points[0]);
		}
		//if unit needs to move
		else if (orders[i].cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_MOV) {
			//erase the repeated end_point
			path_points.erase(path_points.cend() - 1);
			if (path_points.empty()) {
				continue;
			}
			auto id = orders[i].unit_0();
			if (!TiledMap::checkUnitId(id)) {
				continue;
			}
			auto player = TiledMap::getUnitById(id);
			player->setMove(true);
			auto pos = path_points.at(0);
			PathArithmetic* path_finder = PathArithmetic::create();
			auto tempMap = _tiled_Map;
			if (!TiledMap::checkPass(pos)){
				pos = tempMap->findFreeNear(pos);
			}
			log("%f ,%f", pos.x, pos.y);
			auto tiled_pos = tempMap->tileCoordForPosition(player->getPosition());
			path_finder->initPathArithmetic(tempMap, tiled_pos, pos);
			path_finder->findPath();
			auto path = path_finder->getPath();
			if (player->getTargetPos().x != -1) {
				if (pos != player->getTargetPos() && player->getTiledPosition() != player->getTargetPos()) {
					if (!TiledMap::checkPass(player->getTargetPos())) {
//						log("SetPass %f,%f\n", this->getTargetPos().x, this->getTargetPos().y);
						TiledMap::setPass(player->getTargetPos());
					}
				}
			}
			player->setTargetPos(pos);
			TiledMap::setUnpass(pos);
			UnitManager::playerMoveWithWayPoints(orders[i].unit_0(), path, pos);
		}

	}
	//clear for new messages
	this->getMessages().clear();

}

void UnitManager::checkWinOrLose(int win) {
	auto tempScene = static_cast<GameScene*>(this->getParent());
	//this->unschedule(schedule_selector(UnitManager::updateMessage));
	std::vector<FighterUnitBase*> tempVector = this->getUnitVector();
	for (int i = 0; i < tempVector.size(); i++) {
		tempVector.at(i)->unschedule(schedule_selector(FighterUnitBase::autoAttack));
	}
	tempScene->getMoney()->unschedule(schedule_selector(Money::updateMoney));
	tempScene->WinOrLose(win);
}