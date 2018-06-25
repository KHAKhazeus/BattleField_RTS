#include "SmallMap.h"
#include "GameScene.h"
#include "TiledMap.h"

SmallMap* SmallMap::create() {
	auto smallMap = new SmallMap();
	if (smallMap && smallMap->init()) {
		smallMap->autorelease();
		return smallMap;
	}
	CC_SAFE_DELETE(smallMap);
	return NULL;
}

bool SmallMap::init() {
	if (!Sprite::init()) {
		return false;
	}
	if (TiledMap::getMapFlag() == LOSTTEMP) {
		_small_Map = Sprite::create("map/LostSMap.png");
	}
	else {
		_small_Map = Sprite::create("map/SnowSmap.png");
	}
	
	_small_Map->setAnchorPoint(Vec2::ZERO);
	_draw_Node = DrawNode::create(5);
	_draw_UNode = DrawNode::create();
	this->addChild(_small_Map);
	this->addChild(_draw_Node, 200);
	this->addChild(_draw_UNode, 200);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(SmallMap::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(SmallMap::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _small_Map);

	return true;
}

bool SmallMap::onTouchBegan(Touch *touch, Event *event) {
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	auto locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	auto rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode)) {
		return true;
	}
	return false;
}

void SmallMap::onTouchEnded(Touch *touch, Event *event) {
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	auto touchLocation = touch->getLocation();
	auto locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	auto rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode)) {
		clear();
		drawRect(locationInNode);
		auto actualPos = sMapToWorld(locationInNode);
		auto tempScene = static_cast<GameScene*>(this->getParent());
		auto unitManager = tempScene->getUnitManager();
		auto tiledMap = unitManager->getTiledMap();
		auto map = tiledMap->getTiledMap();
		auto mapSize = map->getContentSize();
		auto screenVec = tempScene->getScreen();
		float map_posX = screenVec.x / 2 - actualPos.x;
		float map_posY = screenVec.y / 2 - actualPos.y;
		//log("%f_%f", map_posX, map_posY);
		if (map_posX > 0)
			map_posX = 0;
		if (map_posY > 0)
			map_posY = 0;
		if (map_posX < screenVec.x - mapSize.width)
			map_posX = screenVec.x - mapSize.width;
		if (map_posY < screenVec.y - mapSize.height)
			map_posY = screenVec.y - mapSize.height;
		map->setPosition(map_posX,map_posY);
	}
}

void SmallMap::getDrawVector(Vec2 p[4], Vec2 pos) {
	if (pos.x - 50 < 0) {
		pos.x = 50;
	}
	if (pos.y - 30 < 0) {
		pos.y = 30;
	}
	if (pos.x + 50 >= _small_Map->getContentSize().width) {
		pos.x = _small_Map->getContentSize().width - 50;
	}
	if (pos.y + 30 >= _small_Map->getContentSize().height) {
		pos.y = _small_Map->getContentSize().height - 30;
	}
	p[0] = Vec2(pos.x - 50, pos.y - 30);
	p[1] = Vec2(pos.x - 50, pos.y + 30);
	p[2] = Vec2(pos.x + 50, pos.y + 30);
	p[3] = Vec2(pos.x + 50, pos.y - 30);
}

Vec2 SmallMap::sMapToWorld(Vec2 smallPos) {
	Vec2 pos;
	pos.x = smallPos.x / _small_Map->getContentSize().width * 128 * 32;
	pos.y = smallPos.y / _small_Map->getContentSize().height * 128 * 32;
	return pos;
}

Vec2 SmallMap::worldTosMap(Vec2 worldPos) {
	Vec2 pos;
	pos.x = worldPos.x / (128.0 * 32.0) * _small_Map->getContentSize().width;
	pos.y = worldPos.y / (128.0 * 32.0) * _small_Map->getContentSize().height;
	return pos;
}

void SmallMap::drawRect(Vec2 pos) {
	getDrawVector(_point, pos);
	_draw_Node->drawPolygon(_point, 4, Color4F(0, 0, 0, 0), 3, Color4F(0, 1, 0, 1));
}

void SmallMap::clear() {
	_draw_Node->clear();
}

void SmallMap::drawUnit(float dt) {
	_draw_UNode->clear();
	auto tempMap = TiledMap::getIdAndUnitMap();
	auto tempScene = static_cast<GameScene*>(this->getParent());
	auto unitManager = tempScene->getUnitManager();
	for (auto i = tempMap.cbegin(); i != tempMap.cend(); i++) {
		auto pos = i->second->getPosition();
		auto smallPos = worldTosMap(pos);
		if (i->second->getCampID() == unitManager->_myCamp) {
			_draw_UNode->drawPoint(smallPos, 5, Color4F::RED);
		}
		else {
			_draw_UNode->drawPoint(smallPos, 5, Color4F::BLUE);
		}
	}
	
}
