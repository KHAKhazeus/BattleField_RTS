#ifndef _GameScene
#define _GameScene
#include "cocos2d.h"
#include "Grid.h"
USING_NS_CC;
//Main Class of Game Scene
class GameScene :public cocos2d::Scene {
private:
	//TMX Class
	TMXTiledMap * _tiledMap;//Map
	TMXLayer* _collidable;

	//Sprite Class
	//TODO The definition of Sprite class

	//The Size of Screen
	int _screenWidth;
	int _screenHeight;

	//The vector of collidable item;
	std::vector<Grid *> _collidableVector;

public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void update();
	CREATE_FUNC(GameScene);
	//turn the coordinate of OpenGL to TileMap
	//The return value is the coordinate of tho position in TileMap
	Vec2 tileCoordForPosition(Vec2 position);

};

#endif