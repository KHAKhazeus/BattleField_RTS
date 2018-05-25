#include "GameScene.h"
Scene* GameScene::createScene() {
	auto scene = Scene::create();
	auto gamescene = GameScene::create();
	scene->addChild(gamescene);
	return scene;
}

bool GameScene::init() {
	if (!Scene::init()) {
		return false;
	}
	//get the screen size
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_screenWidth = visibleSize.width;
	_screenHeight = visibleSize.height;
	
	//initial the Map
	_tiledMap = TMXTiledMap::create("LostTemple.tmx");
//	_tiledMap->setAnchorPoint(Vec2::ZERO);
	_tiledMap->setPosition(Vec2::ZERO);
	this->addChild(_tiledMap,0);

	//push the collidable item to the _colliable Vector
	_collidable = _tiledMap->getLayer("Collidable");
	for (int j = 0; j < _tiledMap->getMapSize().height; j++) {
		for (int i = 0; i < _tiledMap->getMapSize().width; i++) {
			Vec2 position = Vec2(i, j);
			int tileGid = _collidable->getTileGIDAt(position);
			if (tileGid) {
				Value properites = _tiledMap->getPropertiesForGID(tileGid);
				auto map = properites.asValueMap();
				auto value = map.at("collidable").asString();
				if (value.compare("true") == 0) {
					auto grid = Grid::create(i, j);
					grid->setPass(false);
					log("%d %d", i, j);
					_collidableVector.push_back(grid);
				//	
				}
			}
		}
	}

	return true;
}

void GameScene::update() {
	//TODO add the concrete coed
}

Vec2 GameScene::tileCoordForPosition(Vec2 position) {
	int x = static_cast<int>
		(position.x / (_tiledMap->getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
	float pointHeight = _tiledMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	int y = static_cast<int>
		((_tiledMap->getMapSize().height * pointHeight - position.y) / pointHeight);
	return Vec2(x, y);
}