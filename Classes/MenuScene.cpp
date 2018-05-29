//
//  MenuScene.cpp
//  BattleField_RTS
//
//  Created by KHAKhazeus on 2018/5/25.
//

#define TEST

#include "MenuScene.h"
#include "SimpleAudioEngine.h"

#ifdef TEST
#include <iostream>
#endif

USING_NS_CC;
using namespace ui;

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MenuScene.cpp\n");
}

Scene* MenuScene::createScene(){
    auto scene = Scene::create();
    auto menu_scene = MenuScene::create();
    scene->addChild(menu_scene);
    return scene;
}

bool MenuScene::init(){
    if(!Layer::init()){
        return false;
    }
    
    Size visible_size = Director::getInstance()->getVisibleSize();
    _screen_width = visible_size.width;
    _screen_height = visible_size.height;
    
    _background = Sprite::create("Menu/MainBackground.png");
    if(!_background){
        problemLoading("Menu/MainBackground.png");
    }
    else{
        _background->setPosition(Vec2(_screen_width/2, _screen_height/2));
    }

//!!!Need to be packed as one button class
    auto start_game_button = Button::create("Menu/OrangeButton.png", "Menu/OrangeButtonHighlight.png");
    auto button_size = start_game_button->getCustomSize();
    float small_adjust = button_size.height/4;
    float height_start_point = (visible_size.height/2 -
                                       button_size.height * 2 - small_adjust);
    if(!start_game_button){
        problemLoading("Menu/OrangeButton.png and Menu/OrangeButtonHightlight.png");
    }
    else{
        start_game_button->setScale(1.0);
        
        _buttons.pushBack(start_game_button);
        start_game_button->setTitleText("Start Game");
        start_game_button->setTitleFontName("Arial-Bold.ttf");
        start_game_button->setTitleColor(Color3B(80,80,80));
        start_game_button->setTitleFontSize(button_size.height/3);
        start_game_button->setPosition(Vec2(visible_size.width/2, height_start_point));
        start_game_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType event_type){
            switch (event_type) {
                case Widget::TouchEventType::BEGAN:{
                    start_game_button->setScale(1.05);
                    break;
                }
                    
                case Widget::TouchEventType::ENDED:{
                    start_game_button->setScale(1.0);
                    //should switch to Network control panel, temporarily switch to GameScene
                    
                    //auto transition_to_gamescene = TransitionSlideInL::create(2.0, GameScene::createScene());
                    //for tests
                    //                auto transition_to_gamescene = TransitionFadeUp::create(1.0, HelloWorld::createScene());
                    //                Director::getInstance()->replaceScene(transition_to_gamescene);
                    
                    //Director::getInstance()->pushScene(HelloWorld::createScene());
                    break;
                }
                    
                case Widget::TouchEventType::CANCELED:{
                    start_game_button->setScale(1.0);
                    break;
                }
                default:
                    break;
            }
        });
    }
    
    auto setting_button = Button::create("Menu/OrangeButton.png", "Menu/OrangeButtonHighlight.png");
    if(!setting_button){
        problemLoading("Menu/OrangeButton.png and Menu/OrangeButtonHightlight.png");
    }
    else{
        setting_button->setScale(1.0);
        _buttons.pushBack(setting_button);
        setting_button->setTitleText("Settings");
        setting_button->setTitleFontName("Arial-Bold.ttf");
        setting_button->setTitleColor(Color3B(80,80,80));
        setting_button->setTitleFontSize(button_size.height/3);
        setting_button->setPosition(Vec2(visible_size.width/2, height_start_point - button_size.height/2 - small_adjust));
        setting_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType event_type){
            switch (event_type) {
                case Widget::TouchEventType::BEGAN:{
                    setting_button->setScale(1.05);
                    break;
                }
                    
                case Widget::TouchEventType::ENDED:{
                    setting_button->setScale(1.0);
                    auto setting_layer = SettingLayer::create();
                    this->addChild(setting_layer);
                    break;
                }
                    
                case Widget::TouchEventType::CANCELED:{
                    setting_button->setScale(1.0);
                    break;
                }
                    
                default:
                    break;
            }
        });
    }
    
    auto quit_button = Button::create("Menu/OrangeButton.png", "Menu/OrangeButtonHighlight.png");
    if(!quit_button){
        problemLoading("Menu/OrangeButton.png and Menu/OrangeButtonHightlight.png");
    }
    else{
        quit_button->setScale(1.0);
        _buttons.pushBack(setting_button);
        quit_button->setTitleText("Quit Game");
        quit_button->setTitleFontName("Arial-Bold.ttf");
        quit_button->setTitleColor(Color3B(80,80,80));
        quit_button->setTitleFontSize(button_size.height/3);
        quit_button->setPosition(Vec2(visible_size.width/2, height_start_point - button_size.height - small_adjust * 2));
        quit_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType event_type){
            switch (event_type) {
                case Widget::TouchEventType::BEGAN:{
                    quit_button->setScale(1.05);
                    break;
                }
                    
                case Widget::TouchEventType::ENDED:{
                    quit_button->setScale(1.0);
                    MenuScene::menuCloseCallback(this);
                    break;
                }
                    
                case Widget::TouchEventType::CANCELED:{
                    quit_button->setScale(1.0);
                    break;
                }
                    
                default:
                    break;
            }
        });
    }
    
    this->addChild(_background, -1);
    this->addChild(start_game_button, 0);
    this->addChild(quit_button, 0);
    this->addChild(setting_button, 0);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("CCCP.mp3", true);
    
    return true;
}

void MenuScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
