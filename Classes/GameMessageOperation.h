#ifndef _GAME_MESSAGE_OPERATION
#define _GAME_MESSAGE_OPERATION
#include <vector>
#include <string>
#include "cocos2d.h"
#include "GameMessage.pb.h"


class GameMessageOperation
{
public:
	GameMessageOperation();
	~GameMessageOperation();
	static std::vector<GameMessage> stringToVector(const std::string& string)
	{
		GameMessageGroup gameMessageSet;
		gameMessageSet.ParseFromString(string);
		std::vector<GameMessage> msgVector;
		for (auto i = 0; i < gameMessageSet.game_message_size(); ++i)
			msgVector.push_back(gameMessageSet.game_message(i));
		return msgVector;
	}

	static std::string vectorToString(const std::vector<GameMessage>& msgVector)
	{
		GameMessageGroup gameMessageSet;
		for (auto temp : msgVector)
		{
			auto game_message = gameMessageSet.add_game_message();
			game_message->CopyFrom(temp);
		}
		return gameMessageSet.SerializeAsString();
	}


	static std::string combineMessage(const std::vector<std::string>& msg)
	{
		GameMessageGroup gameMessageSet;

		for (auto temp : msg)
		{
			//cocos2d::log("%s\n", temp);
			GameMessageGroup gms;
			gms.ParseFromString(temp);
			gameMessageSet.MergeFrom(gms);
		}
		return gameMessageSet.SerializeAsString();
	}
};
#endif // !_GAME_MESSAGE_OPERATION
