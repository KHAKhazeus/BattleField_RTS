#include "TiledMap.h"

//initial the static member
std::map<Grid*,int> TiledMap::_gridAndId_Map;
std::vector<Grid*> TiledMap::_collidable_Vector;


bool TiledMap::init() {
	_tiled_Map = TMXTiledMap::create("map/LostTemple.tmx");
	this->addChild(_tiled_Map);
	return _tiled_Map;
}

void TiledMap::setCollidableVector() {
	//get the Collidabel Layer
	_collidable = _tiled_Map->getLayer("Collidable");
	for (int j = 0; j < _tiled_Map->getMapSize().height; j++) {
		for (int i = 0; i < _tiled_Map->getMapSize().width; i++) {
			Vec2 position = Vec2(i, j);
			int tileGid = _collidable->getTileGIDAt(position);
			if (tileGid) {
				Value properites = _tiled_Map->getPropertiesForGID(tileGid);
				auto map = properites.asValueMap();
				auto value = map.at("collidable").asString();
				if (value.compare("true") == 0) {
					auto grid = Grid::create(i, j);
					grid->setPass(false);
					log("%d %d", i, j);
					_collidable_Vector.push_back(grid);
					//	
				}
			}
		}
	}
}

void TiledMap::newMapGrid(Grid *newPos,int id) {
	_gridAndId_Map.insert({ newPos,id });
	newPos->setPass(false);
}

void TiledMap::updateMapGrid(Grid *oldPos, Grid *newPos) {
	int id = _gridAndId_Map.at(oldPos);
	_gridAndId_Map.erase(oldPos);
	_gridAndId_Map.insert({newPos,id});
	newPos->setPass(false);
	oldPos->setPass(true);
}

void TiledMap::removeMapGrid(Grid* Pos) {
	_gridAndId_Map.erase(Pos);
	Pos->setPass(true);
}

bool TiledMap::checkMapGrid(Grid * Pos) {
	if (_gridAndId_Map.count(Pos) != 0) {
		return true;
	}
	return false;
}

int TiledMap::getUnitId(Grid * Pos) {
	return _gridAndId_Map.at(Pos);
}

TMXObjectGroup* TiledMap::getObjectGroup(std::string layername) {
	return _tiled_Map->getObjectGroup(layername);
}

/*Grid* SearchEnemy() {
	auto rect = GridRect::create(curpos - rang / 2, curpos - range / 2, range, range);
	for (auto i = rect->getX(); i = rect->getWidth(); i++) {
		for (auto j = rect->getY(); j = rect->getHeight(); j++) {
			auto grid = Grid::create(i, j);
			if (TiledMap::checkMapGrid(grid) {
				if (TiledMap::getUnitId(grid) != campId) {
					return grid;
				}
			}
		}
	}
	return nullptr;
}*/



