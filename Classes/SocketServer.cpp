//
//  SocketServer.cpp
//  BattleField_RTS
//
//  Created by Khazeus on 2018/6/10.
//

#include "SocketServer.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include "cocos2d.h"

std::shared_ptr<boost::asio::io_service> SocketServer::_io_service;


TcpConnection::pointer TcpConnection::create(boost::asio::io_service& io_service, SocketServer* parent)
{
	return pointer(new TcpConnection(io_service, parent));
}

tcp::socket& TcpConnection::socket()
{
	return _socket;
}

void TcpConnection::start()
{
	_parent->insertConnection(shared_from_this());

	boost::asio::async_read(_socket,
		boost::asio::buffer(_readMsg.data(), SocketMessage::header_length),
		std::bind(&TcpConnection::handle_read_header, this,
			std::placeholders::_1));
}

void TcpConnection::writeData(std::string s)
{
	if (_errorFlag) {
		return;
	}
	SocketMessage msg;
	if (s.size() == 0){
		s = std::string("\0");
		msg.body_length(1);
	}
	else {
		msg.body_length(s.size());
	}
	memcpy(msg.body(), &s[0u], msg.body_length());
	msg.encode_header();
	boost::asio::write(_socket,
		boost::asio::buffer(msg.data(), msg.length()));
}

std::string TcpConnection::readData()
{
	if (_errorFlag) {
		return "";
	}
	std::unique_lock<std::mutex> lk{ _mut };
	while (_read_Msg_Deque_.empty()) {
		if (_errorFlag) {
			return "";
		}
		_cond.wait(lk);
	}
	auto read_msg = _read_Msg_Deque_.front();
	_read_Msg_Deque_.pop_front();
	lk.unlock();
	auto str = std::string(read_msg.body(), read_msg.body_length());
	return str;
}

void TcpConnection::doClose()
{
	try {
		std::unique_lock<std::mutex> lk{ _mut };
		_errorFlag = true;
		SocketMessage empty_msg;
		memcpy(empty_msg.data(), "0001\0", 5);
		_read_Msg_Deque_.push_back(empty_msg);
		_read_Msg_Deque_.push_back(empty_msg);
		error_code ec;
		_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
		_socket.close();
		_cond.notify_one();
		lk.unlock();
		cocos2d::log("111");
		cocos2d::log("Socket closed\n");
	}
	catch (boost::system::system_error) {
		cocos2d::log("Socket closed!");
        std::cerr << "Server ShutDowned with Process Left" << std::endl;
    }
	catch (std::exception&e)
	{
		e.what();
	}
	deleteFrom();
}

void TcpConnection::handle_read_header(const error_code& error)
{
	if (!error && _readMsg.decode_header())
	{
		std::cout << "here\n";
		boost::asio::async_read(_socket,
			boost::asio::buffer(_readMsg.body(), _readMsg.body_length()),
			std::bind(&TcpConnection::handle_read_body, this,
				std::placeholders::_1));
	}
	else if(_errorFlag == true)
	{
		return;
	}
	else {
		doClose();
	}
}

void TcpConnection::handle_read_body(const error_code& error)
{
	
	if (_errorFlag) {
		return;
	}
	else if (!error)
	{
		std::lock_guard<std::mutex> lk{ _mut };
		_read_Msg_Deque_.push_back(_readMsg);
		_cond.notify_one();
		boost::asio::async_read(_socket,
			boost::asio::buffer(_readMsg.data(), SocketMessage::header_length),
			std::bind(&TcpConnection::handle_read_header, this,
				std::placeholders::_1));
	}
	else {
		doClose();
	}
}

TcpConnection::TcpConnection(boost::asio::io_service& io_service, SocketServer* parent) :
	_socket(io_service), _parent(parent)
{
	std::cout << "new tcp" << std::endl;
}



void TcpConnection::deleteFrom()
{
	if (_parent) {
		shared_from_this()->_parent->removeConnection(shared_from_this());
	}
	_parent = nullptr;
}

SocketServer::~SocketServer(){
   // this->close();
}

SocketServer* SocketServer::create(int port)
{
	_io_service.reset(new boost::asio::io_service);
	auto s = new SocketServer(port);

	s->_thread = new std::thread(
		std::bind(static_cast<std::size_t(boost::asio::io_service::*)()>(&boost::asio::io_service::run),
			_io_service));
	s->_thread->detach();
	return s;
}

void SocketServer::close()
{
	if (_close) {
		return;
	}
	try {
	//	_connection_Vector.clear();
		_acceptor.close();
		_io_service->stop();
		stop = true;
		_io_service.reset(new boost::asio::io_service);
	//	_thread->join();
	//	_loopthread->join();
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		TerminateThread(_thread, 0);
		TerminateThread(_loopthread, 0);
        _io_service.reset(new boost::asio::io_service);
		_close = true;
	}
	catch (std::exception&e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void SocketServer::clickStart()
{
	_acceptor.close();
	using namespace std; 

	std::vector<std::string> campGroup = { "R","B" };
	char map;
	if (getMapselect() == LOSTTEMP) {
		map = 'L';
	}
	else if (getMapselect() == SNOWMAP) {
		map = 'S';
	}

	for (auto i = 0; i < _connection_Vector.size(); i++)
	{
		_connection_Vector[i]->writeData("PLAYER" + campGroup.at(i) + map);
	}
	connection_num_ = _connection_Vector.size();
	cocos2d::log("ConnectionSize %d\n", connection_num_);
	this->_loopthread = new std::thread(std::bind(&SocketServer::loop, this));
	this->_loopthread->detach();
}

bool SocketServer::isError() const
{
	return _error;
}

SocketServer::SocketServer(int port) :
	_acceptor(*_io_service, tcp::endpoint(tcp::v4(), port))
{
	startAccept();
}

void SocketServer::loop()
{
	while (!stop && !_error)
	{
        static int times = 0;
        try{
            if(stop){
                break;
            }
            if (_connection_Vector.size() != connection_num_)
            {
                _error = true;
                break;
            }
            std::unique_lock<std::mutex> lock(_mutex);
            std::vector<std::string> ret;
            for (auto r : _connection_Vector){
				if (r->error()) {
					break;
				}
                ret.push_back(r->readData());
            }
            auto game_msg = GameMessageOperation::combineMessage(ret);
            
			for (auto r : _connection_Vector) {
				r->writeData(game_msg);
			}
        }
        catch(std::exception &e){
            times++;
            std::cerr << "Connection Lost No. " << times << std::endl;
        }
	}
}

std::vector<TcpConnection::pointer> SocketServer::getConnection() const
{
	return _connection_Vector;
}


void SocketServer::insertConnection(TcpConnection::pointer p) {
	_connection_Vector.push_back(p);
}

void SocketServer::removeConnection(TcpConnection::pointer p)
{
	//		_connection_Vector.erase(std::remove(_connection_Vector.begin(), _connection_Vector.end(), p), _connection_Vector.end());
	std::unique_lock<std::mutex> lock(_mutex);
	auto position = std::find(_connection_Vector.begin(), _connection_Vector.end(), p);

	if (position == _connection_Vector.end()) {
		return;
	}
	else {
		_connection_Vector.erase(position);
	}
}


void SocketServer::startAccept()
{
	TcpConnection::pointer new_connection =
		TcpConnection::create(_acceptor.get_io_service(), this);

	_acceptor.async_accept(new_connection->socket(),
		std::bind(&SocketServer::handleAccept, this, new_connection,
			std::placeholders::_1));
	std::cout << "start accept " << std::endl;
}

void SocketServer::handleAccept(TcpConnection::pointer new_connection, const error_code& error)
{
	std::cout << "handle_accept\n";
	if (!error)
	{
		cocos2d::log("connection + 1");
		new_connection->start();
	}
	startAccept();
}



