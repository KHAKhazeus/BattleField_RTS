#include "UnitManager.h"
#include "time.h"
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
						Vec2 from = _tiled_Map->tileCoordForPosition(temp->getPosition());
						if (!_tiled_Map->checkPass(tiledLocation)) {
							tiledLocation = _tiled_Map->findFreeNear(tiledLocation);
						}
						PathArithmetic* path_finder = PathArithmetic::create();
						path_finder->initPathArithmetic(_tiled_Map, from, tiledLocation);
						path_finder->findPath();
						auto path = path_finder->getPath();
						if (temp->getNumberOfRunningActions() == 0) {
							temp->clearAllType();
							temp->setMove(true);
							playerMoveWithWayPoints(temp, touch_point, path);
						}
						else {
							temp->stopAllActions();
							temp->clearAllType();
							temp->setMove(true);
							playerMoveWithWayPoints(temp, touch_point, path);
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

void UnitManager::playerMoveWithWayPoints(Unit* player, Vec2 position, std::vector<Vec2> path) {
	player->stopAllActions();
	if (path.empty()) {
		return;
	}
	auto tiledLocation = path.back();
	/*change the direction of the unit according to the target position*/
	Vec2 tarPos = _tiled_Map->locationForTilePos(position);
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
	switch (player->getType())
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
		log("%s", "Done!");
		if (player->isMove()) {
			player->setMove(false);
			player->setAutoAttack(true);
		}
	});
/*	for (int i = 0; i < path.size(); i++) {
		Vec2 openGL_point = _tiled_Map->locationForTilePos(path[i]);
		if (TiledMap::checkPass(path[i])1) {
			if (i > 0) {
				player->stopActionByTag(i - 1);
			}
			auto moveTo = MoveTo::create(0.4f, openGL_point);
			moveTo->setTag(i);
			TiledMap::updateMapGrid(player->getTiledPosition(), path[i]);
			player->setTiledPosition(path[i]);
			player->runAction(moveTo);
		}
		else {
			auto path_finder = PathArithmetic::create();
			if (!TiledMap::checkPass(tiledLocation)) {
				tiledLocation = _tiled_Map->findFreeNear(tiledLocation);
			}
			path_finder->initPathArithmetic(_tiled_Map, player->getTiledPosition(), tiledLocation);
			path_finder->findPath();
			auto path = path_finder->getPath();
			playerMoveWithWayPoints(player, tiledLocation, path);
			return;
		}
	}*/
	Sequence *sequence;
	for (int i = 0; i < path.size(); i++) {
		if (i == 0) {
			TiledMap::setUnpass(tiledLocation);
		}
		Vec2 openGL_point = _tiled_Map->locationForTilePos(path[i]);
		//auto moveTo = MoveTo::create(0.4f, openGL_point);
	//	moveTo->setTag(i+1);
		auto callfuncPosition = CallFunc::create([=] {
			auto oldPos = player->getTiledPosition();
		/*	if (i >= 1 && !TiledMap::checkMapGrid(path[i - 1])) {
				if (player->getTiledPosition() != path[i - 1]) {
					if (!TiledMap::checkMapGrid(player->getTiledPosition())) {
						TiledMap::newMapGrid(path[i], player->getUnitID());
					}
					else {
						TiledMap::updateMapGrid(player->getTiledPosition(), path[i]);
					}
				}
			}*/
			TiledMap::updateMapGrid(player->getTiledPosition(), path[i]);
			player->setTiledPosition(path[i]);
			auto player_id = player->getUnitID();
			log("%d    %d\n", player_id,i);
			log("%f.%f change to %f.%f\n", oldPos.x,oldPos.y, path[i].x,path[i].y);
			if (i < path.size() - 1) {
				if((TiledMap::checkMapGrid(path[i+1])))
				{
					player->stopAllActions();
					log("STOP!");
					return;
				}
			}
			/*if (i < path.size() - 2) {
				int count = 0;
				if (TiledMap::checkPass(path[i + 1])){
					TiledMap::setUnpass(path[i + 1]);
				}
				if (count == 100000 && !TiledMap::checkPass(path[i + 1])) {
					auto location = path.back();
					if (!_tiled_Map->checkPass(location)) {
						location = _tiled_Map->findFreeNear(location);
					}
					PathArithmetic* path_finder = PathArithmetic::create();
					path_finder->initPathArithmetic(_tiled_Map, path[i], location);
					path_finder->findPath();
					auto path = path_finder->getPath();
					playerMoveWithWayPoints(player, location, path);
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

void UnitManager::attack(Unit* player, Unit* enemy) {
	auto attackNumber = player->getAttack();
	//decrease the Hp
	enemy->setLifeValue(enemy->getLifeValue() - attackNumber);
	if (enemy->getLifeValue() <= 0 && TiledMap::checkMapGrid(enemy->getTiledPosition())){
		if (enemy->isBuilding()) {
			auto tiledLocation = _tiled_Map->tileCoordForPosition(enemy->getPosition());
			TiledMap::removeMapGrid(tiledLocation, enemy->getFixModel());
			TiledMap::removeMapId(enemy->getUnitID());
			_tiled_Map->getTiledMap()->removeChild(enemy);
		}
		else {
			TiledMap::removeMapGrid(enemy->getTiledPosition());
			TiledMap::removeMapId(enemy->getUnitID());
			_tiled_Map->getTiledMap()->removeChild(enemy);
			return;
		}
	}
	if (enemy->getHP() != nullptr) {
		enemy->getHP()->setPercent(enemy->getHPInterval() * static_cast<float>(enemy->getLifeValue()));
	}
}

void UnitManager::attackEffect(Unit* player, Unit *enemy) {
	auto type = player->getType();
	auto pos = enemy->getPosition();
	bool isDog = false;
	Sprite* bullet;
	Animate* dogAttack;
	switch (type) {
	case 's':
		player->setTexture("unit/FighterUnit_2.png");
		bullet = Sprite::create("soldierAttack/bullet.png");
		bullet->setPosition(player->getPosition());
		bullet->setScale(0.2);
		bullet->setFlippedX(true);
		_tiled_Map->getTiledMap()->addChild(bullet, 30);
		break;
	case 'd':
		player->setTexture("unit/FighterUnit_1.png");
		dogAttack = player->getAnimateByName("dogAttack/dog_attack", 0.1, 6);
		isDog = true;
		break;
	case 't':
		bullet = Sprite::create("tank/tankBullet.png");
		bullet->setPosition(player->getPosition());
		bullet->setScale(0.4);
		_tiled_Map->getTiledMap()->addChild(bullet, 30);
		break;
	default:
		break;
	}
	Vec2 tarPos = _tiled_Map->locationForTilePos(enemy->getPosition());
	Vec2 myPos = _tiled_Map->locationForTilePos(player->getPosition());
	float angle = atan2((tarPos.y - myPos.y), 
		(tarPos.x - myPos.x)) * 180 / 3.14159;
	if (player->isFlippedX()) {
		player->setFlippedX(false);
	}
	if (enemy->getPosition().x < player->getPosition().x) {
		player->setFlippedX(true);
		player->setRotation(angle - 180);
	}
	else {
		player->setRotation(angle);
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
				attack(_unit_Vector.at(i), enemy);
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

		target->setLifeValue((target->getLifeValue()) - player->getAttack());
		if (target->getHP() != nullptr) {
			target->getHP()->setPercent(target->getHPInterval()*target->getLifeValue());
		}
		auto remove = Sequence::create(
			DelayTime::create(0.2f),
			CallFunc::create([=] {
			_tiled_Map->getTiledMap()->removeChild(fire, true);
		}), NULL);
		player->runAction(remove);
	}
	else if (player->getType() == 't') {
		player->stopAllActions();
		player->setTexture("dogAttack/dog_attack0.png");
		auto animate = player->getAnimateByName("dogAttack/dog_attack", 0.2f, 6);
		auto repeateAnimate = RepeatForever::create(animate);
		player->runAction(repeateAnimate);
		ParticleSystem *bloodSpurts  = ParticleSystemQuad::create("bloodSpurts.plist");
		// 设置粒子效果位置独立 particle's effection is independent
		bloodSpurts->setPositionType(ParticleSystem::PositionType::FREE);
		// 粒子效果完成后自动删除 if finished automatically delete itself
		bloodSpurts->setAutoRemoveOnFinish(true);
		bloodSpurts->setPosition(Vec2(10, 0));
		player->addChild(bloodSpurts);

		target->setLifeValue((target->getLifeValue()) - player->getAttack());
		if (target->getHP() != nullptr) {
			target->getHP()->setPercent(target->getHPInterval()*target->getLifeValue());
		}
		auto remove = Sequence::create(
			DelayTime::create(0.2f),
			CallFunc::create([=] {
			player->removeChild(bloodSpurts, true);
		}), NULL);
		player->runAction(remove);
	}
}
*/