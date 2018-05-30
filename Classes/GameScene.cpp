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
	
	//initial the Map
	_tiled_map = TMXTiledMap::create("map/LostTemple.tmx");
	_tiled_map->setAnchorPoint(Vec2::ZERO);
	_tiled_map->setPosition(Vec2::ZERO);
	this->addChild(_tiled_map,0);

	//push the collidable item to the _colliable Vector
	_collidable = _tiled_map->getLayer("Collidable");
	for (int j = 0; j < _tiled_map->getMapSize().height; j++) {

		for (int i = 0; i < _tiled_map->getMapSize().width; i++) {
			Vec2 position = Vec2(i, j);
			int tileGid = _collidable->getTileGIDAt(position);
			if (tileGid) {
				Value properites = _tiled_map->getPropertiesForGID(tileGid);
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
	_money_image = Sprite::create("ui/Coin.png");
	_money_image->setPosition(Vec2(_screen_width*0.83, _screen_height*0.04));
	this->addChild(_money_image);
    _money = Money::create();
	_money->setPosition(Vec2(_screen_width *0.92, _screen_height*0.04));
	this->addChild(_money);
	_power_image = Sprite::create("ui/electric.png");
	_power_image->setPosition(Vec2(_screen_width*0.83, _screen_height*0.10));
	_power_image->setScale(0.08);
	_power = Power::create();
	_power->setPosition(Vec2(_screen_width*0.92, _screen_height*0.10));
	this->addChild(_power);
	this->addChild(_power_image);


	_base = Base::create();
	_base->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	_tiled_map->addChild(_base);
	_money->schedule(schedule_selector(Money::updateMoney), 1);
	_power->schedule(schedule_selector(Power::updatePower), 1);
	
	//TODO initial the money and power

	//start the update

	//Mouse listener for scroll map
	resetCursor();
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(GameScene::onMouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	//Create rect of selection
	mouse_rect = MouseRect::create();
	_tiled_map->addChild(mouse_rect, 50);

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
	int x = static_cast<int>
		(position.x / (_tiled_map->getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
	float pointHeight = _tiled_map->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	int y = static_cast<int>
		((_tiled_map->getMapSize().height * pointHeight - position.y) / pointHeight);
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
	mouse_rect->start = touch_point - _tiled_map->getPosition();
	mouse_rect->end = touch_point - _tiled_map->getPosition();
	mouse_rect->schedule(schedule_selector(MouseRect::update));
	return true;
}

void GameScene::onTouchMoved(Touch* touch, Event* event) {
	Vec2 touch_point = touch->getLocation();
	mouse_rect->end = touch_point - _tiled_map->getPosition();
	mouse_rect->clear();
	mouse_rect->setVisible(true);
}

void GameScene::onTouchEnded(Touch* touch, Event* event) {
	Vec2 touch_point = touch->getLocation() - _tiled_map->getPosition();
	mouse_rect->end = touch_point;

	/*
	//Selected rect
	float rect_x = MIN(mouse_rect->start.x, mouse_rect->end.x);
	float rect_y = MIN(mouse_rect->start.y, mouse_rect->end.y);
	float rect_width = fabs(mouse_rect->start.x - mouse_rect->end.x);
	float rect_height = fabs(mouse_rect->start.y - mouse_rect->end.y);
	Rect select_rect(rect_x, rect_y, rect_width, rect_height);

	
	Vec2 player_point = player->getPosition();

	//Concel select
	if (rect_width * rect_height > 100.0) {
		if (!select_rect.containsPoint(player_point)) {
			_select_flag = 0;
			auto unselected = TintTo::create(0.1f, 255, 255, 255);
			player->runAction(unselected);
		}
	}
	if (_select_flag == 1) {
		//Run action
	}

	//Select
	if (rect_width * rect_height > 100.0) {
		if (select_rect.containsPoint(player_point)) {
			_select_flag = 1;
			auto selected = TintTo::create(0.1f, 255, 0, 0);
			player->runAction(selected);
		}
	}
	*/

	mouse_rect->reset();
}