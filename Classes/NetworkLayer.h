//
//  SettingLayer.h
//  BattleField_RTS
//
//  Created by KHAKhazeus on 2018/6/10.
//

#ifndef __NETWORK_LAYER_H__
#define __NETWORK_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SocketServer.h"
#include "SocketClient.h"
class NetworkLayer : public cocos2d::Layer
{
private:
    float _node_height;
    float _node_width;
    cocos2d::EventListenerTouchOneByOne* _swallow_listner;
    SocketServer *socket_server;
    SocketClient *socket_client;
public:
    
    virtual bool init() override;
    
    virtual void onExit() override;
    
    virtual void onEnter() override;
    
    void close(cocos2d::Node* pSender);
    
    void fadeInIteration(cocos2d::Vector<Node*> targets);
    
    void removeLayer(float dt);
    
    static void fadeOutIteration(cocos2d::Vector<Node*> targets);
    
    void initializeServerSide();
    
    void initializeClientSide();
    
    void resetClientAndServer();
    
    // implement the "static create()" method manually
    CREATE_FUNC(NetworkLayer);
};

#endif // __NETWORK_LAYER_H__

