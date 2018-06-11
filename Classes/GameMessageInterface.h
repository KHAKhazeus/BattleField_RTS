//
//  GameMessageInterface.h
//  BattleField_RTS
//
//  Created by Khazeus on 2018/6/8.
//

#ifndef __GAME_MESSAGE_INTERFACE_H__
#define __GAME_MESSAGE_INTERFACE_H__
#include "GameMessage.pb.h"
#include <vector>

class GameMessageInterface{
    GameMessageInterface() = delete;    //should prevent any instances
    ~GameMessageInterface();
    static std::string combineMessagesToString(std::vector<GameMessage> game_messages);
    
    static std::vector<GameMessage> parseMessagesFromString(std::string data);
    
};

#endif //__GAME_MESSAGE_INTERFACE_H__
