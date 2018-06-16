#include "UnitManager.h"
#include "time.h"
#include "SimpleAudioEngine.h"
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
	_base->setTiledPosition(tiledPos);
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
				/*	if (temp->judgeAttack(tiledLocation) && TiledMap::checkMapGrid(tiledLocation)) {
						temp->stopAllActions();
						msgs->newAttackMessage(temp->getUnitID(), enemy->getUnitID(), temp->getAttack());
						attackEffect(temp, enemy);
						attack(temp, enemy);


						if (temp->judgeAttack(tiledLocation)) {
							//TODO function attack
						//	attack(temp, tempSprite);
						}
						else {
							//TODO function tracing
						}
					}*/

					//Set the Type to Attack
					temp->stopAllActions();
					temp->clearAllType();
					temp->setAttack(true);
					temp->setTargetID(enemy->getUnitID());
					msgs->newAttackMessage(temp->getUnitID(), enemy->getUnitID(), temp->getAttack());
					attack(temp->getUnitID(), enemy->getUnitID(), temp->getAttack());

				}
			}
			else {
				//judge the one in vector is building or not 判断在vector里面的唯一一个元素是不是建筑
				if (TiledMap::checkSize() == 1) {
					auto temp = TiledMap::getSelectedVector()->at(0);
					//if not
					if (!temp->isBuilding()) {
					/*
						if (temp->judgeAttack(tiledLocation)) {
						//	attack(temp, tempSprite);
							//TODO function attack
							msgs->newAttackMessage(temp->getUnitID(), enemy->getUnitID(), temp->getAttack());
							temp->stopAllActions();
							attackEffect(temp, enemy);
							attack(temp, enemy);
								
						}
						else {
							//TODO function tracing
						}*/
						//Set the Type to Attack
						temp->stopAllActions();
						temp->clearAllType();
						temp->setAttack(true);
						auto id = enemy->getUnitID();
						temp->setTargetID(id);
						msgs->newAttackMessage(temp->getUnitID(), enemy->getUnitID(), temp->getAttack());
						attack(temp->getUnitID(), enemy->getUnitID(), temp->getAttack());
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
							msgs->newMoveMessage(temp->getUnitID(), path, tiledLocation);
							temp->clearAllType();
							temp->setMove(true);
							playerMoveWithWayPoints(temp->getUnitID(), path, tiledLocation);
						}
						else {
							temp->stopAllActions();
							msgs->newMoveMessage(temp->getUnitID(), path, tiledLocation);

							temp->clearAllType();
							temp->setMove(true);
							playerMoveWithWayPoints(temp->getUnitID(), path, tiledLocation);
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
	if (player->getTargetPos().x != -1) {
		if (player->getTiledPosition() != player->getTargetPos()) {
			if (!TiledMap::checkPass(player->getTargetPos())) {
				TiledMap::setPass(player->getTargetPos());
			}
		}
	}
	player->setTargetPos(tiledLocation);
	TiledMap::setUnpass(tiledLocation);
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
		speed = SOIDIER_SPEED;
		break;
	case 'd':
		animate = player->getAnimateByName("dogRun/dog", 0.2, 5);
		speed = DOG_SPEED;
		break;
	case 't':
		animate = player->getAnimateByName("tank/tank", 0.2, 7);
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
			player->setTexture("unit/FighterUnit.png");
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
		if (i == 0) {
		//	TiledMap::setUnpass(tiledLocation);
		}
		Vec2 openGL_point = _tiled_Map->locationForTilePos(path_points[i]);
			MoveTo* moveTo = MoveTo::create(speed, openGL_point);
		//	auto action = Spawn::create(moveTo);
			actionVector.pushBack(moveTo);
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
	auto player = TiledMap::getUnitById(attacker_id);
	auto enemy = TiledMap::getUnitById(under_attack_id);
	attackEffect(player, enemy);
	auto attackNumber = player->getAttack();
	//decrease the Hp
	enemy->setLifeValue(enemy->getLifeValue() - attackNumber);
	if (enemy->getLifeValue() <= 0 && TiledMap::checkMapGrid(enemy->getTiledPosition())){
		if (enemy->isBuilding()) {
			auto callFunc = CallFunc::create([=] {
				destroyEffect(enemy, true);
				auto tiledLocation = _tiled_Map->tileCoordForPosition(enemy->getPosition());
				TiledMap::removeMapGrid(tiledLocation, enemy->getFixModel());
				TiledMap::removeMapId(enemy->getUnitID());
				_tiled_Map->getTiledMap()->removeChild(enemy);
			});
			this->runAction(callFunc);
		}
		else {
			auto callFunc = CallFunc::create([=] {
				destroyEffect(enemy, false);
				TiledMap::removeMapGrid(enemy->getTiledPosition());
				TiledMap::removeMapId(enemy->getUnitID());
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



void UnitManager::autoAttack(float dt) {
	if (_unit_Vector.size() > 0) {
		for (auto i = 0; i < _unit_Vector.size(); i++) {
			auto pos = _unit_Vector.at(i)->searchEnemy();
			if (pos.x != -1) {
				auto id = TiledMap::getUnitIdByPosition(pos);
				auto enemy = TiledMap::getUnitById(id);
				attack(_unit_Vector.at(i)->getUnitID(), enemy->getUnitID(), _unit_Vector.at(i)->getAttack());
				attackEffect(_unit_Vector.at(i), enemy);
			}
		}
	}
}

/*
void UnitManager::attack(Unit *player, Unit *target) {
	/*change the direction of the unit according to the target position
	Vec2 tarPos = _tiled_Map->locationForTilePos(target->getPosition());
	Vec2 myPos = _tiled_Map->locationForTilePos(player->getPosition());
	float angle = atan2((tarPos.y - myPos.y), (tarPos.x - myPos.x)) * 180 / 3.14159;
	if (tarPos.x < myPos.x) {
		player->setFlippedX(true);
		player->setRotation(angle - 180);
	}
	else {
		player->setRotation(angle);
	}
	if (player->getType() == 's') {
		player->stopAllActions();
		player->setTexture("soldierAttack/soldier_attack.png");
		auto fire = ParticleFire::create();
		player->addChild(fire);
		fire->setPosition(Vec2(30, 0));
		/*change the direction of the unit according to the target position
		
		target->setLifeValue((target->getLifeValue()) - player->getAttack());
		if (target->getHP() != nullptr) {
			target->getHP()->setPercent(target->getHPInterval()*target->getLifeValue());
		}
		auto remove = Sequence::create(
			DelayTime::create(0.2f),
			CallFunc::create([=] {
			player->removeChild(fire,true);
		}), NULL);
		player->runAction(remove);
	}
	else if (player->getType() == 't') {
		player->stopAllActions();
		player->setTexture("tank/tank0.png");
		auto fire = ParticleFire::create();
		fire->setScale(0.3);
		_tiled_Map->getTiledMap()->addChild(fire, 40);
		fire->setPosition(player->getPosition());
>>>>>>> dev_L'sbranch
*/
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