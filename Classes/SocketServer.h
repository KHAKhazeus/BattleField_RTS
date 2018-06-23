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
{
public:
	//	~TcpConnection();
	static TcpConnection* create(boost::asio::io_service& io_service, SocketServer* parent);
	tcp::socket& socket();

	void start();

	void writeData(std::string s);

	std::string readData();
	bool error()const { return _errorFlag; }

	void do_close();
private:

	void handle_read_header(const error_code& error);
	void handle_read_body(const error_code& error);

	TcpConnection(boost::asio::io_service& io_service, SocketServer* parent);;

	void check_timer();
	void deleteFrom();
	
	tcp::socket _socket;
	SocketServer* _parent;
	bool _errorFlag{ false };

	SocketMessage _readMsg;
	std::deque<SocketMessage> _read_Msg_Deque_;
	std::condition_variable _cond;
	std::mutex _mut;
	//	asio::steady_timer steady_timer_;

};

class SocketServer
{
public:
	static SocketServer* create(int port = 8080);
	//	~SocketServer() { _acceptor.close(); _io_service->stop(); }
	/**
	* \brief close the server
	*/
	void close();
    void stopAccept();
	/**
	* \brief
	* \return TcpConnection vector
	*/

	void setMapselect(int mapID) { _mapselect = mapID; }
	int getMapselect() { return _mapselect; }
	/**
	* \brief remove a connction, if there is a connction
	* \param p tcp connection
	*/
    void removeConnection(TcpConnection* p);
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
    
    bool stop{false};
private:
	SocketServer(int port);
	void startAccept();

	void handleAccept(TcpConnection* new_connection,
		const error_code& error);

	void loop();


	tcp::acceptor _acceptor;
	std::vector<TcpConnection*> _connection_Vector;
	int connection_num_;

	static std::shared_ptr<boost::asio::io_service> _io_service;

	std::thread* _thread, *_loopthread{ nullptr };
	std::mutex _mutex;
	bool _error{ false };
	int _mapselect { LOSTTEMP};
	std::condition_variable _cond;
};


#endif /* __SOCKET_SERVER_H__ */
