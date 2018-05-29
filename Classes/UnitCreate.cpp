#include "UnitCreate.h"
Base* Base::create() {
	auto base = new Base();
	if (base && base->init()) {
		base->autorelease();
		return base;
	}
	CC_SAFE_DELETE(base);
	return NULL;
}

bool Base::init() {
	if(!Sprite::init()) {
		return false;
	}
	_base = Sprite::create("unit/base_28.png");
	_base->setPosition(0,0);

	isselected = false;
	iscreated = false;
	//temp_sprite->setPosition()
	addChild(_base);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Base::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(Base::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _base);

	return true;
}
//Click the Base building
//TODO judge if can create it (money,power,grid)
//TODO if can't send out warning
//TODO the animation of creating

bool Base::onTouchBegan(Touch *touch, Event *event) {
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	auto rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode)) {	//if click is valid
		if (isselected || iscreated) { 
			return false;
		}
		isselected = true;
		//TODO change this to vector
		auto width = -this->getPosition().x * 0.2;
		auto height = this->getPosition().y * 0.3;
		std::vector<Sprite *> temp_sprite;
		for (int i = 0; i < 4; i++) {
			auto temp = Sprite::create(StringUtils::format("unit/building_%d.png",i+1));
			temp->setScale(0.3);
			temp->setPosition(width + this->getPosition().x * i / 8, height);
			temp_sprite.push_back(temp);
			this->addChild(temp);
		}
	//	auto temp_sprite = Sprite::create("unit/MinetoMoney_24.png"); //create a small icon
	//	temp_sprite->setScale(0.3);
	//	temp_sprite->setPosition(-this->getPosition().x *0.2, this->getPosition().y*0.3);
	//	this->addChild(temp_sprite);
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		//Click the small icon
		for (auto i = 0; i < temp_sprite.size(); i++) {
			auto temp = temp_sprite.at(i);
			listener->onTouchBegan = [temp,temp_sprite,this,i](Touch *touch, Event *event) {
				/*	auto touchLocation = touch->getLocation();
				auto node = (Node*)temp_sprite;
				auto rect = Rect(0, 0,
				temp_sprite->getContentSize().width, temp_sprite->getContentSize().height);*/
				auto target = static_cast<Sprite*>(event->getCurrentTarget());
				auto locationInNode = target->convertToNodeSpace(touch->getLocation());
				Size s = target->getContentSize();
				auto rect = Rect(0, 0, s.width, s.height);
				if (rect.containsPoint(locationInNode)) {
					if (iscreated) {
						return false;
					}
					iscreated = true;
					auto temp_building = Sprite::create(StringUtils::format("unit/building_%d.png", i + 1));
					temp_building->setOpacity(50);
					temp_building->setPosition(temp->getPosition());
					this->addChild(temp_building);
					auto listener1 = EventListenerTouchOneByOne::create();
					listener1->setSwallowTouches(true);
					listener1->onTouchBegan = [temp_building](Touch *touch, Event *event) {
						auto touchLocation = touch->getLocation();
						auto node = (Node*)temp_building;
						auto rect = Rect(0, 0,
							temp_building->getContentSize().width, temp_building->getContentSize().height);
						if (rect.containsPoint(node->convertToNodeSpace(touchLocation))) {
							return true;
						}
					};
					listener1->onTouchMoved = [temp_building](Touch *touch, Event *event) {
						auto target = static_cast<Sprite*>(event->getCurrentTarget());
						target->setPosition(target->getPosition() + touch->getDelta());
					};
					listener1->onTouchEnded = [this, temp_building](Touch *touch, Event *event) {
						this->removeChild(temp_building, true);
						iscreated = false;
					};
					Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener1, temp_building);
					return true;
				}
				return false;
			};
			//if click is ended,remove the sprite created
			listener->onTouchEnded = [this, temp_sprite](Touch *touch, Event *event) {
				for (int i = 0; i < temp_sprite.size(); i++) {
					this->removeChild(temp_sprite.at(i),true);
			//Debug		log("%d", temp_sprite.size());
				}
				isselected = false;
			};
			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(),temp);
		}
		
		
		return true;
	}
	return false;
}

void Base::onTouchEnded(Touch *touch, Event *event) {

}

