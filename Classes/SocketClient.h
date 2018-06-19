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
	* \param port port number, default to 8008
	* \return a socket client
	*/
	static SocketClient* create(std::string ip = "127.0.0.1", int port = 8008);

	//	~SocketClient() {  io_service_.stop();do_close(); }

	/**
	* \brief close the socket
	*/
	void close();

	/**
	* \brief start a socket
	*/
	void start()
	{
		start_connect();
	};
	[[deprecated("just for test")]]
	std::vector<GameMessage> get_game_messages();

	[[deprecated("just for test")]]
	void send_game_message(const std::vector<GameMessage>& vec_game_msg);


	/**
	* \brief send string through socket
	* \param s protubuf serialized string
	*/
	void send_string(std::string s);

	/**
	* \brief this is a block function of receving stirng
	* \return protubuf serialized string
	*/
	std::string get_string();;

	/**
	* \brief inner use
	*/
	void do_close();



	/**
	* \brief
	* \return if game has started
	*/
	bool started() const { return start_flag_; }
	/**
	* \brief
	* \return if there is error
	*/
	bool error()const { return error_flag_; }

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
	SocketClient::SocketClient(std::string ip, int port) : socket_(io_service_),
		endpoint_(boost::asio::ip::address_v4::from_string(ip), port)
	{
		start();
	}


	void write_data(std::string s);

	void start_connect();

	void handle_connect(const error_code& error);

	void handle_read_header(const error_code& error);

	void handle_read_body(const error_code& error);

	std::string read_data();
private:


	boost::asio::io_service io_service_;
	tcp::socket	socket_;
	tcp::endpoint endpoint_;
	std::deque<SocketMessage> read_msg_deque_;
	SocketMessage read_msg_;

	bool start_flag_{ false }, error_flag_{ false };

	std::thread *thread_, *read_thread_;
	int camp_, total_;

	std::condition_variable data_cond_;
	std::mutex mut;
};


#endif /* __SOCKET_CLIENT_H__ */
