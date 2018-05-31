//
//  SettingLayer.h
//  BattleField_RTS
//
//  Created by KHAKhazeus on 2018/5/26.
//

#ifndef __SETTING_LAYER_H__
#define __SETTING_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
class SettingLayer : public cocos2d::Layer
{
private:
    float _node_height;
    float _node_width;
    cocos2d::EventListenerTouchOneByOne* _swallow_listner;
public:
    
    virtual bool init() override;
    
    virtual void onExit() override;
    
    virtual void onEnter() override;
    
    void close(cocos2d::Node* pSender);
    
    void groupFadeIn(cocos2d::Vector<Node*> group);
    
    void removeLayer(float dt);
    
    static void fadeOutIteration(cocos2d::Vector<Node*> target);
    
    // implement the "static create()" method manually
    CREATE_FUNC(SettingLayer);
};

#endif // __SETTING_NODE_H__
