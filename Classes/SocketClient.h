//
//  SocketClient.h
//  BattleField_RTS
//
//  Created by Khazeus on 2018/6/10.
//

#ifndef __SOCKET_CLIENT_H__
#define __SOCKET_CLIENT_H__

#include "include/boost/asio.hpp"
//#include "GameMessage.pb.h"
#include "GameMessageInterface.h"
#include "SocketMessage.h"
#include "BattleField_RTS.h"
#include "GameMessageOperation.h"
#include <thread>
#include <memory>
#include <string>
#include <mutex>


using boost::asio::ip::tcp;
using boost::system::error_code;

class SocketClient
{
public:
	//@@param1 is a str refer to ip address, default  localhost
	//@@param2 is an int refer to port number, default  8080
	//create a SocketClient Pointer
	static SocketClient* create(std::string ip = "127.0.0.1", int port = 8080);

	~SocketClient() { doClose(); }

	//Close operation
	void close();

	//Get or Set Map operation
	void setMapselect(int mapID) { _mapSelect = mapID; }
	int getMapselect() { return _mapSelect; }

	//Start Operation
	void start()
	{
		startConnect();
	};

	std::vector<GameMessage> getGameMessages();

	void sendGameMessages(const std::vector<GameMessage>& vec_game_msg);


	

	//Close operation which be called by close()
	void doClose();


	//To judge if it has already been connected
	bool isStart() const { return _start_Flag; }
	
	//To judge if there is something error
	bool error()const { return _error_Flag; }

	//To get the client camp
	int camp() const;


private:
	SocketClient(std::string ip, int port) : _socket(_io_service),
		_endpoint(boost::asio::ip::address_v4::from_string(ip), port)
	{
		start();
	}

	//Write Operation
	void write_data(std::string s);

	void startConnect();

	void handle_connect(const error_code& error);

	void handle_read_header(const error_code& error);

	void handle_read_body(const error_code& error);

	//Read Operation 
	std::string read_data();
private:

	boost::asio::io_service _io_service;
	tcp::socket	_socket;
	tcp::endpoint _endpoint;
	std::deque<SocketMessage> _read_Msg_Deque;
	SocketMessage _read_msg;

	//Judge flag
	bool _start_Flag{ false }, _error_Flag{ false };

	// thread to bind io_service run
	std::thread *_thread;
	int _camp;
	int _mapSelect;
	std::condition_variable _data_cond;
	std::mutex _mut;
};


#endif /* __SOCKET_CLIENT_H__*/ 
