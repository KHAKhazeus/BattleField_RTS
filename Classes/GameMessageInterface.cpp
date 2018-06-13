//
//  GameMessageInterface.c
//  BattleField_RTS
//
//  Created by Khazeus on 2018/6/8.
//

#include "GameMessageInterface.h"

#define CLIENT_ERROR "Error"
#define CLIENT_CANCEL "Cancelled"


std::string GameMessageInterface::combineMessagesToString(std::vector<GameMessage> game_messages){
    GameMessageGroup game_message_group;
    for(auto msg: game_messages){
        auto new_game_message = game_message_group.add_game_message();
        new_game_message->CopyFrom(msg);
    }
    auto result = game_message_group.SerializeAsString();
    return result;
}

//use this function after excluding CLIENT_ERROR & CLIENT_CANCEL

std::vector<GameMessage> GameMessageInterface::parseMessagesFromString(std::string data){
    GameMessageGroup game_message_group;
    game_message_group.ParseFromString(data);
    std::vector<GameMessage> result;
    for(auto num = 0; num < game_message_group.game_message_size(); num++ ){
        auto ret = game_message_group.game_message(num);
        result.push_back(ret);
    }
    return result;
}

GameMessage GameMessageInterface::newCreateBuildingMessage(int new_building_id, std::string new_building_type, int base_id, int from_building_id){
    GameMessage CRTBU_new_message;
    CRTBU_new_message.set_unit_0(new_building_id);
    CRTBU_new_message.set_create_type(new_building_type);
    CRTBU_new_message.set_base(base_id);
    CRTBU_new_message.set_building(from_building_id);
    CRTBU_new_message.set_cmd_code(GameMessage_CmdCode::GameMessage_CmdCode_CRTBU);
    return CRTBU_new_message;
}

GameMessage GameMessageInterface::newCreateUnitMessage(int new_unit_id, std::string new_unit_type, int base_id, int from_building_id){
    GameMessage CRTBD_new_message;
    CRTBD_new_message.set_unit_0(new_unit_id);
    CRTBD_new_message.set_create_type(new_unit_type);
    CRTBD_new_message.set_base(base_id);
    CRTBD_new_message.set_building(from_building_id);
    CRTBD_new_message.set_cmd_code(GameMessage_CmdCode::GameMessage_CmdCode_CRTBD);
    return CRTBD_new_message;
}

GameMessage GameMessageInterface::newMoveMessage(int move_unit_id, std::vector<cocos2d::Vec2> path_points, cocos2d::Vec2 end_point){
    GameMessage MOV_new_message;
    MOV_new_message.set_unit_0(move_unit_id);
    int times = 0;
    for(auto point: path_points){
        GridPoint grid_point;
        grid_point.set_x(static_cast<int>(point.x));
        grid_point.set_y(static_cast<int>(point.y));
        MOV_new_message.mutable_grid_path()->add_grid_point();
        *(MOV_new_message.mutable_grid_path()->mutable_grid_point(times)) = grid_point;
        times ++;
    }
    GridPoint end_grid_point;
    end_grid_point.set_x(static_cast<int>(end_point.x));
    end_grid_point.set_y(static_cast<int>(end_point.y));
    MOV_new_message.mutable_grid_path()->add_grid_point();
    *(MOV_new_message.mutable_grid_path()->mutable_grid_point(times)) = end_grid_point;
    MOV_new_message.set_cmd_code(GameMessage_CmdCode::GameMessage_CmdCode_MOV);
    return MOV_new_message;
}

GameMessage GameMessageInterface::newAttackMessage(int attacker_id, int under_attack_id, int damage){
    GameMessage ATK_new_message;
    ATK_new_message.set_unit_0(attacker_id);
    ATK_new_message.set_unit_1(under_attack_id);
    ATK_new_message.set_damage(damage);
    ATK_new_message.set_cmd_code(GameMessage_CmdCode::GameMessage_CmdCode_ATK);
    return ATK_new_message;
}

GameMessage GameMessageInterface::newEMPMessage(){
    GameMessage EMP_new_message;
    EMP_new_message.set_cmd_code(GameMessage_CmdCode::GameMessage_CmdCode_EMP);
    return EMP_new_message;
}

