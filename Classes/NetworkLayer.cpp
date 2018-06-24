//
//  NetworkLayer.cpp
//  BattleField_RTS
//
//  Created by Khazeus on 2018/6/10.
//

#include "NetworkLayer.h"
#include "SimpleAudioEngine.h"
#include "MenuScene.h"
#include "GameMessageOperation.h"
#include "LoadingScene.h"

#ifdef TEST
#include <iostream>
#endif

USING_NS_CC;
using namespace ui;

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in NetworkLayer.cpp\n");
}

void NetworkLayer::initializeServerSide(){
    if(!server && !client){
        auto color_layer = this->getChildByName("color_layer");
        auto portbox = static_cast<TextField *>(color_layer->getChildByName("portbox"));
        auto ipbox = static_cast<TextField *>(color_layer->getChildByName("ipbox"));
        auto ip = ipbox->getString();
        std::stringstream port_stream(portbox->getString());
        int port_number;
        port_stream >> port_number;
        try{
            if(!_socket_server){
                auto new_socket_server = SocketServer::create(port_number);
				if (_socket_server != NULL) {
					delete _socket_server;
				}
                if(new_socket_server){
                    _socket_server = new_socket_server;
                }
            }
            if(!_socket_client){
				if (_socket_client != NULL) {
					delete _socket_client;
				}
                auto new_socket_client = SocketClient::create(ip, port_number);
                if(new_socket_client){
                    _socket_client = new_socket_client;
                }
            }
            server = true;
        }
        catch(boost::system::system_error){
			_socket_client = NULL;
            std::cerr << "Server Or Client Create Denied, Resetting, Try Again" << std::endl;
        }
    }
}

void NetworkLayer::initializeClientSide(){
    if(!server && !client){
        auto color_layer = this->getChildByName("color_layer");
        auto ipbox = static_cast<TextField *>(color_layer->getChildByName("ipbox"));
        auto portbox = static_cast<TextField *>(color_layer->getChildByName("portbox"));
        auto ip = ipbox->getString();
        std::stringstream port_stream(portbox->getString());
        int port_number;
        port_stream >> port_number;
        try{
            if((!_socket_client)){
				if (_socket_client != NULL) {
					delete _socket_client;
				}
                auto new_socket_client = SocketClient::create(ip, port_number);
                if(new_socket_client){
                    _socket_client = new_socket_client;
                }
            }
            client = true;
        }
        catch(boost::system::system_error){
            _socket_client = NULL;
            std::cerr << "Client Create Denied, Resetting, Try Again" << std::endl;
        }
    }
}

void NetworkLayer::resetClientAndServer(){
    if(_socket_client){
        _socket_client->doClose();
		if (_socket_client != NULL) {
			delete _socket_client;
			_socket_client = NULL;
		}
    }
    if(_socket_server && server){
        _socket_server->close();
		if (_socket_server != NULL) {
			delete _socket_server;
			_socket_server = NULL;
		}
    }
	//_socket_client.reset(static_cast<SocketClient*>(nullptr),[](SocketClient*){});
   // _socket_server.reset(static_cast<SocketServer*>(nullptr),[](SocketServer*){});
    client = false;
    server = false;
    log("%s", "Client and Server Reset");
}

bool NetworkLayer::init(){
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
    layer->setName("color_layer");
    
    this->addChild(layer);
    
    
    Button *start_server = Button::create("Menu/OrangeButton.png", "Menu/OrangeButtonHighlight.png");
    float small_adjust = (start_server->getContentSize()).height/4;
    if(!start_server){
        problemLoading("Menu/OrangeButton.png and Menu/OrangeButtonHighlight.png");
    }
    else{
        auto button_size = start_server->getCustomSize();
        start_server->setScale(1.0);
        start_server->setTitleText("Start Server");
        start_server->setTitleFontName("Arial-Bold.ttf");
        start_server->setTitleColor(Color3B(80,80,80));
        start_server->setTitleFontSize(button_size.height/3);
        start_server->setPosition(Vec2(visible_size.width/2, visible_size.height/2 + button_size.height/2 + small_adjust));
        start_server->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType event_type){
            switch (event_type) {
                case Widget::TouchEventType::BEGAN:{
                    start_server->setScale(1.05);
                    break;
                }
                    
                case Widget::TouchEventType::ENDED:{
                    start_server->setScale(1.0);
                    NetworkLayer::initializeServerSide();    //need to be extended
			//		auto temp = _socket_client->getGameMessages();
			//		auto str = GameMessageOperation::vectorToString(temp);
			//		cocos2d::log("%s", str);
                    break;
                }
                    
                case Widget::TouchEventType::CANCELED:{
                    start_server->setScale(1.0);
                    break;
                }
                
                default:
                    break;
            }
        });
    }
    
    auto start_server_size = start_server->getContentSize();
    
    
    auto ipbox = TextField::create("Input IPv4", "Arial-Bold.ttf", start_server_size.height/3);
    if(!ipbox){
        problemLoading("Menu/InputBox.png");
    }
    else{
        auto ipbox_size = ipbox->getContentSize();
        ipbox->setName("ipbox");
        ipbox->setPlaceHolderColor(Color4B(255,255,255,200));
        ipbox->setPosition(Vec2(visible_width/2 - ipbox_size.width * 0.8 - small_adjust, visible_height/2 + start_server_size.height + ipbox_size.height/2 + small_adjust + up_margin));
        ipbox->setMaxLength(15);
        ipbox->setTextColor(Color4B::WHITE);
        ipbox->setTextHorizontalAlignment(TextHAlignment::LEFT);
        ipbox->addEventListener([=](Ref *pSender, TextField::EventType event_type){
            switch (event_type) {
                case TextField::EventType::ATTACH_WITH_IME:
                    break;
                case TextField::EventType::DETACH_WITH_IME:
                    break;
                case TextField::EventType::INSERT_TEXT:{
                    auto string = ipbox->getString();
                    auto length = (ipbox->getString()).length();
                    if(length > ipbox->getMaxLength()){
                        ipbox->setString(string.substr(0,ipbox->getMaxLength()));
                    }
                    break;
                }
                case TextField::EventType::DELETE_BACKWARD:
                    break;
                default:
                    break;
            }
        });
    }
    
    auto box_size = ipbox->getCustomSize();
    
    //!!!need to be s
    LayerGradient *text_ip_background = LayerGradient::create(Color4B(139,0,0,0),Color4B(139,35,35,200));
    if(!text_ip_background){
        problemLoading("LayerGradient");
    }
    else{
        text_ip_background->changeWidthAndHeight(left_margin * 1.4, up_margin/1.2);
        text_ip_background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        text_ip_background->setPosition(Vec2((ipbox->getPosition()).x - (text_ip_background->getContentSize()).width/2 - box_size.width * 2, (ipbox->getPosition()).y - (text_ip_background->getContentSize()).height/2));
    }
    
    auto ipbox_text = Text::create("Input Server IPv4", "arial.ttf", text_ip_background->getContentSize().height/3.5);
    if(!ipbox_text){
        problemLoading("Text: Input server IP");
    }
    else{
        ipbox_text->setTextColor(Color4B(238,56,56,255)); //238,56,56
        text_ip_background->addChild(ipbox_text);
        ipbox_text->setPosition(Vec2(left_margin * 0.7, up_margin/2.4));
    }
    
    
    LayerGradient *text_port_background = LayerGradient::create(Color4B(139,0,0,0),Color4B(139,35,35,200));
    if(!text_port_background){
        problemLoading("LayerGradient");
    }
    else{
        text_port_background->changeWidthAndHeight(left_margin * 1.4, up_margin/1.2);
        text_port_background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        text_port_background->setPosition(Vec2((ipbox->getPosition()).x  + box_size.width * 1.5, (ipbox->getPosition()).y - (text_ip_background->getContentSize()).height/2));
    }
    
    
    auto port_text = Text::create("Input Port Number", "arial.ttf", text_ip_background->getContentSize().height/3.5);
    if(!port_text){
        problemLoading("Text: Main Volume");
    }
    else{
        port_text->setTextColor(Color4B(238,56,56,255)); //238,56,56
        text_port_background->addChild(port_text);
        port_text->setPosition(Vec2(left_margin * 0.7, up_margin/2.4));
    }
    
    auto portbox = TextField::create("port", "Arial-Bold.ttf", start_server_size.height/3);
    if(!portbox){
        problemLoading("Menu/InputBox.png");
    }
    else{
        auto portbox_size = portbox->getContentSize();
        portbox->setName("portbox");
        portbox->setPlaceHolderColor(Color4B(255,255,255,200));
        portbox->setPosition(Vec2((text_port_background->getPosition()).x + portbox_size.width * 1.8 + small_adjust + (text_port_background->getContentSize()).width/2, visible_height/2 + start_server_size.height + portbox_size.height/2 + small_adjust + up_margin));
        portbox->setMaxLength(4);
        portbox->setTextColor(Color4B::WHITE);
        portbox->setTextHorizontalAlignment(TextHAlignment::LEFT);
        portbox->addEventListener([=](Ref *pSender, TextField::EventType event_type){
            switch (event_type) {
                case TextField::EventType::ATTACH_WITH_IME:
                    break;
                case TextField::EventType::DETACH_WITH_IME:
                    break;
                case TextField::EventType::INSERT_TEXT:{
                    auto string = portbox->getString();
                    auto length = (portbox->getString()).length();
                    if(length > portbox->getMaxLength()){
                        portbox->setString(string.substr(0,portbox->getMaxLength()));
                    }
                    break;
                }
                case TextField::EventType::DELETE_BACKWARD:
                    break;
                default:
                    break;
            }
        });
    }
    
    Button *start_client = Button::create("Menu/OrangeButton.png", "Menu/OrangeButtonHighlight.png");
    if(!start_client){
        problemLoading("Menu/OrangeButton.png and Menu/OrangeButtonHighlight.png");
    }
    else{
        auto button_size = start_client->getCustomSize();
        start_client->setScale(1.0);
        start_client->setTitleText("Start Client");
        start_client->setTitleFontName("Arial-Bold.ttf");
        start_client->setTitleColor(Color3B(80,80,80));
        start_client->setTitleFontSize(button_size.height/3);
        start_client->setPosition(Vec2((start_server->getPosition()).x, (start_server->getPosition()).y -start_server_size.height/2 - small_adjust));
        start_client->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType event_type){
            switch (event_type) {
                case Widget::TouchEventType::BEGAN:{
                    start_client->setScale(1.05);
                    break;
                }
                    
                case Widget::TouchEventType::ENDED:{
                    start_client->setScale(1.0);
                    auto ip_string = ipbox->getString();
                    auto port_string = portbox->getString();
					if ((_socket_client == NULL) && !_socket_server) {
						NetworkLayer::initializeClientSide();    //need to be extended
					}
                    if(client && _socket_client){
                        this->schedule(schedule_selector(NetworkLayer::startSchedule), 0.1f);
                    }
                   

                    break;
                }
                    
                case Widget::TouchEventType::CANCELED:{
                    start_client->setScale(1.0);
                    break;
                }
                    
                default:
                    break;
            }
        });
    }
    
    Button *reset = Button::create("Menu/OrangeButton.png", "Menu/OrangeButtonHighlight.png");
    if(!reset){
        problemLoading("Menu/OrangeButton.png and Menu/OrangeButtonHighlight.png");
    }
    else{
        auto button_size = reset->getCustomSize();
        reset->setScale(1.0);
        reset->setTitleText("Reset Client/Server");
        reset->setTitleFontName("Arial-Bold.ttf");
        reset->setTitleColor(Color3B(80,80,80));
        reset->setTitleFontSize(button_size.height/4);
        reset->setPosition(Vec2((start_client->getPosition()).x, (start_client->getPosition()).y -start_server_size.height/2 - small_adjust));
        reset->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType event_type){
            switch (event_type) {
                case Widget::TouchEventType::BEGAN:{
                    reset->setScale(1.05);
                    break;
                }
                    
                case Widget::TouchEventType::ENDED:{
                    reset->setScale(1.0);
                    auto ip_default_string = StringUtils::toString("127.0.0.1");
                    auto port_default_string = StringUtils::toString("8080");
                    ipbox->setString(ip_default_string);
                    portbox->setString(port_default_string);
                    NetworkLayer::resetClientAndServer();    //need to be extended
                    break;
                }
                    
                case Widget::TouchEventType::CANCELED:{
                    reset->setScale(1.0);
                    break;
                }
                    
                default:
                    break;
            }
        });
    }


	Button *select_map = Button::create("Menu/OrangeButton.png", "Menu/OrangeButtonHighlight.png");
	if (!select_map) {
		problemLoading("Menu/OrangeButton.png and Menu/OrangeButtonHighlight.png");
	}
	else {
		auto button_size = select_map->getCustomSize();
		select_map->setScale(1.0);
		select_map->setTitleText("Select Map Lost Temple");
		select_map->setTitleFontName("Arial-Bold.ttf");
		select_map->setTitleColor(Color3B(80, 80, 80));
		select_map->setTitleFontSize(button_size.height / 5);
		select_map->setPosition(Vec2((reset->getPosition()).x, (reset->getPosition()).y - start_server_size.height / 2 - small_adjust));
		select_map->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType event_type) {
			switch (event_type) {
			case Widget::TouchEventType::BEGAN: {
				select_map->setScale(1.05);
				break;
			}

			case Widget::TouchEventType::ENDED: {
				select_map->setScale(1.0);
				if (_socket_server != NULL) {
					if (select_map->getTitleText().compare("Select Map Lost Temple") == 0) {
						select_map->setTitleText("Select Map Snow World");
						_socket_server->setMapselect(SNOWMAP);
						TiledMap::setMapFlagSnow();
					}
					else if (select_map->getTitleText().compare("Select Map Snow World") == 0) {
						select_map->setTitleText("Select Map Lost Temple");
						_socket_server->setMapselect(LOSTTEMP);
						TiledMap::setMapFlagLost();
					}
				}
				break;
			}

			case Widget::TouchEventType::CANCELED: {
				select_map->setScale(1.0);
				break;
			}

			default:
				break;
			}
		});
	}
    
    Button *start_game = Button::create("Menu/OrangeButton.png", "Menu/OrangeButtonHighlight.png");
    if(!reset){
        problemLoading("Menu/OrangeButton.png and Menu/OrangeButtonHighlight.png");
    }
    else{
        auto button_size = start_game->getCustomSize();
        start_game->setScale(1.0);
        start_game->setTitleText("Start!");
        start_game->setTitleFontName("Arial-Bold.ttf");
        start_game->setTitleColor(Color3B(80,80,80));
        start_game->setTitleFontSize(button_size.height/3);
        start_game->setPosition(Vec2((select_map->getPosition()).x, (select_map->getPosition()).y -start_server_size.height/2 - small_adjust));
        start_game->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType event_type){
            switch (event_type) {
                case Widget::TouchEventType::BEGAN:{
                    start_game->setScale(1.05);
                    break;
                }
                    
                case Widget::TouchEventType::ENDED:{
                    start_game->setScale(1.0);
				/*	if (_socket_server != NULL) {
						_socket_server->startService();
					}*/
                    if(_socket_server && _socket_client){
                        _socket_server->clickStart();
						auto gameScene = GameScene::createScene(_socket_server, _socket_client);
						auto sceneAniamte = TransitionCrossFade::create(0.5f, gameScene);
						Director::getInstance()->replaceScene(sceneAniamte);
                    }
				/*	auto temp = _socket_client->getGameMessages();
					auto str = GameMessageOperation::vectorToString(temp);
					cocos2d::log("%s", str);*/
                    //need to be extended
                    break;
                }
                    
                case Widget::TouchEventType::CANCELED:{
                    start_game->setScale(1.0);
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
					if (_socket_client != NULL) {
						_socket_client->close();
						delete _socket_client;
						_socket_client = NULL;
					}

					
					if (_socket_server != NULL) {
						_socket_server->close();
						delete _socket_server;
						_socket_server = NULL;
					}
					
                    NetworkLayer::close(this);
					
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
    Vector<Node *> fade_targets{text_ip_background, text_port_background, return_button, start_server, start_client, portbox, ipbox, reset, start_game,select_map};
    for(auto target : fade_targets){
        target->setOpacity(0);
        layer->addChild(target);
    }
    
    fadeInIteration(fade_targets);
    auto background_fade = FadeTo::create(0.5, 170);
    layer->runAction(background_fade);
    return true;
}

void NetworkLayer::fadeInIteration(Vector<Node*> targets){
    if(!targets.empty()){
        for(auto target : targets){
            auto fade_in = FadeIn::create(0.5);
            target->runAction(fade_in);
            fadeInIteration(target->getChildren());
        }
    }
}

void NetworkLayer::onEnter()
{
    Layer::onEnter();
    //    std::cout << (typeid(*this)== typeid(NetworkLayer)) << std::endl;
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

void NetworkLayer::onExit(){
    Layer::onExit();
    if(_swallow_listner){
        Director::getInstance()->getEventDispatcher()->removeEventListener(_swallow_listner);
    }
}

void NetworkLayer::fadeOutIteration(Vector<Node*> targets){
    if(!targets.empty()){
        for(auto target : targets){
            auto fade_out = FadeOut::create(0.5);
            target->runAction(fade_out);
            fadeOutIteration(target->getChildren());
        }
    }
}

void NetworkLayer::removeLayer(float dt){
    this->getParent()->removeChild(this);
}

void NetworkLayer::close(Node* pSender){
    Vector<Node*> targets;
    targets.pushBack(pSender);
    fadeOutIteration(targets);
    pSender->scheduleOnce(schedule_selector(NetworkLayer::removeLayer), 0.5);
}


void NetworkLayer::startSchedule(float dt) {
	if ( _socket_client && _socket_client->started()) {
		wait_start();
	}
}
void NetworkLayer::wait_start() {
	unscheduleAllSelectors();
	auto map = _socket_client->getMapselect();
	cocos2d::log("%d\n", map);
	if (_socket_client->getMapselect() == LOSTTEMP) {
		TiledMap::setMapFlagLost();
		
	}
	else if (_socket_client->getMapselect() == SNOWMAP) {
		TiledMap::setMapFlagSnow();
	}
	auto gameScene = GameScene::createScene(_socket_server, _socket_client);
	auto sceneAniamte = TransitionCrossFade::create(0.5f, gameScene);
	Director::getInstance()->replaceScene(sceneAniamte);	
}
