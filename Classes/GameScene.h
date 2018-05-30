#ifndef _GameScene
#define _GameScene
#include "cocos2d.h"
#include "Grid.h"
#include "MyResources.h"
#include "UnitCreate.h"
USING_NS_CC;

//Class of mouse rect selection
class MouseRect : public cocos2d::DrawNode
{
public:
	CREATE_FUNC(MouseRect);

	//Start point and end point
	Vec2 start, end;

	virtual void update(float delta);

	void reset();
};

//Main Class of Game Scene
class GameScene :public cocos2d::Scene {
private:
	//TMX Class
	TMXTiledMap * _tiled_map;//Map
	TMXLayer* _collidable;

	//Sprite Class
	//TODO The definition of Sprite class

	//The Size of Screen
	int _screen_width;
	int _screen_height;

	//Cursor point
	float _cursorX, _cursorY;

	bool _select_flag = 0;

	//The vector of collidable item;
	std::vector<Grid *> _collidable_Vector;

	//The ui of item
	Sprite* _money_image;
	Sprite* _power_image;
	Money* _money;
	Power* _power;
	
	Base* _base;

public:
	
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void update(float dt) override;
	
	void updateMoney(float );
	void updatePower(float );

	CREATE_FUNC(GameScene);
	//turn the coordinate of OpenGL to TileMap
	//The return value is the coordinate of tho position in TileMap
	Vec2 tileCoordForPosition(Vec2 position);
	
	//The function of Scene Transition
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();

	//Create the KeyBorad CallBack();
	void onKeyPressed(EventKeyboard::KeyCode, Event * event) ;
	//If you win,then ?
	//TODO
//	void win();
//	void lose();

	void resetCursor();

	//Scroll map
	void mapScroll();

	//Pass data from listener
	MouseRect* mouse_rect;

	//Touch listener
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	//Mouse listener
	void onMouseMove(Event* event);

	//The function of Touch
	
	

};




#endif _GameScene