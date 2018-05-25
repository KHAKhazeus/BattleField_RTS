#ifndef _Grid
#define _Grid

#include "cocos2d.h"
class Grid :public cocos2d::Ref {
private:
	int _x; //the x coordinate
	int _y; //the y coordinate
	bool _pass;// judge if the unit can pass 
public:
	static Grid* create(int x, int y) {
		auto g = new Grid();
		if (g && g->initWithPoint(x, y)) {
			g->autorelease();
			return g;
		}
		CC_SAFE_DELETE(g);
		return nullptr;
	}
	// initial the Point
	bool initWithPoint(int x, int y) {
		_x = x;
		_y = y;
		_pass = true;
		return true;
	}
	void setX(int x) { _x = x; }
	int getX() { return _x; }
	void setY(int y) { _y = y; }
	int getY() { return _y; }
	void setPass(bool pass) { _pass = pass; }
	bool isPass() { return _pass; }
};

#endif // 