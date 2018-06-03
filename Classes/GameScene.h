#ifndef _GameScene
#define _GameScene
#include "cocos2d.h"
#include "MyResources.h"
#include "UnitManager.h"
#include "BuildingBase.h"
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

	//The Size of Screen
	int _screen_width;
	int _screen_height;

	//Cursor point
	float _cursorX, _cursorY;

	bool _select_flag = 0;

	//Pointer to class needed instance
	Sprite* _money_Image;
	Sprite* _power_Image;
	Money* _money;
	Power* _power;
	//Can create the base and check something(TODO)
	UnitManager * _unit_Manager;
	//The Map class contains _collidabel_Vector and gridAndId_Map (Static)
	TiledMap *_tiled_Map;

	//vector for unit
	Vector<MoneyMine*> _moneyMine;
	Vector<PowerPlant*> _powerPlant;
	Vector<WarFactory*>_warFactory;
	Vector<SoldierBase*>_soldierBase;

public:
	
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void update(float dt) override;
	
	Vector<MoneyMine*>& getVectorMine() { return _moneyMine; }
	Vector<PowerPlant*>& getVectorPower() { return _powerPlant; }
	Vector<SoldierBase*>& getVectorSoldier() { return _soldierBase; }
	Vector<WarFactory*>& getVectorFactory() { return _warFactory; }

	Money* getMoney() { return _money; }

	Power* getPower() { return _power; }

	CREATE_FUNC(GameScene);
	
	
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

	//Mouse function
	float getCursorX() { return _cursorX; }
	float getCursorY() { return _cursorY; }

	//The function of Touch
	
	

};


#endif _GameScene