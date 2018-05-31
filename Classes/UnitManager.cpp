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