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
	float speed_low = 4.0;
	float speed_high = 8.0;
	//log("%f_%f", _cursorX, _cursorY);
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

void MouseRect::update(float delta) {
	clear();
	drawRect(start, end, Color4F(0, 1, 0, 1));
}

void MouseRect::reset() {
	setVisible(false);
	if (isScheduled(schedule_selector(MouseRect::update)))
		unschedule(schedule_selector(MouseRect::update));
	start = end = Vec2::ZERO;
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


	//Selected rect
	float rect_x = MIN(mouse_rect->start.x, mouse_rect->end.x);
	float rect_y = MIN(mouse_rect->start.y, mouse_rect->end.y);
	float rect_width = fabs(mouse_rect->start.x - mouse_rect->end.x);
	float rect_height = fabs(mouse_rect->start.y - mouse_rect->end.y);
	Rect select_rect(rect_x, rect_y, rect_width, rect_height);

	auto tempUnit = this->_unit_Manager->getBase();
	Vec2 player_point = tempUnit->getPosition();

	//Concel select
	if (rect_width * rect_height > 100.0) {
		if (!select_rect.containsPoint(player_point)) {
			tempUnit->setSelected(false);
			tempUnit->getHP()->setVisible(false);
		}
	}
	if (tempUnit->isSelected()) {
		//Run action
		tempUnit->getHP()->setVisible(true);
	}

	//Select
	if (rect_width * rect_height > 100.0) {
		if (select_rect.containsPoint(player_point)) {
			tempUnit->setSelected(true);
			tempUnit->getHP()->setVisible(true);
		}
	}

	for (unsigned int i = 0; i < this->_dogs.size(); i++) {
		auto tempUnit = _dogs.at(i);
		Vec2 player_point = tempUnit->getPosition();

		//Concel select
		if (rect_width * rect_height > 100.0) {
			if (!select_rect.containsPoint(player_point)) {
				tempUnit->setSelected(false);
				tempUnit->getHP()->setVisible(false);
			}
		}
		if (tempUnit->isSelected()) {
			//Run action
			tempUnit->getHP()->setVisible(true);
		}

		//Select
		if (rect_width * rect_height > 100.0) {
			if (select_rect.containsPoint(player_point)) {
				tempUnit->setSelected(true);
				tempUnit->getHP()->setVisible(true);
			}
		}
	}

	for (unsigned int i = 0; i < this->_soldiers.size(); i++) {
		auto tempUnit = _soldiers.at(i);
		Vec2 player_point = tempUnit->getPosition();

		//Concel select
		if (rect_width * rect_height > 100.0) {
			if (!select_rect.containsPoint(player_point)) {
				tempUnit->setSelected(false);
				tempUnit->getHP()->setVisible(false);
			}
		}
		if (tempUnit->isSelected()) {
			//Run action
			tempUnit->getHP()->setVisible(true);
		}

		//Select
		if (rect_width * rect_height > 100.0) {
			if (select_rect.containsPoint(player_point)) {
				tempUnit->setSelected(true);
				tempUnit->getHP()->setVisible(true);
			}
		}
	}

	for (unsigned int i = 0; i < this->_tanks.size(); i++) {
		auto tempUnit = _tanks.at(i);
		Vec2 player_point = tempUnit->getPosition();

		//Concel select
		if (rect_width * rect_height > 100.0) {
			if (!select_rect.containsPoint(player_point)) {
				tempUnit->setSelected(false);
				tempUnit->getHP()->setVisible(false);
			}
		}
		if (tempUnit->isSelected()) {
			//Run action
			tempUnit->getHP()->setVisible(true);
		}

		//Select
		if (rect_width * rect_height > 100.0) {
			if (select_rect.containsPoint(player_point)) {
				tempUnit->setSelected(true);
				tempUnit->getHP()->setVisible(true);
			}
		}
	}

	for (unsigned int i = 0; i < this->_moneyMine.size(); i++) {
		auto tempUnit = _moneyMine.at(i);
		Vec2 player_point = tempUnit->getPosition();

		//Concel select
		if (rect_width * rect_height > 100.0) {
			if (!select_rect.containsPoint(player_point)) {
				tempUnit->setSelected(false);
				tempUnit->getHP()->setVisible(false);
			}
		}
		if (tempUnit->getSelected()) {
			//Run action
			tempUnit->getHP()->setVisible(true);
		}

		//Select
		if (rect_width * rect_height > 100.0) {
			if (select_rect.containsPoint(player_point)) {
				tempUnit->setSelected(true);
				tempUnit->getHP()->setVisible(true);
			}
		}
	}

	for (unsigned int i = 0; i < this->_powerPlant.size(); i++) {
		auto tempUnit = _powerPlant.at(i);
		Vec2 player_point = tempUnit->getPosition();

		//Concel select
		if (rect_width * rect_height > 100.0) {
			if (!select_rect.containsPoint(player_point)) {
				tempUnit->setSelected(false);
				tempUnit->getHP()->setVisible(false);
			}
		}
		if (tempUnit->getSelected()) {
			//Run action
			tempUnit->getHP()->setVisible(true);
		}

		//Select
		if (rect_width * rect_height > 100.0) {
			if (select_rect.containsPoint(player_point)) {
				tempUnit->setSelected(true);
				tempUnit->getHP()->setVisible(true);
			}
		}
	}

	for (unsigned int i = 0; i < this->_soldierBase.size(); i++) {
		auto tempUnit = _soldierBase.at(i);
		Vec2 player_point = tempUnit->getPosition();

		//Concel select
		if (rect_width * rect_height > 100.0) {
			if (!select_rect.containsPoint(player_point)) {
				tempUnit->setSelected(false);
				tempUnit->getHP()->setVisible(false);
			}
		}
		if (tempUnit->getSelected()) {
			//Run action
			tempUnit->getHP()->setVisible(true);
		}

		//Select
		if (rect_width * rect_height > 100.0) {
			if (select_rect.containsPoint(player_point)) {
				tempUnit->setSelected(true);
				tempUnit->getHP()->setVisible(true);
			}
		}
	}

	for (unsigned int i = 0; i < this->_warFactory.size(); i++) {
		auto tempUnit = _warFactory.at(i);
		Vec2 player_point = tempUnit->getPosition();

		//Concel select
		if (rect_width * rect_height > 100.0) {
			if (!select_rect.containsPoint(player_point)) {
				tempUnit->setSelected(false);
				tempUnit->getHP()->setVisible(false);
			}
		}
		if (tempUnit->getSelected()) {
			//Run action
			tempUnit->getHP()->setVisible(true);
		}

		//Select
		if (rect_width * rect_height > 100.0) {
			if (select_rect.containsPoint(player_point)) {
				tempUnit->setSelected(true);
				tempUnit->getHP()->setVisible(true);
			}
		}
	}

	mouse_rect->reset();
}