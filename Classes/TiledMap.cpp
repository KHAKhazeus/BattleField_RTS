#include "TiledMap.h"

//initial the static member
std::map<Grid*,int> TiledMap::_gridAndId_Map;
std::vector<Vector<Grid*>> TiledMap::_grid_Vector;   
std::map<int, Unit*> TiledMap::_idAndUnit_Map;
std::vector<Unit*> TiledMap::_select_Vector;

int TiledMap::_map_flag = LOSTTEMP;
bool TiledMap::init() {
	if (_map_flag == SNOWMAP) {
		_tiled_Map = TMXTiledMap::create("map/Snow.tmx");
	}	
	else {
		_tiled_Map = TMXTiledMap::create("map/LostTemple.tmx");
	}
	_tiled_Map->setAnchorPoint(Vec2::ZERO);
	this->addChild(_tiled_Map);
	return _tiled_Map;
}

void TiledMap::setGridVector() {
	//get the Collidabel Layer
	for (int i = 0; i < _tiled_Map->getMapSize().width; i++) {
		Vector<Grid*> inner;
		for (int j = 0; j < _tiled_Map->getMapSize().height; j++) {
			auto grid = Grid::create(i, j);
			inner.insert(j,grid);
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
				}
			}
		}
	}
}

bool TiledMap::checkBoundary(Vec2 pos) {
	if (pos.x < 0 || pos.x >= 128 || pos.y < 0 || pos.y >= 128) {
		return false;
	}
	return true;
}

bool TiledMap::checkPass(Vec2 pos) {
	auto x = static_cast<int> (pos.x + 0.1);
	auto y = static_cast<int> (pos.y + 0.1);
	auto grid = _grid_Vector.at(x).at(y);
	return grid->isPass();
}

bool TiledMap::checkPass(int x, int y) {
	auto grid = _grid_Vector.at(x).at(y);
	return grid->isPass();
}

bool TiledMap::checkBuilt(Vec2 pos, int range) {
	auto x = static_cast<int> (pos.x + 0.1);
	auto y = static_cast<int> (pos.y + 0.1);
	for (auto i = x - range; i <= x + range; i++) {
		for (auto j = y - range; j <= y + range; j++) {
//			log("%d %d", i, j);
			if (!checkBoundary(Vec2(i, j))) {
				return false;
			}
			auto judge = checkPass(Vec2(i, j));
			if (!judge) {
				return false;
			}
		}
	}
	return true;
}

bool TiledMap::checkCreate(Vec2 pos) {
	auto x = static_cast<int> (pos.x + 0.1);
	auto y = static_cast<int> (pos.y + 0.1);
	if (!checkBoundary(Vec2(x, y))) {
		return false;
	}
	auto judge = checkPass(Vec2(x, y));
	if (!judge) {
		return false;
	}
	return true;
}
		/*   _gridAndId_Map          API*/

void TiledMap::newMapGrid(Vec2 newPos,int id) {
	auto x = static_cast<int> (newPos.x + 0.1);
	auto y = static_cast<int> (newPos.y + 0.1);
	auto grid = _grid_Vector.at(x).at(y);
	_gridAndId_Map.insert({ grid,id });
	grid->setPass(false);
}

void TiledMap::newMapGrid(Vec2 newPos, int id,int range,int fix_modle) {
	auto x = static_cast<int> (newPos.x + 0.1);
	auto y = static_cast<int> (newPos.y + 0.1);
	switch (fix_modle) {
		case FIX_SQUARE:
			for (auto i = x - range; i <= x + range; i++) {
				for (auto j = y - range; j <= y + range; j++) {
					auto grid = _grid_Vector.at(i).at(j);
					grid->setPass(false);
					_gridAndId_Map.insert({ grid,id });
				}
			}
			break;
		case FIX_HEIGHT:
			for (auto i = x - range; i <= x + range; i++) {
				for (auto j = y - range + 1; j <= y + range - 1; j++) {
					auto grid = _grid_Vector.at(i).at(j);
					grid->setPass(false);
					_gridAndId_Map.insert({ grid,id });
				}
			}
			break;
		default:
			break;
	}
}

void TiledMap::updateMapGrid(Vec2 oldPos, Vec2 newPos) {
	auto id = getUnitIdByPosition(oldPos);
	newMapGrid(newPos,id);
	auto x = static_cast<int> (oldPos.x + 0.1);
	auto y = static_cast<int> (oldPos.y + 0.1);
	auto grid = _grid_Vector.at(x).at(y);
	grid->setPass(true);
	_gridAndId_Map.erase(grid);
}

void TiledMap::removeMapGrid(Vec2 Pos) {
	auto x = static_cast<int> (Pos.x + 0.1);
	auto y = static_cast<int> (Pos.y + 0.1);
	auto grid = _grid_Vector.at(x).at(y);
	auto id = getUnitIdByPosition(Pos);
	removeMapId(id);
	_gridAndId_Map.erase(grid);
	grid->setPass(true);
}

bool TiledMap::checkMapGrid(Vec2 Pos) {
	auto x = static_cast<int> (Pos.x + 0.1);
	auto y = static_cast<int> (Pos.y + 0.1);
	auto grid = _grid_Vector.at(x).at(y);
	if (_gridAndId_Map.count(grid) != 0) {
		return true;
	}
	return false;
}

void TiledMap::removeMapGrid(Vec2 pos, int fix_model) {
	auto x = static_cast<int> (pos.x + 0.1);
	auto y = static_cast<int> (pos.y + 0.1);
	auto id = getUnitIdByPosition(pos);
	auto tempSprite = getUnitById(id);
	auto range = tempSprite->getRange();
	switch (fix_model) {
		case FIX_HEIGHT:
			for (auto i = x - range; i <= x + range; i++) {
				for (auto j = y - range + 1; j <= y + range - 1; j++) {
					auto grid = _grid_Vector.at(i).at(j);
					grid->setPass(true);
					_gridAndId_Map.erase(grid);
				}
			}
			break;
		case FIX_SQUARE:
			for (auto i = x - range; i <= x + range; i++) {
				for (auto j = y - range; j <= y + range; j++) {
					auto grid = _grid_Vector.at(i).at(j);
					grid->setPass(true);
					_gridAndId_Map.erase(grid);
				}
			}
			break;
		default:
			break;
	}
	removeMapId(id);
}



int TiledMap::getUnitIdByPosition(Vec2 Pos) {
	auto x = static_cast<int> (Pos.x + 0.1);
	auto y = static_cast<int> (Pos.y + 0.1);
	auto grid = _grid_Vector.at(x).at(y);
	return _gridAndId_Map.at(grid);
}

/*--------------------------------------------------------*/

TMXObjectGroup* TiledMap::getObjectGroup(std::string layername) {
	return _tiled_Map->getObjectGroup(layername);
}



TMXTiledMap* TiledMap::getTiledMap() {
	return _tiled_Map;
}

void TiledMap::setUnpass(Vec2 Pos, int range) {
	auto x = static_cast<int> (Pos.x + 0.1);
	auto y = static_cast<int> (Pos.y + 0.1);
	for (int i = x - range; i <= x + range; i++) {
		for (int j = y - range; j <= y + range; j++) {
			_grid_Vector.at(i).at(j)->setPass(false);
			//DEBUG 
		//	log("%d %d", i, j);
		}
	}
}

void TiledMap::setUnpass(Vec2 Pos) {
	auto x = static_cast<int> (Pos.x + 0.1);
	auto y = static_cast<int> (Pos.y + 0.1);
	_grid_Vector.at(x).at(y)->setPass(false);
	//DEBUG 
	//	log("%d %d", i, j);

}

void TiledMap::setPass(Vec2 Pos) {
	auto x = static_cast<int> (Pos.x + 0.1);
	auto y = static_cast<int> (Pos.y + 0.1);
	_grid_Vector.at(x).at(y)->setPass(true);
}

Vec2 TiledMap::tileCoordForPosition(Vec2 position) {
	int x = static_cast<int>
		(position.x / (_tiled_Map->getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
	float pointHeight = _tiled_Map->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	int y = static_cast<int>
		((_tiled_Map->getMapSize().height * pointHeight - position.y) / pointHeight);
	return Vec2(x, y);
}

Vec2 TiledMap::locationForTilePos(Vec2 position) {
	int x = (int)(position.x*(_tiled_Map->getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
	float pointHeight = _tiled_Map->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	int y = (int)((_tiled_Map->getMapSize().height * pointHeight) - (position.y * pointHeight));
	return Point(x, y);
}


				/*
					_idAndUnit_Map API
				*/	

void TiledMap::newMapId(int id, Unit* unit) {
	_idAndUnit_Map.insert({ id,unit });
}

void TiledMap::removeMapId(int id) {
	_idAndUnit_Map.erase(id);
}

Unit* TiledMap::getUnitById(int id){
	return _idAndUnit_Map.at(id);
}

bool TiledMap::checkUnitId(int id) {
	if (_idAndUnit_Map.count(id) != 0) {
		return true;
	}
	return false;
}



/*Grid* SearchEnemy() {
	auto rect = GridRect::create(curpos - rang / 2, curpos - range / 2, range, range);
	for (auto i = rect->getX(); i = rect->getWidth(); i++) {
		for (auto j = rect->getY(); j = rect->getHeight(); j++) {
			auto grid = Grid::create(i, j);
			if (TiledMap::checkMapGrid(grid) {
				if (TiledMap::getUnitIdByPosition(grid) != campId) {
					return grid;
				}
			}
		}
	}
	return nullptr;
}*/

/*
			_select_Vector API
*/


void TiledMap::newVectorUnit(Unit* unit) {
	_select_Vector.push_back(unit);
	if (unit->getProgressed()) {
		unit->getHP()->setVisible(true);
	}
}

void TiledMap::clearUp() {
	for (auto i : _select_Vector) {
		i->getHP()->setVisible(false);
	}
	_select_Vector.clear();
	//TODO   can use STL function swap £¿
}

int TiledMap::checkSize() {
	return _select_Vector.size();
}

std::vector<Unit*>* TiledMap::getSelectedVector() {
	return &_select_Vector;
}

Vec2 TiledMap::findFreeNear(Vec2 position) {
	if (checkPass(position)) {
		return position;
	}
	for (int i = 1; i < _tiled_Map->getMapSize().width; i++) {
		for (int j = -i; j <= i; j++) {
			int k = i - abs(j);
			if (TiledMap::checkBoundary(Vec2(position.x + j, position.y + k))
				&& checkPass(position.x + j, position.y + k))
				return Vec2(position.x + j, position.y + k);
			if (TiledMap::checkBoundary(Vec2(position.x + j, position.y - k))
				&& checkPass(position.x + j, position.y - k))
				return Vec2(position.x + j, position.y - k);
		}
	}
	return Vec2(-1, -1);
}