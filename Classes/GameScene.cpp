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
	

	_tiled_Map = TiledMap::create();
	_tiled_Map->setCollidableVector();
	this->addChild(_tiled_Map);
	
	
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


	/*_base = Base::create();
	_base->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(_base);*/
	_unit_Manager = UnitManager::create(_tiled_Map);
	_unit_Manager->setPosition(Vec2::ZERO);
	this->addChild(_unit_Manager);
	_unit_Manager->initBase();


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
	//call the function getTiledMap to get the TMXTiledMap *
	auto tiledMap = _tiled_Map->getTiledMap();
	int x = static_cast<int>
		(position.x / (tiledMap->getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
	float pointHeight = tiledMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	int y = static_cast<int>
		((tiledMap->getMapSize().height * pointHeight - position.y) / pointHeight);
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
		//TODO call the scene of Setting;
		break;
	case EventKeyboard::KeyCode::KEY_H:
		//TODO go back to the base
		break;
	}
}