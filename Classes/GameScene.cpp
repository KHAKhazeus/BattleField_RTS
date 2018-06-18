#include "GameScene.h"

std::shared_ptr<SocketServer> GameScene::_socket_server;
std::shared_ptr<SocketClient> GameScene::_socket_client;


Scene* GameScene::createScene(std::shared_ptr<SocketServer> spserver, std::shared_ptr<SocketClient> spclient) {
	auto scene = Scene::create();
	auto gamescene = GameScene::create();
	scene->addChild(gamescene);
	_socket_server = spserver;
	_socket_client = spclient;
	return scene;
}

bool GameScene::init() {
	if (!Scene::init()) {
		return false;
	}
	//get the screen size
/*	auto visibleSize = Director::getInstance()->getVisibleSize();
	_screen_width = visibleSize.width;
	_screen_height = visibleSize.height;
	

	_tiled_Map = TiledMap::create();
	_tiled_Map->setGridVector();
	this->addChild(_tiled_Map);
	
	
	// Set money and power
	_money_Image = Sprite::create("ui/Coin.png");
	_money_Image->setPosition(Vec2(_screen_width*0.83, _screen_height*0.04));
	this->addChild(_money_Image);
    _money = Money::create();
	_money->setPosition(Vec2(_screen_width *0.90, _screen_height*0.04));
	this->addChild(_money);
	_power_Image = Sprite::create("ui/electric.png");
	_power_Image->setPosition(Vec2(_screen_width*0.83, _screen_height*0.12));
	_power_Image->setScale(0.08);
	_power = Power::create();
	_power->setPosition(Vec2(_screen_width*0.90, _screen_height*0.12));
	this->addChild(_power);
	this->addChild(_power_Image);


	_unit_Manager = UnitManager::create(_tiled_Map);
	_unit_Manager->setPosition(Vec2::ZERO);
	this->addChild(_unit_Manager);
	//Crate the Base
	_unit_Manager->initBase();


	_money->schedule(schedule_selector(Money::updateMoney), 1);
	_unit_Manager->schedule(schedule_selector(UnitManager::updateMessage), 5.0f/60);


	//TODO initial the money and power

	//start the update

	//Mouse listener for scroll map
	resetCursor();
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(GameScene::onMouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	//Create rect of selection
	mouse_rect = MouseRect::create();
	
	_tiled_Map->getTiledMap()->addChild(mouse_rect, 50);

	//Start update
	schedule(schedule_selector(GameScene::update));

	//Touch listener for mouse rect selection
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//Keyboard listener for go back to base
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
	*/
	return true;
}

void GameScene::update(float dt) {
	mapScroll();
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
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_screen_width = visibleSize.width;
	_screen_height = visibleSize.height;


	_tiled_Map = TiledMap::create();
	_tiled_Map->setGridVector();
	this->addChild(_tiled_Map);


	// Set money and power
	_money_Image = Sprite::create("ui/Coin.png");
	_money_Image->setPosition(Vec2(_screen_width*0.83, _screen_height*0.04));
	this->addChild(_money_Image);
	_money = Money::create();
	_money->setPosition(Vec2(_screen_width *0.90, _screen_height*0.04));
	this->addChild(_money);
	_power_Image = Sprite::create("ui/electric.png");
	_power_Image->setPosition(Vec2(_screen_width*0.83, _screen_height*0.12));
	_power_Image->setScale(0.08);
	_power = Power::create();
	_power->setPosition(Vec2(_screen_width*0.90, _screen_height*0.12));
	this->addChild(_power);
	this->addChild(_power_Image);


	_unit_Manager = UnitManager::create(_tiled_Map,_socket_server,_socket_client);
	_unit_Manager->setPosition(Vec2::ZERO);
	this->addChild(_unit_Manager);
	//Crate the Base
	_unit_Manager->initBase();


	_money->schedule(schedule_selector(Money::updateMoney), 1);
	_unit_Manager->schedule(schedule_selector(UnitManager::updateMessage), 5.0f / 60);


	//TODO initial the money and power

	//start the update

	//Mouse listener for scroll map
	resetCursor();
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(GameScene::onMouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	//Create rect of selection
	mouse_rect = MouseRect::create();

	_tiled_Map->getTiledMap()->addChild(mouse_rect, 50);

	//Start update
	schedule(schedule_selector(GameScene::update));

	//Touch listener for mouse rect selection
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//Keyboard listener for go back to base
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
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
		auto _tiled_map = _tiled_Map->getTiledMap();
		auto map_size = _tiled_map->getContentSize();
		Vec2 base_point = _unit_Manager->getBasePosition("ObjectLayer",RED);
		//log("%f_%f", base_point.x, base_point.y);
		float map_posX = _screen_width / 2 - base_point.x;
		float map_posY = _screen_height / 2 - base_point.y;
		//log("%f_%f", map_posX, map_posY);
		if (map_posX > 0)
			map_posX = 0;
		if (map_posY > 0)
			map_posY = 0;
		if (map_posX < _screen_width - map_size.width)
			map_posX = _screen_width - map_size.width;
		if (map_posX < _screen_height - map_size.height)
			map_posX = _screen_height - map_size.height;
		_tiled_map->setPosition(map_posX, map_posY);
		break;
	}
}

//Back to center
void GameScene::resetCursor() {
	_cursorX = _screen_width * 0.5;
	_cursorY = _screen_height * 0.5;
}

void GameScene::onMouseMove(Event* event) {
	EventMouse* mouse_event = static_cast<EventMouse*> (event);
	_cursorX = mouse_event->getCursorX();
	_cursorY = mouse_event->getCursorY();
	//Boundary violation Back to center
	if (_cursorX < 0 || _cursorX > _screen_width || _cursorY < 0 || _cursorY > _screen_height) {
		resetCursor();
	}
}

void GameScene::mapScroll() {
	auto _tiled_map = _tiled_Map->getTiledMap();
	float posX = _tiled_map->getPositionX();
	float posY = _tiled_map->getPositionY();
	float speed_low = 6.0;
	float speed_high = 12.0;
//	log("%f_%f", _cursorX - (_tiled_map->getPosition()).x, _cursorY - (_tiled_map->getPosition()).y);
	if (_cursorX <= _screen_width * 0.05) {
		if (_cursorY <= _screen_height * 0.05) {
			posX += speed_high;
			posY += speed_high;
		}
		if (_cursorY >= _screen_height * 0.95) {
			posX += speed_high;
			posY -= speed_high;
		}
		if ((_cursorY > _screen_height * 0.05) && (_cursorY < _screen_height * 0.95)) {
			posX += speed_high;
		}
	}
	if ((_cursorX > _screen_width * 0.05) && (_cursorX <= _screen_width * 0.1)) {
		if ((_cursorY > _screen_height * 0.05) && (_cursorY <= _screen_height * 0.1)) {
			posX += speed_low;
			posY += speed_low;
		}
		if ((_cursorY < _screen_height * 0.95) && (_cursorY >= _screen_height * 0.9)) {
			posX += speed_low;
			posY -= speed_low;
		}
		if ((_cursorY < _screen_height * 0.9) && (_cursorY > _screen_height * 0.1)) {
			posX += speed_low;
		}
	}
	if (_cursorX >= _screen_width * 0.95) {
		if (_cursorY <= _screen_height * 0.05) {
			posX -= speed_high;
			posY += speed_high;
		}
		if (_cursorY >= _screen_height * 0.95) {
			posX -= speed_high;
			posY -= speed_high;
		}
		if ((_cursorY > _screen_height * 0.05) && (_cursorY < _screen_height * 0.95)) {
			posX -= speed_high;
		}
	}
	if ((_cursorX >= _screen_width * 0.9) && (_cursorX < _screen_width * 0.95)) {
		if ((_cursorY > _screen_height * 0.05) && (_cursorY <= _screen_height * 0.1)) {
			posX -= speed_low;
			posY += speed_low;
		}
		if ((_cursorY < _screen_height * 0.95) && (_cursorY >= _screen_height * 0.9)) {
			posX -= speed_low;
			posY -= speed_low;
		}
		if ((_cursorY < _screen_height * 0.9) && (_cursorY > _screen_height * 0.1)) {
			posX -= speed_low;
		}
	}
	if ((_cursorX > _screen_width * 0.1) && (_cursorX < _screen_width * 0.9)) {
		if ((_cursorY >= _screen_height * 0.9) && (_cursorY < _screen_height * 0.95)) {
			posY -= speed_low;
		}
		if ((_cursorY > _screen_height * 0.05) && (_cursorY <= _screen_height * 0.1)) {
			posY += speed_low;
		}
	}
	if ((_cursorX > _screen_width * 0.05) && (_cursorX < _screen_width * 0.95)) {
		if (_cursorY >= _screen_height * 0.95) {
			posY -= speed_high;
		}
		if (_cursorY <= _screen_height * 0.05) {
			posY += speed_high;
		}
	}
	auto tileWidth = _tiled_map->getContentSize().width;
	auto tileHeight = _tiled_map->getContentSize().height;
	if (posX <= 0 && posX >= (_screen_width - tileWidth)) {
		if (posY <= 0 && posY >= (_screen_height - tileHeight)) {
			_tiled_map->setPosition(posX, posY);
		}
	}
}

bool GameScene::onTouchBegan(Touch* touch, Event* event) {
	Vec2 touch_point = touch->getLocation();
	mouse_rect->start = touch_point - _tiled_Map->getTiledMap()->getPosition();
	mouse_rect->end = touch_point - _tiled_Map->getTiledMap()->getPosition();
	mouse_rect->schedule(schedule_selector(MouseRect::update));
	return true;
}

void GameScene::onTouchMoved(Touch* touch, Event* event) {
	Vec2 touch_point = touch->getLocation();
	mouse_rect->end = touch_point - _tiled_Map->getTiledMap()->getPosition();
	mouse_rect->clear();
	mouse_rect->setVisible(true);
}

void GameScene::onTouchEnded(Touch* touch, Event* event) {
	Vec2 touch_point = touch->getLocation() - _tiled_Map->getTiledMap()->getPosition();
	mouse_rect->end = touch_point;

	float rect_width = fabs(mouse_rect->start.x - mouse_rect->end.x);
	float rect_height = fabs(mouse_rect->start.y - mouse_rect->end.y);

	//select by point
	if (rect_width * rect_height < 100.0) {
		_unit_Manager->selectUnitsByPoint(touch_point);
	}
	//select by rect
	else {
		_unit_Manager->selectUnitsByRect(mouse_rect);
	}
	mouse_rect->reset();
}

	
