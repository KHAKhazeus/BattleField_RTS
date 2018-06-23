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
	/**
	* \brief create a socket client
	* \param ip ip address, default to localhost
	* \param port port number, default to 8080
	* \return a socket client
	*/
	static SocketClient* create(std::string ip = "127.0.0.1", int port = 8080);

	~SocketClient() { doClose(); }

	/**
	* \brief close the socket
	*/
	void close();

	void setMapselect(int mapID) { _mapSelect = mapID; }
	int getMapselect() { return _mapSelect; }

	/**
	* \brief start a socket
	*/
	void start()
	{
		startConnect();
	};
	std::vector<GameMessage> getGameMessages();

	void sendGameMessages(const std::vector<GameMessage>& vec_game_msg);


	/**
	* \brief send string through socket
	* \param s protubuf serialized string
	*/
	void send_string(std::string s);

	/**
	* \brief this is a block function of receving stirng
	* \return protubuf serialized string
	*/
	std::string get_string();

	/**
	* \brief inner use
	*/
	void doClose();


	/**
	* \brief
	* \return if game has started
	*/
	bool started() const { return _start_Flag; }
	/**
	* \brief
	* \return if there is isError
	*/
	bool error()const { return _error_Flag; }

	/**
	* \brief start from 1
	* \return camp number
	*/
	int camp() const;

	/**
	* \brief
	* \return total player number
	*/
	int total() const;

private:
	SocketClient(std::string ip, int port) : _socket(_io_service),
		_endpoint(boost::asio::ip::address_v4::from_string(ip), port)
	{
		start();
	}


	void write_data(std::string s);

	void startConnect();

	void handle_connect(const error_code& error);

	void handle_read_header(const error_code& error);

	void handle_read_body(const error_code& error);

	std::string read_data();
private:

	boost::asio::io_service _io_service;
	tcp::socket	_socket;
	tcp::endpoint _endpoint;
	std::deque<SocketMessage> _read_Msg_Deque;
	SocketMessage _read_msg;

	bool _start_Flag{ false }, _error_Flag{ false };

	std::thread *_thread;
	int _camp;
	int _mapSelect;
	std::condition_variable _data_cond;
	std::mutex _mut;
};


#endif /* __SOCKET_CLIENT_H__*/ 
