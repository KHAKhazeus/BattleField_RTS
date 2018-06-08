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

	//Keyboard listener for go back to base
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

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
		auto _tiled_map = _tiled_Map->getTiledMap();
		auto map_size = _tiled_map->getContentSize();
		Vec2 base_point = _unit_Manager->getBasePosition("ObjectLayer");
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
	log("%f_%f", _cursorX - (_tiled_map->getPosition()).x, _cursorY - (_tiled_map->getPosition()).y);
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
	float rect_y = MAX(mouse_rect->start.y, mouse_rect->end.y);
	float rect_width = fabs(mouse_rect->start.x - mouse_rect->end.x);
	float rect_height = fabs(mouse_rect->start.y - mouse_rect->end.y);
	Rect select_rect(rect_x, rect_y, rect_width, rect_height);

	
	if (rect_width * rect_height < 100.0) {
		auto tiledLocation = tileCoordForPosition(touch_point);
		//judge if there is a unit in the Grid  判断瓦片上有没有单位
		if (TiledMap::checkMapGrid(tiledLocation)) {
			auto id = TiledMap::getUnitIdByPosition(tiledLocation);
			auto tempSprite = TiledMap::getUnitById(id);
			//if the tempSprite is the enemy  如果选中的精灵是敌人
			if (tempSprite->getCampID() != _unit_Manager->getBase()->getCampID()) {
				//if > 1 what in the vector isn't a building   如果SelectVector的容量>1,那么它一定不会包含建筑
				if (TiledMap::checkSize() > 1) {
					for (auto temp : *TiledMap::getSelectedVector()) {
						//if the enemy is in the attack range 如果敌人在攻击范围内
						if (temp->judgeAttack(tiledLocation)) {
							//TODO function attack
							
						}
						else {
							//TODO function tracing
						}
					}	
				}
				else {
					//judge the one in vector is building or not 判断在vector里面的唯一一个元素是不是建筑
					if (TiledMap::checkSize() == 1) {
						auto temp = TiledMap::getSelectedVector()->at(0);
						//if not
						if (!temp->isBuilding()) {
							if (temp->judgeAttack(tiledLocation)) {
								//TODO function attack
							}
							else {
								//TODO function tracing
							}
						}
					}
				}
			}
			//if not, then clear up the vector and then push the new one
			//如果选中的精灵不是敌人,那么清空vector,并把此次点击的精灵加入进去
			else
			{
				TiledMap::clearUp();
				TiledMap::newVectorUnit(tempSprite);	
			}
		}
		//if not 如果点到的是空地
		else {
			//check the vector and judge if there is a building in it 
			//检查vector,看是否只存了一个建筑
			if (TiledMap::checkSize()) {
				//if yes, clear up
				if (TiledMap::getSelectedVector()->at(0)->isBuilding()) {
					TiledMap::clearUp();
				}
				//if not, call all the unit in the vector to find a path and move to the Position
				else {
					//TODO function to move to the Position
					for (auto temp : *TiledMap::getSelectedVector()) {
						temp->moveTo(touch_point);
						TiledMap::updateMapGrid(temp->getTiledPosition(), tiledLocation);
						temp->setTiledPosition(tiledLocation);
					}
				}
			}
		}
	}
	else {
		TiledMap::clearUp();	//cancel select
		auto tiledPos = tileCoordForPosition(Vec2(rect_x, rect_y));
		auto tiledRect = tileCoordForPosition(Vec2(rect_width, rect_height));
		tiledRect.y = 127 - tiledRect.y;
		for (auto i = tiledPos.x; i < tiledRect.x + tiledPos.x; i++) {
			for (auto j = tiledPos.y; j < tiledRect.y + tiledPos.y; j++) {
				auto pos = Vec2(i, j);
				//if there is a unit Grid in this pos
				if (TiledMap::checkMapGrid(pos)) {
					auto id = TiledMap::getUnitIdByPosition(pos);
					auto tempSprite = TiledMap::getUnitById(id);
					//if the unit is belonging to us and it isn't a building
					if ((tempSprite->getCampID() == _unit_Manager->getBase()->getCampID())
						&& !tempSprite->isBuilding()) {
						TiledMap::newVectorUnit(tempSprite);
					}
				}
			}
		}
	}

/*	for (unsigned int i = 0; i < this->_dogs.size(); i++) {
		auto tempUnit = _dogs.at(i);
		Vec2 player_point = tempUnit->getPosition();
		Size size = tempUnit->getContentSize();
		//One point selection
		if (rect_width * rect_height < 100.0) {
			Rect rect = Rect(player_point.x - size.width / 4,
				player_point.y - size.height / 4, size.width / 2, size.height / 2);
			if (rect.containsPoint(touch_point)) {
				tempUnit->setSelected(true);
				tempUnit->getHP()->setVisible(true);
			}
			else {
				tempUnit->setSelected(false);
				tempUnit->getHP()->setVisible(false);
			}
		}

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
		Size size = tempUnit->getContentSize();
		//One point selection
		if (rect_width * rect_height < 100.0) {
			Rect rect = Rect(player_point.x - size.width / 4,
				player_point.y - size.height / 4, size.width / 2, size.height / 2);
			if (rect.containsPoint(touch_point)) {
				tempUnit->setSelected(true);
				tempUnit->getHP()->setVisible(true);
			}
			else {
				tempUnit->setSelected(false);
				tempUnit->getHP()->setVisible(false);
			}
		}

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
		Size size = tempUnit->getContentSize();
		//One point selection
		if (rect_width * rect_height < 100.0) {
			Rect rect = Rect(player_point.x - size.width / 4,
				player_point.y - size.height / 4, size.width / 2, size.height / 2);
			if (rect.containsPoint(touch_point)) {
				tempUnit->setSelected(true);
				tempUnit->getHP()->setVisible(true);
			}
			else {
				tempUnit->setSelected(false);
				tempUnit->getHP()->setVisible(false);
			}
		}

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
		Size size = tempUnit->getContentSize();
		//One point selection
		if (rect_width * rect_height < 100.0) {
			Rect rect = Rect(player_point.x - size.width / 4,
				player_point.y - size.height / 4, size.width / 2, size.height / 2);
			if (rect.containsPoint(touch_point)) {
				tempUnit->setSelected(true);
				tempUnit->getHP()->setVisible(true);
			}
			else {
				tempUnit->setSelected(false);
				tempUnit->getHP()->setVisible(false);
			}
		}

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
	}

	for (unsigned int i = 0; i < this->_powerPlant.size(); i++) {
		auto tempUnit = _powerPlant.at(i);
		Vec2 player_point = tempUnit->getPosition();
		Size size = tempUnit->getContentSize();
		//One point selection
		if (rect_width * rect_height < 100.0) {
			Rect rect = Rect(player_point.x - size.width / 4,
				player_point.y - size.height / 4, size.width / 2, size.height / 2);
			if (rect.containsPoint(touch_point)) {
				tempUnit->setSelected(true);
				tempUnit->getHP()->setVisible(true);
			}
			else {
				tempUnit->setSelected(false);
				tempUnit->getHP()->setVisible(false);
			}
		}

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
	}

	for (unsigned int i = 0; i < this->_soldierBase.size(); i++) {
		auto tempUnit = _soldierBase.at(i);
		Vec2 player_point = tempUnit->getPosition();
		Size size = tempUnit->getContentSize();
		//One point selection
		if (rect_width * rect_height < 100.0) {
			Rect rect = Rect(player_point.x - size.width / 4,
				player_point.y - size.height / 4, size.width / 2, size.height / 2);
			if (rect.containsPoint(touch_point)) {
				tempUnit->setSelected(true);
				tempUnit->getHP()->setVisible(true);
			}
			else {
				tempUnit->setSelected(false);
				tempUnit->getHP()->setVisible(false);
			}
		}

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
	}

	for (unsigned int i = 0; i < this->_warFactory.size(); i++) {
		auto tempUnit = _warFactory.at(i);
		Vec2 player_point = tempUnit->getPosition();
		Size size = tempUnit->getContentSize();
		//One point selection
		if (rect_width * rect_height < 100.0) {
			Rect rect = Rect(player_point.x - size.width / 4,
				player_point.y - size.height / 4, size.width / 2, size.height / 2);
			if (rect.containsPoint(touch_point)) {
				tempUnit->setSelected(true);
				tempUnit->getHP()->setVisible(true);
			}
			else {
				tempUnit->setSelected(false);
				tempUnit->getHP()->setVisible(false);
			}
		}

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

	}
	*/
	mouse_rect->reset();
}