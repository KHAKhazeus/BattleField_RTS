#include "TiledMap.h"

//initial the static member
std::map<Grid*,int> TiledMap::_gridAndId_Map;
std::vector<std::vector<Grid*>> TiledMap::_grid_Vector;


bool TiledMap::init() {
	_tiled_Map = TMXTiledMap::create("map/LostTemple.tmx");
	this->addChild(_tiled_Map);
	return _tiled_Map;
}

void TiledMap::setGridVector() {
	//get the Collidabel Layer
	for (int i = 0; i < _tiled_Map->getMapSize().width; i++) {
		std::vector<Grid*> inner;
		for (int j = 0; j < _tiled_Map->getMapSize().height; j++) {
			auto grid = Grid::create(i, j);
			inner.push_back(grid);
		}
		_grid_Vector.push_back(inner);
	}


	_collidable = _tiled_Map->getLayer("Collidable");
	for (int i = 0; i < _tiled_Map->getMapSize().width; i++) {
		auto inner = _grid_Vector.at(i);
		for (int j = 0; j < _tiled_Map->getMapSize().height; j++) {
			auto grid = inner.at(j);
			Vec2 position = Vec2(i, j);
			int tileGid = _collidable->getTileGIDAt(position);
			if (tileGid) {
				Value properites = _tiled_Map->getPropertiesForGID(tileGid);
				auto map = properites.asValueMap();
				auto value = map.at("collidable").asString();
				if (value.compare("true") == 0) {
					grid->setPass(false);
		//DEBUG		log("%d %d", i, j);
				}
			}
		}
	}
}

bool TiledMap::checkPass(Vec2 pos) {
	auto x = static_cast<int> (pos.x);
	auto y = static_cast<int> (pos.y);
	auto grid = _grid_Vector.at(x).at(y);
	return grid->isPass();
}

void TiledMap::newMapGrid(Vec2 newPos,int id) {
	auto x = static_cast<int> (newPos.x);
	auto y = static_cast<int> (newPos.y);
	auto grid = _grid_Vector.at(x).at(y);
	_gridAndId_Map.insert({ grid,id });
	grid->setPass(false);
}

void TiledMap::updateMapGrid(Vec2 oldPos, Vec2 newPos) {
	auto x = static_cast<int> (oldPos.x);
	auto y = static_cast<int> (oldPos.y);
	auto oldGrid = _grid_Vector.at(x).at(y);
	int id = _gridAndId_Map.at(oldGrid);
	_gridAndId_Map.erase(oldGrid);
	auto newX = static_cast<int> (newPos.x);
	auto newY = static_cast<int> (newPos.y);
	auto newGrid = _grid_Vector.at(newX).at(newY);
	_gridAndId_Map.insert({newGrid,id});
	newGrid->setPass(false);
	oldGrid->setPass(true);
}

void TiledMap::removeMapGrid(Vec2 Pos) {
	auto x = static_cast<int> (Pos.x);
	auto y = static_cast<int> (Pos.y);
	auto grid = _grid_Vector.at(x).at(y);
	_gridAndId_Map.erase(grid);
	grid->setPass(true);
}

bool TiledMap::checkMapGrid(Vec2 Pos) {
	auto x = static_cast<int> (Pos.x);
	auto y = static_cast<int> (Pos.y);
	auto grid = _grid_Vector.at(x).at(y);
	if (_gridAndId_Map.count(grid) != 0) {
		return true;
	}
	return false;
}

int TiledMap::getUnitId(Vec2 Pos) {
	auto x = static_cast<int> (Pos.x);
	auto y = static_cast<int> (Pos.y);
	auto grid = _grid_Vector.at(x).at(y);
	return _gridAndId_Map.at(grid);
}

TMXObjectGroup* TiledMap::getObjectGroup(std::string layername) {
	return _tiled_Map->getObjectGroup(layername);
}

TMXTiledMap* TiledMap::getTiledMap() {
	return _tiled_Map;
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



