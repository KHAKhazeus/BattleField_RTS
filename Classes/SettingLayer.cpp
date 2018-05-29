//
//  SettingLayer.cpp
//  BattleField_RTS
//
//  Created by Khazeus on 2018/5/26.
//

#include "SettingLayer.h"
#include "SimpleAudioEngine.h"
#include "MenuScene.h"

#ifdef TEST
#include <iostream>
#endif

USING_NS_CC;
using namespace ui;

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in SettingLayer.cpp\n");
}

bool SettingLayer::init(){
    if(!Layer::init()){
        return false;
    }
    
    Size visible_size = Director::getInstance()->getVisibleSize();
    auto visible_width = visible_size.width;
    auto visible_height = visible_size.height;
    float left_margin = visible_size.width/10;
    float up_margin = visible_height/8;
    
    LayerColor *layer = LayerColor::create(Color4B(0,0,0,0));
    if(!layer){
        problemLoading("LayerColor");
    }
    
    this->addChild(layer);
    
    float small_adjust = 15.0;
    
    //!!!need to be packed
    LayerGradient *text_background_volume = LayerGradient::create(Color4B(75,0,130,200),Color4B(147,112,219,200));
    if(!text_background_volume){
        problemLoading("LayerGradient");
    }
    else{
        text_background_volume->changeWidthAndHeight(left_margin * 1.4, up_margin/1.2);
        text_background_volume->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        text_background_volume->setPosition(Vec2(left_margin,visible_height - up_margin - text_background_volume->getContentSize().height));
    }
    
    auto volume_text = Text::create("Music Volume", "arial.ttf", text_background_volume->getContentSize().height/3);
    if(!volume_text){
        problemLoading("Text: Main Volume");
    }
    else{
        volume_text->setTextColor(Color4B(238,56,56,255)); //238,56,56
        text_background_volume->addChild(volume_text);
        volume_text->setPosition(Vec2(left_margin * 0.7, up_margin/2.4));
    }
    
    auto volume_slider = Slider::create();
    volume_slider->loadBarTexture("Menu/SliderTrack.png");
    volume_slider->loadSlidBallTextures("Menu/SliderThumb.png", "Menu/SliderThumb.png", "");
    volume_slider->loadProgressBarTexture("Menu/SliderProgress.png");
    if(!volume_slider){
        problemLoading("Volume Slider/SliderTrack or SliderThumb or SliderProgress");
    }
    else{
        float music_percent = UserDefault::getInstance()->getFloatForKey("music_percent");
        if(music_percent == 0.0f){
            music_percent = 100.0f;
        }
        volume_slider->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        volume_slider->setPercent(music_percent);
        volume_slider->setPosition(Vec2((text_background_volume->getPosition()).x + (text_background_volume->getContentSize()).width + small_adjust, (text_background_volume->getPosition()).y + (text_background_volume->getContentSize()).height/2) );
        volume_slider->addEventListener([=](Ref* pSender, Slider::EventType event_type){
            switch (event_type) {
                case Slider::EventType::ON_PERCENTAGE_CHANGED:{
                    int percent = volume_slider->getPercent();
                    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(static_cast<float>(percent)/100);
                    UserDefault::getInstance()->setFloatForKey("music_percent", percent);
                    break;
                }
                
                default:
                    break;
            }
        });
    }
    
    LayerGradient *text_background_effects = LayerGradient::create(Color4B(75,0,130,200),Color4B(147,112,219,200));
    if(!text_background_effects){
        problemLoading("LayerGradient");
    }
    else{
        text_background_effects->changeWidthAndHeight(left_margin * 1.4, up_margin/1.2);
        text_background_effects->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        text_background_effects->setPosition(Vec2(left_margin,visible_height - up_margin - text_background_volume->getContentSize().height * 2 - small_adjust));
    }
    
    auto effects_text = Text::create("Effects Volume", "arial.ttf", text_background_volume->getContentSize().height/3);
    if(!effects_text){
        problemLoading("Text: Main Volume");
    }
    else{
        effects_text->setTextColor(Color4B(238,56,56,255)); //238,56,56
        text_background_effects->addChild(effects_text);
        effects_text->setPosition(Vec2(left_margin * 0.7, up_margin/2.4));
    }
    
    auto effects_slider = Slider::create();
    effects_slider->loadBarTexture("Menu/SliderTrack.png");
    effects_slider->loadSlidBallTextures("Menu/SliderThumb.png", "Menu/SliderThumb.png", "");
    effects_slider->loadProgressBarTexture("Menu/SliderProgress.png");
    if(!effects_slider){
        problemLoading("Volume Slider/SliderTrack or SliderThumb or SliderProgress");
    }
    else{
        float effects_percent = UserDefault::getInstance()->getFloatForKey("effects_percent");
        if(effects_percent == 0.0f){
            effects_percent = 100.0f;
        }
        effects_slider->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        effects_slider->setPercent(effects_percent);
        effects_slider->setPosition(Vec2((text_background_volume->getPosition()).x + (text_background_volume->getContentSize()).width + small_adjust, (text_background_volume->getPosition()).y + (text_background_volume->getContentSize()).height * -1/2 - small_adjust));
        effects_slider->addEventListener([=](Ref* pSender, Slider::EventType event_type){
            switch (event_type) {
                case Slider::EventType::ON_PERCENTAGE_CHANGED:{
                    int percent = effects_slider->getPercent();
                    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(static_cast<float>(percent)/100);
                    UserDefault::getInstance()->setFloatForKey("effects_percent", percent);
                    break;
                }
                    
                default:
                    break;
            }
        });
    }
    
    auto return_button = Button::create("Menu/OrangeButton.png", "Menu/OrangeButtonHighlight.png");
    if(!return_button){
        problemLoading("Menu/OrangeButton.png and Menu/OrangeButtonHighlight.png");
    }
    else{
        auto button_size = return_button->getCustomSize();
        return_button->setScale(1.0);
        return_button->setTitleText("Return");
        return_button->setTitleFontName("Arial-Bold.ttf");
        return_button->setTitleColor(Color3B(80,80,80));
        return_button->setTitleFontSize(button_size.height/3);
        return_button->setPosition(Vec2(visible_size.width/2, button_size.height));
        return_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType event_type){
            switch (event_type) {
                case Widget::TouchEventType::BEGAN:{
                    return_button->setScale(1.05);
                    break;
                }
                    
                case Widget::TouchEventType::ENDED:{
                    return_button->setScale(1.0);
                    SettingLayer::close(this);
                    break;
                }
                    
                case Widget::TouchEventType::CANCELED:{
                    return_button->setScale(1.0);
                    break;
                }
                    
                default:
                    break;
            }
        });
    }
    //!!!Iteration?
    Vector<Node *> fade_targets{text_background_volume, text_background_effects, effects_slider, volume_slider, return_button, volume_text, effects_text};
    for(auto target : fade_targets){
        target->setOpacity(0);
    }
    
    layer->addChild(text_background_volume, 0);
    layer->addChild(volume_slider, 0);
    layer->addChild(text_background_effects, 0);
    layer->addChild(effects_slider, 0);
    layer->addChild(return_button, 0);
    
    groupFadeIn(fade_targets);
    auto background_fade = FadeTo::create(0.5, 150);
    layer->runAction(background_fade);
    return true;
}

void SettingLayer::groupFadeIn(cocos2d::Vector<Node*> group){
    for(auto target : group){
        auto fade_in = FadeIn::create(0.5);
        target->runAction(fade_in);
    }
}

void SettingLayer::onEnter()
{
    Layer::onEnter();
//    std::cout << (typeid(*this)== typeid(SettingLayer)) << std::endl;
//    std::cout << (typeid(*(this->getParent()))== typeid(MenuScene)) << std::endl;
    if(!_swallow_listner){
        _swallow_listner = EventListenerTouchOneByOne::create();
        _swallow_listner->onTouchBegan = [](Touch* touch, Event* event)
        {
            return true;
        };
        _swallow_listner->setSwallowTouches(true);
        if(_swallow_listner){
            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_swallow_listner, this);
        }
    }
}

void SettingLayer::onExit(){
    Layer::onExit();
    if(_swallow_listner){
        Director::getInstance()->getEventDispatcher()->removeEventListener(_swallow_listner);
    }
}

void SettingLayer::fadeOutIteration(Vector<Node*> targets){
    if(!targets.empty()){
        for(auto target : targets){
            auto fade_out = FadeOut::create(0.5);
            target->runAction(fade_out);
            fadeOutIteration(target->getChildren());
        }
    }
}

void SettingLayer::removeLayer(float dt){
    this->getParent()->removeChild(this);
}

void SettingLayer::close(Node* pSender){
    Vector<Node*> targets;
    targets.pushBack(pSender);
    fadeOutIteration(targets);
    pSender->scheduleOnce(schedule_selector(SettingLayer::removeLayer), 0.5);
}
