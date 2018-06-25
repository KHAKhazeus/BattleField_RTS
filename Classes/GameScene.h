#ifndef _GameScene
#define _GameScene
#include "cocos2d.h"
#include "MyResources.h"
#include "UnitManager.h"
#include "BuildingBase.h"
#include "FighterUnitBase.h"
#include "MouseRect.h"
#include "ui/CocosGUI.h"
#include "SmallMap.h"
USING_NS_CC;
using namespace ui;

//Main Class of Game Scene
class GameScene :public cocos2d::Scene {
private:

	//The Size of Screen
	int _screen_width;
	int _screen_height;

	//Cursor point
	float _cursorX, _cursorY;

	//Position compensation
	Vec2 _returnPos;


	//Pointer to class needed instance
	Button* _money_Image;
	Button* _power_Image;
	
	Money* _money;
	Power* _power;
	//Can create the base and check something(TODO)
	UnitManager * _unit_Manager;
	//The Map class contains _collidabel_Vector and gridAndId_Map (Static)
	TiledMap *_tiled_Map;

	TextField* _inputBox;

	Text* _myShow;
	Text* _enShow;
	LayerGradient* _textBackground;

	SmallMap* _small_Map;

	//To judge if is able to mapscroll
	bool _judge_Move;

	//vector for unit
	Vector<MoneyMine*> _moneyMine;
	static SocketServer* _socket_server;
	static SocketClient* _socket_client;

public:
	
	static cocos2d::Scene* createScene(SocketServer* spserver, SocketClient* spclient);
	virtual bool init();
	virtual void update(float dt) override;
	
	Vector<MoneyMine*>& getVectorMine() { return _moneyMine; }

	Money* getMoney() { return _money; }

	Power* getPower() { return _power; }

	TiledMap* getMap() { return _tiled_Map; }

	UnitManager* getUnitManager() { return _unit_Manager; }
	
	void winOrLose(bool win);

	CREATE_FUNC(GameScene);
	//turn the coordinate of OpenGL to TileMap
	//The return value is the coordinate of tho position in TileMap
	Vec2 tileCoordForPosition(Vec2 position);
	
	//The function of Scene Transition
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();

	Vec2 getScreen() { return Vec2(_screen_width, _screen_height); }

	//Create the KeyBoard CallBack();
	void onKeyPressed(EventKeyboard::KeyCode, Event * event) ;
	//If you win,then ?
	//TODO
//	void win();
//	void lose();

	void resetCursor();

	//Scroll map
	void mapScroll();

	
	//check if there is a valid address
	bool isServer();
	bool isClient();

	Text* getMyShow() { return _myShow; }
	Text* getEnShow() { return _enShow; }
 

	//Pass data from listener
	MouseRect* mouse_rect;

	//Touch listener
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	bool getJudgeMove() { return _judge_Move; }
	void setJudgeMove(bool tag) { _judge_Move = tag; }


	//Mouse listener
	void onMouseMove(Event* event);

	void menuCloseCallback(cocos2d::Ref* pSender);
};


#endif _GameScene