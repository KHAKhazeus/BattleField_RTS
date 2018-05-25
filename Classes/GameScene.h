#ifndef _GameScene
#define _GameScene
#include "cocos2d.h"
#include "Grid.h"
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
	int _screen_Width;
	int _screen_Height;

	//The vector of collidable item;
	std::vector<Grid *> _collidable_Vector;

	Label* _money_Count;
	Label* _power_Count;
	Sprite* _money_Image;
	Sprite* _power_Image;

public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void update();
	CREATE_FUNC(GameScene);
	//turn the coordinate of OpenGL to TileMap
	//The return value is the coordinate of tho position in TileMap
	Vec2 tileCoordForPosition(Vec2 position);
	
	//The function of Scene Transition
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();

};

#endif