//
//  SocketServer.h
//  BattleField_RTS
//
//  Created by Khazeus on 2018/6/10.
//

#ifndef __SOCKET_SERVER_H__
#define __SOCKET_SERVER_H__
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <list>
#include "BattleField_RTS.h"
#include "SocketMessage.h"
#include "GameMessageOperation.h"
#include "include/boost/asio.hpp"
#include "GameMessageInterface.h"
using boost::asio::ip::tcp;
using boost::system::error_code;

typedef std::shared_ptr<tcp::socket> socket_ptr;

class SocketServer;
class TcpConnection
	: public std::enable_shared_from_this<TcpConnection>
{
public:
	typedef std::shared_ptr<TcpConnection> pointer;
	//	~TcpConnection();
	static pointer create(boost::asio::io_service& io_service, SocketServer* parent);
	tcp::socket& socket();

	void start();

	void write_data(std::string s);

	std::string read_data();
	bool error()const { return error_flag_; }

	void do_close();
private:

	void handle_read_header(const error_code& error);
	void handle_read_body(const error_code& error);

	TcpConnection(boost::asio::io_service& io_service, SocketServer* parent);;

	void check_timer();
	void delete_from_parent();
	
	tcp::socket socket_;
	SocketServer* parent;
	bool error_flag_{ false };

	SocketMessage read_msg_;
	std::deque<SocketMessage> read_msg_deque_;
	std::condition_variable data_cond_;
	std::mutex mut_;
	//	asio::steady_timer steady_timer_;

};

class SocketServer
{
public:
    ~SocketServer();
	static SocketServer* create(int port = 8080);
	//	~SocketServer() { acceptor_.close(); _io_service->stop(); }
	/**
	* \brief close the server
	*/
	void close();
	/**
	* \brief
	* \return TcpConnection vector
	*/
	std::vector<TcpConnection::pointer> get_connection() const;

	void setMapselect(int mapID) { _mapselect = mapID; }
	int getMapselect() { return _mapselect; }
	/**
	* \brief remove a connction, if there is a connction
	* \param p tcp connection
	*/
	void remove_connection(TcpConnection::pointer p);
	/**
	* \brief start the game
	*/
	void button_start();

	/**
	* \brief
	* \return if error occured
	*/
	bool error() const;

	/**
	* \brief
	* \return total connction number
	*/
	int connection_num() const;
    
    bool stop{false};
private:
	SocketServer(int port);
	void start_accept();

	void handle_accept(TcpConnection::pointer new_connection,
		const error_code& error);

	void loop_process();


	tcp::acceptor acceptor_;
	std::vector<TcpConnection::pointer> connections_;
	int connection_num_;

	static std::shared_ptr<boost::asio::io_service> io_service_;

	std::shared_ptr<std::thread> thread_, button_thread_{ nullptr };
	std::mutex delete_mutex_;
	bool error_flag_{ false };
	int _mapselect { LOSTTEMP };
	std::condition_variable data_cond_;
};


#endif /* __SOCKET_SERVER_H__ */
