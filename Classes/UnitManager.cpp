#include "UnitManager.h"
#include "time.h"
bool UnitManager::init(TiledMap * tiledMap) {
	_building = 1;
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
						attackEffect(temp, enemy);
						attack(temp, enemy);
					}
					else {
						//TODO function tracing
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
							//TODO function attack
							temp->stopAllActions();
							attackEffect(temp, enemy);
							attack(temp, enemy);
								
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
							playerMoveWithWayPoints(temp, touch_point, path);
						}
						else {
							temp->stopAllActions();
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
	Vector<FiniteTimeAction*> actionVector;
	Animate* animate;
	switch (player->getType())
	{
	case 's':
		animate = player->getAnimateByName("soldierRun/soldier", 0.2f, 7);
		break;
	case 'd':
		animate = player->getAnimateByName("dogRun/dog", 0.2f, 10);
		break;
	case 't':
		animate = player->getAnimateByName("tank/tank", 0.2f, 7);
	default:
		break;
	}
	auto repeatanimate = RepeatForever::create(animate);
	player->runAction(repeatanimate);
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
	});
	/*	for (int i = 0; i < path.size(); i++) {
			Vec2 openGL_point = _tiled_Map->locationForTilePos(path[i]);
			if (/*TiledMap::checkPass(path[i])1) {
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
			}*/
	for (int i = 0; i < path.size(); i++) {
		if (i == 0) {
			TiledMap::setUnpass(tiledLocation);
		}
		Vec2 openGL_point = _tiled_Map->locationForTilePos(path[i]);
		auto moveTo = MoveTo::create(0.4f, openGL_point);
		moveTo->setTag(i+1);
		auto callfuncPosition = CallFunc::create([=] {
				TiledMap::updateMapGrid(player->getTiledPosition(), path[i]);
				player->setTiledPosition(path[i]);
	/*			if (i < path.size() - 2) {
					int count = 0;
					if (TiledMap::checkPass(path[i + 1])){
						TiledMap::setUnpass(path[i + 1]);
					}
					/*if (count == 100000 && !TiledMap::checkPass(path[i + 1])) {
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
				}*/
			});
		callfuncPosition->setTag(200 + i);
		actionVector.pushBack(moveTo);
		actionVector.pushBack(callfuncPosition);
	}
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

void UnitManager::attack(Unit* player, Unit* enemy) {
	auto attackNumber = player->getAttack();
	//decrease the Hp
	enemy->setLifeValue(enemy->getLifeValue() - attackNumber);
	if (enemy->getLifeValue() <= 0) {
		if (enemy->isBuilding()) {
			auto tiledLocation = _tiled_Map->tileCoordForPosition(enemy->getPosition());
			TiledMap::removeMapGrid(tiledLocation, enemy->getFixModel());
			_tiled_Map->getTiledMap()->removeChild(enemy);
		}
		else {
			TiledMap::removeMapGrid(enemy->getTiledPosition());
			_tiled_Map->getTiledMap()->removeChild(enemy);
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
			bullet->setPosition(player->getPosition() );
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
			bullet->setPosition(player->getPosition() );
			bullet->setScale(0.4);
			_tiled_Map->getTiledMap()->addChild(bullet, 30);
			break;
		default:
			break;
	}
	if (isDog) {
		auto callfuc = CallFunc::create([=] {
			player->setTexture("FighterUnit_1.png");
		});
		auto sequence = Sequence::create(dogAttack,callfuc,NULL);
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

/*	bullet = Sprite::create("tank/tankBullet.png");
	bullet->setPosition(player->getPosition());
	bullet->setScale(0.4);
	_tiled_Map->getTiledMap()->addChild(bullet,30);
	auto moveTo = MoveTo::create(0.5f, pos);
	bullet->runAction(moveTo);
	auto callfuc = CallFunc::create([=] {
		_tiled_Map->getTiledMap()->removeChild(bullet);
	});*/

}
