//
//  GameMessageInterface.h
//  BattleField_RTS
//
//  Created by Khazeus on 2018/6/8.
//

#ifndef __GAME_MESSAGE_INTERFACE_H__
#define __GAME_MESSAGE_INTERFACE_H__
#include "GameMessage.pb.h"
#include "cocos2d.h"
#include <vector>

class GameMessageInterface{
    GameMessageInterface() = delete;    //should prevent any instances
    ~GameMessageInterface();
    static std::string combineMessagesToString(std::vector<GameMessage> game_messages);
    
    static std::vector<GameMessage> parseMessagesFromString(std::string data);
    
    static GameMessage newCreateBuildingMessage(int new_building_id, std::string new_building_type, int base_id, int from_building_id);
    
    static GameMessage newCreateUnitMessage(int new_unit_id, std::string new_unit_type, int base_id, int from_building_id);
    
    static GameMessage newMoveMessage(int move_unit_id, std::vector<cocos2d::Vec2> path_points, cocos2d::Vec2 end_point);
    
    static GameMessage newAttackMessage(int attacker_id, int under_attack_id, int damage);
};

#endif //__GAME_MESSAGE_INTERFACE_H__
