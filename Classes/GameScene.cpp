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
	screen_Width = visibleSize.width;
	screen_Height = visibleSize.height;
	

	_tiledMap = TiledMap::create();
	_tiledMap->setCollidableVector();
	this->addChild(_tiledMap);
	
	auto grid1 = Grid::create(1, 1);
	auto grid2 = Grid::create(100, 100);
	TiledMap::newMapGrid(grid1, 1);
	TiledMap::updateMapGrid(grid1, grid2);
	TiledMap::checkMapGrid(grid2);
	TiledMap::getUnitId(grid2);
	TiledMap::removeMapGrid(grid2);

	
	// set money and power
	_money_Image = Sprite::create("ui/Coin.png");
	_money_Image->setPosition(Vec2(screen_Width*0.83, screen_Height*0.04));
	this->addChild(_money_Image);
    _money = Money::create();
	_money->setPosition(Vec2(screen_Width *0.92, screen_Height*0.04));
	this->addChild(_money);
	_power_Image = Sprite::create("ui/electric.png");
	_power_Image->setPosition(Vec2(screen_Width*0.83, screen_Height*0.10));
	_power_Image->setScale(0.08);
	_power = Power::create();
	_power->setPosition(Vec2(screen_Width*0.92, screen_Height*0.10));
	this->addChild(_power);
	this->addChild(_power_Image);


	_base = Base::create();
	_base->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(_base);
	_money->schedule(schedule_selector(Money::updateMoney), 1);
	_power->schedule(schedule_selector(Power::updatePower), 1);
	


	//TODO initial the money and power



	//start the update
	
	return true;

}

void GameScene::update(float dt) {
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



//Debug


void GameScene::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event) {
	switch (keycode) {
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		//TODO 调出setting画面;
		break;
	case EventKeyboard::KeyCode::KEY_H:
		//TODO 回到主基地
		break;
	}
}