#ifndef _LOADING_SCENE
#define _LOADING_SCENE
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "BattleField_RTS.h"
#include "SocketClient.h"
#include "SocketServer.h"
#include "GameScene.h"
USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;
class LoadingScene :public Scene {
private:
	LoadingBar* _loadingBar;
	Sprite* _background;
	int _count;
	static SocketServer* _socket_server;
	static std::shared_ptr<SocketClient> _socket_client;
public:
	static Scene* createScene();
	virtual bool init();
	virtual void onEnter();
	CREATE_FUNC(LoadingScene);
	void loadingCallback();
	Animation * getAnimateByName(std::string animName, float delay, int animNum);
//	virtual void onEnter()override;
};
#endif // !_LOADING_SCENE
