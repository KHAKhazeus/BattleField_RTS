//
//  GameMessageInterface.c
//  BattleField_RTS
//
//  Created by Khazeus on 2018/6/8.
//

#include "GameMessageInterface.h"

static std::string combineMessagesToString(std::vector<GameMessage> game_messages){
    GameMessageGroup game_message_group;
    for(auto msg: game_messages){
        auto new_game_message = game_message_group.add_game_message();
        new_game_message->CopyFrom(msg);
    }
    auto result = game_message_group.SerializeAsString();
    return result;
}

static std::vector<GameMessage> parseMessagesFromString(std::string data){
    GameMessageGroup game_message_group;
    game_message_group.ParseFromString(data);
    std::vector<GameMessage> result;
    for(auto num = 0; num < game_message_group.game_message_size(); num++ ){
        auto ret = game_message_group.game_message(num);
        result.push_back(ret);
    }
    return result;
}
