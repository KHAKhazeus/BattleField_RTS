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


//create a Tcp class to handle the connect between clint and server
class SocketServer;
class TcpConnection
	: public std::enable_shared_from_this<TcpConnection>
{
public:
	typedef std::shared_ptr<TcpConnection> pointer;
	//	~TcpConnection();
	//create a Point of TcpConnection
	static pointer create(boost::asio::io_service& io_service, SocketServer* parent);
	tcp::socket& socket();

	void start();

	void writeData(std::string s);

	std::string readData();
	bool error()const { return _errorFlag; }

	void doClose();
private:

	void handle_read_header(const error_code& error);
	void handle_read_body(const error_code& error);

	TcpConnection(boost::asio::io_service& io_service, SocketServer* parent);;

	//delete it from the server
	void deleteFrom();
	
	tcp::socket _socket;
	//pointer to the server
	SocketServer* _parent;
	bool _errorFlag{ false };

	SocketMessage _readMsg;
	std::deque<SocketMessage> _read_Msg_Deque_;
	std::condition_variable _cond;
	std::mutex _mut;

};

class SocketServer
{
public:
    ~SocketServer();
	static SocketServer* create(int port = 8080);

	//Close Operation
	void close();
	
	//The return value is a vector of Tcp Pointer
	std::vector<TcpConnection::pointer> getConnection() const;

	//set or get Map Operation
	void setMapselect(int mapID) { _mapselect = mapID; }
	int getMapselect() { return _mapselect; }
	/**
	* \brief remove a connction, if there is a connction
	* \param p tcp connection
	*/
	void removeConnection(TcpConnection::pointer p);

	void insertConnection(TcpConnection::pointer p);
	/**
	* \brief start the game
	*/
	void clickStart();

	/**
	* \brief
	* \return if isError occured
	*/
	bool isError() const;

	/**
	* \brief
	* \return total connction number
	*/
	int connection_num() const;
	bool _close{ false };
    bool stop{false};
	
private:
	SocketServer(int port);
	void startAccept();

	void handleAccept(TcpConnection::pointer new_connection,
		const error_code& error);

	void loop();


	tcp::acceptor _acceptor;
	
	int connection_num_;

	static std::shared_ptr<boost::asio::io_service> _io_service;
	std::vector<TcpConnection::pointer> _connection_Vector;
	std::thread* _thread;
	std::thread* _loopthread{ nullptr };
	std::mutex _mutex;
	bool _error{ false };
	int _mapselect { LOSTTEMP };
	std::condition_variable _cond;
};


#endif /* __SOCKET_SERVER_H__ */
