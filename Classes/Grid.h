#ifndef _GRID
#define _GRID
#define FIX_SQUARE 0
#define FIX_HEIGHT 1
#define RED 99
#define BLUE 66
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

class GridRect:public cocos2d::Ref {
private:
	int _x;
	int _y;
	int _width;
	int _height;
	bool initwithRect(int x, int y, int w, int h){
		_x = x;
		_y = y;
		_width = w;
		_height = h;
		return true;
	}
public:
	static GridRect* create(int x, int y, int w, int h) {
		auto g = new GridRect();
		if (g && g->initwithRect(x, y, w, h)) {
			g->autorelease();
			return g;
		}
		CC_SAFE_DELETE(g);
		return nullptr;
	}
	int getX() { return _x; }
	int getY() { return _y; }
	int getWidth() { return _width; }
	int getHeight() { return _height; }
};
#endif // !_GRID