#ifndef _GameScene
#define _GameScene
#include "cocos2d.h"
#include "MyResources.h"
#include "UnitManager.h"
USING_NS_CC;
//Main Class of Game Scene
class GameScene :public cocos2d::Scene {
private:

	//Sprite Class
	//TODO The definition of Sprite class

	//The Size of Screen
	int screen_Width;
	int screen_Height;

	//The vector of collidable item;
	std::vector<Grid *> _collidable_Vector;

	//the point to class needed instance
	Sprite* _money_Image;
	Sprite* _power_Image;
	Money* _money;
	Power* _power;
	UnitManager * _unit_Manager;
	Base* _base;
	TiledMap *_tiled_Map;

public:
	
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void update(float dt) override;
	

	CREATE_FUNC(GameScene);
	//turn the coordinate of OpenGL to TileMap
	//The return value is the coordinate of tho position in TileMap
	Vec2 tileCoordForPosition(Vec2 position);
	
	//The function of Scene Transition
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();

	//Create the KeyBoard CallBack();
	void onKeyPressed(EventKeyboard::KeyCode, Event * event) ;
	//If you win,then ?
	//TODO
//	void win();
//	void lose();

	//The function of Touch
	
	

};




#endif _GameScene