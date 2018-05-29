#ifndef _UNIT_CREATE
#define _UNIT_CREATE
#include "cocos2d.h"
USING_NS_CC;

class Base :public Sprite {
private:
	Sprite * _base;
	std::vector<Sprite *>_build_child;
public:
	bool iscreated;
	bool isselected;
	static Base* create();
	bool init();
	/*static Base* create(std::string filename) {
		auto *sprite = new Base();
		if (sprite && sprite->initWithFile(filename)) {
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}*/
	virtual bool onTouchBegan(Touch *touch, Event *event);
	virtual void onTouchEnded(Touch *touch, Event *event);
};


#endif // !_UNIT_IMAGE

