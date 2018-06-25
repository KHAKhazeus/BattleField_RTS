#ifndef _SMALL_MAP
#define _SMALL_MAP
#include "cocos2d.h"
USING_NS_CC;
class SmallMap : public cocos2d::Sprite {
private:
	Sprite * _small_Map;
	//This one is to draw the rect
	DrawNode * _draw_Node;
	//This one is to draw the unit
	DrawNode * _draw_UNode;
	Vec2 _point[4];
	void getDrawVector(Vec2 p[4], Vec2 pos);
	
public:
	static SmallMap* create();
	bool init();
	Vec2 sMapToWorld(Vec2 smallPos);
	Vec2 worldTosMap(Vec2 worldPos);
	void clear();
	void drawRect(Vec2 pos);
	void drawUnit(float dt);
	Vec2 getSMapSize() { return _small_Map->getContentSize(); }
	bool onTouchBegan(Touch *touch, Event *event);
	void onTouchEnded(Touch *touch, Event *event);

};
#endif // !_SMALL_MAP
