#ifndef _GameScene
#define _GameScene
#include "cocos2d.h"
#include "Grid.h"
#include "MyResources.h"
#include "UnitCreate.h"
USING_NS_CC;
//Main Class of Game Scene
class GameScene :public cocos2d::Scene {
private:
	//TMX Class
	TMXTiledMap * _tiled_Map;//Map
	TMXLayer* _collidable;

	//Sprite Class
	//TODO The definition of Sprite class

	//The Size of Screen
	int screen_Width;
	int screen_Height;

	//The vector of collidable item;
	std::vector<Grid *> _collidable_Vector;

	//The ui of item
	Sprite* _money_Image;
	Sprite* _power_Image;
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

	//The function of Touch
	
	

};




#endif _GameScene