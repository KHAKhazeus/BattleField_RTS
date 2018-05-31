//
//  MenuScene.h
//  BattleField_RTS
//
//  Created by KHAKhazeus on 2018/5/25.
//

#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SettingLayer.h"

class MenuScene : public cocos2d::Layer
{
private:
    int _screen_width, _screen_height;
    cocos2d::Sprite* _background;
    cocos2d::Vector<cocos2d::ui::Button *> _buttons;
public:
    static cocos2d::Scene* createScene();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MenuScene);
};

#endif // __MENU_SCENE_H__
