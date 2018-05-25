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
	_screen_Width = visibleSize.width;
	_screen_Height = visibleSize.height;
	
	//initial the Map
	_tiled_Map = TMXTiledMap::create("map/LostTemple.tmx");
	_tiled_Map->setAnchorPoint(Vec2::ZERO);
	_tiled_Map->setPosition(Vec2::ZERO);
	this->addChild(_tiled_Map,0);

	//push the collidable item to the _colliable Vector
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
					_collidable_Vector.push_back(grid);
				//	
				}
			}
		}
	}

	// set money and power


	//TODO initial the money and power



	//start the update
	this->scheduleUpdate();
	return true;
}

void GameScene::update() {
	//TODO add the concrete code
}

Vec2 GameScene::tileCoordForPosition(Vec2 position) {
	int x = static_cast<int>
		(position.x / (_tiled_Map->getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
	float pointHeight = _tiled_Map->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	int y = static_cast<int>
		((_tiled_Map->getMapSize().height * pointHeight - position.y) / pointHeight);
	return Vec2(x, y);
}

void GameScene::onEnter() {
	Scene::onEnter();
	_money_Image = Sprite::create("ui/Coin.png");
	_money_Image->setPosition(Vec2(_screen_Width*0.8, _screen_Height*0.04));
	_money_Count = Label::createWithSystemFont("10000", "fonts/Arial", 32);
	_money_Count->setPosition(Vec2(_screen_Width*0.9, _screen_Height*0.04));
	this->addChild(_money_Image);
	this->addChild(_money_Count);
	// TODO _power_Image = Sprite::create("");
	_power_Count = Label::createWithSystemFont("5000", "fonts/Arial", 32);
	_power_Count->setPosition(Vec2(_screen_Width*0.9, _screen_Height*0.10));
	//this->addChild(_power_Image);
	this->addChild(_power_Count);
	//TODO
}

void GameScene::onEnterTransitionDidFinish() {
	Scene::onEnterTransitionDidFinish();
	//TODO maybe about audio
}

void GameScene::onExit() {
	Scene::onExit();
	//TODO 
}