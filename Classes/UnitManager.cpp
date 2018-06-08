#include "UnitManager.h"
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
	auto vect = _base->getSprite()->getContentSize();
	auto range = _base->getMinRange();
	auto tiledPos = _tiled_Map->tileCoordForPosition(pos); //change the OpenGL coordinate to TiledMap
	tiledPos.y -= 1;
	_base->setBuildId(_base->getIdCount());
	_base->addIdCount();
	TiledMap::newMapGrid(tiledPos, _base->getBuildId(),/* _base->getMinRange()*/0);
	TiledMap::newMapId(_base->getBuildId(), _base);
	//TODO set the camera to the Base
	_tiled_Map->getTiledMap()->addChild(_base,100);
	_tiled_Map->getTiledMap()->setPosition(0-_base->getPositionX()+vect.width *2
		,0-_base->getPositionY() + vect.height * 1.5	);
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
	//judge if there is a unit in the Grid  判断瓦片上有没有单位
	if (TiledMap::checkMapGrid(tiledLocation)) {
		auto id = TiledMap::getUnitIdByPosition(tiledLocation);
		auto tempSprite = TiledMap::getUnitById(id);
		//if the tempSprite is the enemy  如果选中的精灵是敌人
		if (tempSprite->getCampID() != this->getBase()->getCampID()) {
			//if > 1 what in the vector isn't a building   如果SelectVector的容量>1,那么它一定不会包含建筑
			if (TiledMap::checkSize() > 1) {
				for (auto temp : *TiledMap::getSelectedVector()) {
					//if the enemy is in the attack range 如果敌人在攻击范围内
					if (temp->judgeAttack(tiledLocation)) {
						//TODO function attack

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
						}
						else {
							//TODO function tracing
						}
					}
				}
			}
		}
		//if not, then clear up the vector and then push the new one
		//如果选中的精灵不是敌人,那么清空vector,并把此次点击的精灵加入进去
		else
		{
			TiledMap::clearUp();
			TiledMap::newVectorUnit(tempSprite);
		}
	}
	//if not 如果点到的是空地
	else {
		//check the vector and judge if there is a building in it 
		//检查vector,看是否只存了一个建筑
		if (TiledMap::checkSize()) {
			//if yes, clear up
			if (TiledMap::getSelectedVector()->at(0)->isBuilding()) {
				TiledMap::clearUp();
			}
			//if not, call all the unit in the vector to find a path and move to the Position
			else {
				//TODO function to move to the Position
				for (auto temp : *TiledMap::getSelectedVector()) {
					temp->moveTo(touch_point);
					TiledMap::updateMapGrid(temp->getTiledPosition(), tiledLocation);
					temp->setTiledPosition(tiledLocation);
				}
			}
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