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

//TcpConnection::~TcpConnection()
//{
//    std::cout << "delete";
//    deleteFrom();
//}

TcpConnection* TcpConnection::create(boost::asio::io_service& io_service, SocketServer* parent)
{
    auto connection = new TcpConnection(io_service, parent);
	return connection;
}

tcp::socket& TcpConnection::socket()
{
	return _socket;
}

void TcpConnection::start()
{
	//	
	//	_timer_.async_wait(std::bind(&TcpConnection::check_timer, this));
	//	steady_timer_.expires_from_now(std::chrono::seconds(60));
	boost::asio::async_read(_socket,
		boost::asio::buffer(_readMsg.data(), SocketMessage::header_length),
		std::bind(&TcpConnection::handle_read_header, this,
			std::placeholders::_1));
}

void TcpConnection::writeData(std::string s)
{
	if (_errorFlag) return;
	SocketMessage msg;
	if (s.size() == 0)
	{
		s = std::string("\0");
		msg.body_length(1);
	}
	else
		msg.body_length(s.size());
	memcpy(msg.body(), &s[0u], msg.body_length());
	msg.encode_header();
	boost::asio::write(_socket,
		boost::asio::buffer(msg.data(), msg.length()));
}

std::string TcpConnection::readData()
{
	if (_errorFlag)
		return "";
	std::unique_lock<std::mutex> lk{ _mut };
	while (_read_Msg_Deque_.empty())
		_cond.wait(lk);
	auto read_msg = _read_Msg_Deque_.front();
	_read_Msg_Deque_.pop_front();
	lk.unlock();
	auto ret = std::string(read_msg.body(), read_msg.body_length());
	return ret;
}

void TcpConnection::do_close()
{
	try {
		//		steady_timer_.cancel();
		std::unique_lock<std::mutex> lk{ _mut };
		_errorFlag = true;
		SocketMessage empty_msg;
		memcpy(empty_msg.data(), "0001\0", 5);
		_read_Msg_Deque_.push_back(empty_msg);
		_read_Msg_Deque_.push_back(empty_msg);
		_cond.notify_one();
		lk.unlock();
		error_code ec;
		_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
		cocos2d::log("111");
		return;
		/*try*/ 
		cocos2d::log("Socket closed\n");
		_socket.close();
		/*catch (boost::system::error_code &ec) {
			cocos2d::log("Socket closed!");
			std::cerr << "Socket closed!";
		}*/
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
		//		steady_timer_.expires_from_now(std::chrono::seconds(10));
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
		do_close();
	}
}

void TcpConnection::handle_read_body(const error_code& error)
{
	if (_errorFlag) {
		return true;
	}
	else if (!error)
	{
		//		steady_timer_.expires_from_now(std::chrono::seconds(10));
		std::lock_guard<std::mutex> lk{ _mut };
		_read_Msg_Deque_.push_back(_readMsg);
		_cond.notify_one();
		boost::asio::async_read(_socket,
			boost::asio::buffer(_readMsg.data(), SocketMessage::header_length),
			std::bind(&TcpConnection::handle_read_header, this,
				std::placeholders::_1));
	}
	else {
		do_close();
	}
}

TcpConnection::TcpConnection(boost::asio::io_service& io_service, SocketServer* parent) :
	_socket(io_service), _parent(parent)
	//,steady_timer_(io_service)
{
	std::cout << "new tcp" << std::endl;
}

//void TcpConnection::check_timer()
//{
//	if (steady_timer_.expires_at() <= std::chrono::steady_clock::now())
//	{
//		// The deadline has passed. The socket is closed so that any outstanding
//		// asynchronous operations are cancelled.
//		doClose();
//		steady_timer_.expires_at(std::chrono::steady_clock::time_point::max());
//		return;
//	}
//
//	// Put the actor back to sleep.
//	steady_timer_.async_wait(std::bind(&TcpConnection::check_timer, this));
//}


void TcpConnection::deleteFrom()
{
	if (_parent)
        _parent->removeConnection(this);
	_parent = nullptr;
}

SocketServer* SocketServer::create(int port)
{
	//	_io_service = new asio::io_service;
	_io_service.reset(new boost::asio::io_service);
	auto s = new SocketServer(port);

	s->_thread = new std::thread(
		std::bind(static_cast<std::size_t(boost::asio::io_service::*)()>(&boost::asio::io_service::run),
			_io_service));
	//s->_thread->detach();
	return s;
}

void SocketServer::stopAccept(){
    try{
        _acceptor.close();
    }
    catch(...){
        std::cerr << "Acceptor Out" << std::endl;
    }
}

void SocketServer::close()
{
	try {
		_connection_Vector.clear();
		_io_service->stop();
		//_acceptor.close();
		//		_thread = nullptr;
		stop = true;
        _io_service.reset(new boost::asio::io_service);
        if(_thread){
            _thread->join();
        }
        if(_loopthread){
            _loopthread->join();
        }
        delete _thread;
        delete _loopthread;
        delete this;
	}
	catch (std::exception&e)
	{
		std::cerr << e.what() << std::endl;
        delete this;
	}
}

void SocketServer::clickStart()
{
	_acceptor.close();
	using namespace std; // For sprintf and memcpy.
	char total[4 + 1] = "";
	sprintf(total, "%4d", static_cast<int>(_connection_Vector.size()));

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
	//_loopthread->detach();
}

bool SocketServer::isError() const
{
	return _error;
}

int SocketServer::connection_num() const
{
	return _connection_Vector.size();
}

SocketServer::SocketServer(int port) :
	_acceptor(*_io_service, tcp::endpoint(tcp::v4(), port))
{
	startAccept();
}

void SocketServer::loop()
{
	while (true)
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
            //            throw std::exception{"lost connection"};
            std::unique_lock<std::mutex> lock(_mutex);
            std::vector<std::string> ret;
            for (auto r : _connection_Vector)
            {
                if (r->error())
                    //                break;
                    _error |= r->error();
                ret.push_back(r->readData());
            }
            auto game_msg = GameMessageOperation::combineMessage(ret);
            
            for (auto r : _connection_Vector)
                r->writeData(game_msg);
            lock.unlock();
        }
        catch(std::exception &e){
            times++;
            std::cerr << "Connection Lost No. " << times << std::endl;
        }
	}
}

//std::vector<TcpConnection::pointer>& SocketServer::getConnection() const
//{
//    return _connection_Vector;
//}

void SocketServer::removeConnection(TcpConnection* p)
{
	//		_connection_Vector.erase(std::remove(_connection_Vector.begin(), _connection_Vector.end(), p), _connection_Vector.end());
	std::unique_lock<std::mutex> lock(_mutex);
	auto position = std::find(_connection_Vector.begin(), _connection_Vector.end(), p);

	if (position == _connection_Vector.end())
		std::cout << "delete not succ\n";
    else{
        delete *position;
        _connection_Vector.erase(position);
    }
	std::cout << "delete succ\n";
    lock.unlock();
}


void SocketServer::startAccept()
{
	auto new_connection =
		TcpConnection::create(_acceptor.get_io_service(), this);
	_acceptor.async_accept(new_connection->socket(),
		std::bind(&SocketServer::handleAccept, this, new_connection,
			std::placeholders::_1));
	std::cout << "start accept " << std::endl;
}

void SocketServer::handleAccept(TcpConnection* new_connection, const error_code& error)
{
	std::cout << "handle_accept\n";
	if (!error)
	{
		cocos2d::log("connection + 1");
		_connection_Vector.push_back(new_connection);
		std::cout << new_connection->socket().remote_endpoint().address()
			<< ":" << new_connection->socket().remote_endpoint().port() << std::endl;
		new_connection->start();
	}
    if(!stop){
        startAccept();
    }
	//			std::cout << "handle accept\n";
}



