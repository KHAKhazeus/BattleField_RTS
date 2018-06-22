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

std::shared_ptr<boost::asio::io_service> SocketServer::io_service_;

//TcpConnection::~TcpConnection()
//{
//	std::cout << "delete";
//	delete_from_parent();
//}

TcpConnection::pointer TcpConnection::create(boost::asio::io_service& io_service, SocketServer* parent)
{
	return pointer(new TcpConnection(io_service, parent));
}

tcp::socket& TcpConnection::socket()
{
	return socket_;
}

void TcpConnection::start()
{
	//	
	//	_timer_.async_wait(std::bind(&TcpConnection::check_timer, this));
	//	steady_timer_.expires_from_now(std::chrono::seconds(60));
	boost::asio::async_read(socket_,
		boost::asio::buffer(read_msg_.data(), SocketMessage::header_length),
		std::bind(&TcpConnection::handle_read_header, this,
			std::placeholders::_1));
}

void TcpConnection::write_data(std::string s)
{
	if (error_flag_) return;
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
	boost::asio::write(socket_,
		boost::asio::buffer(msg.data(), msg.length()));
}

std::string TcpConnection::read_data()
{
	if (error_flag_)
		return "";
	std::unique_lock<std::mutex> lk{ mut_ };
	while (read_msg_deque_.empty())
		data_cond_.wait(lk);
	auto read_msg = read_msg_deque_.front();
	read_msg_deque_.pop_front();
	lk.unlock();
	auto ret = std::string(read_msg.body(), read_msg.body_length());
	return ret;
}

void TcpConnection::do_close()
{
	try {
		//		steady_timer_.cancel();
		std::unique_lock<std::mutex> lk{ mut_ };
		error_flag_ = true;
		SocketMessage empty_msg;
		memcpy(empty_msg.data(), "0001\0", 5);
		read_msg_deque_.push_back(empty_msg);
		read_msg_deque_.push_back(empty_msg);
		data_cond_.notify_one();
		lk.unlock();
		error_code ec;
		socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
		if (!ec) {
			cocos2d::log("111");
		//	return;
		}
		/*try*/ 
			cocos2d::log("Socket closed\n");
			socket_.close();
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
	delete_from_parent();
}

void TcpConnection::handle_read_header(const error_code& error)
{
	if (!error && read_msg_.decode_header())
	{
		//		steady_timer_.expires_from_now(std::chrono::seconds(10));
		std::cout << "here\n";
		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
			std::bind(&TcpConnection::handle_read_body, this,
				std::placeholders::_1));
	}
	else if(error_flag_ == true)
	{
		return;
	}
	else {
		do_close();
	}
}

void TcpConnection::handle_read_body(const error_code& error)
{
	if (error_flag_) {
		true;
	}
	else if (!error)
	{
		//		steady_timer_.expires_from_now(std::chrono::seconds(10));
		std::lock_guard<std::mutex> lk{ mut_ };
		read_msg_deque_.push_back(read_msg_);
		data_cond_.notify_one();
		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.data(), SocketMessage::header_length),
			std::bind(&TcpConnection::handle_read_header, this,
				std::placeholders::_1));
	}
	else {
		do_close();
	}
}

TcpConnection::TcpConnection(boost::asio::io_service& io_service, SocketServer* parent) :
	socket_(io_service), parent(parent)
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


void TcpConnection::delete_from_parent()
{
	if (parent)
		shared_from_this()->parent->remove_connection(shared_from_this());
	parent = nullptr;
}

SocketServer::~SocketServer(){
    this->close();
}

SocketServer* SocketServer::create(int port)
{
	//	_io_service = new asio::io_service;
	io_service_.reset(new boost::asio::io_service);
	auto s = new SocketServer(port);

	s->thread_.reset(new std::thread(
		std::bind(static_cast<std::size_t(boost::asio::io_service::*)()>(&boost::asio::io_service::run),
			io_service_)));
	s->thread_->detach();
	return s;
}

void SocketServer::close()
{
	try {
		connections_.clear();
		io_service_->stop();
		acceptor_.close();
		//		_thread = nullptr;
		stop = true;
        io_service_.reset(new boost::asio::io_service);
	}
	catch (std::exception&e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void SocketServer::button_start()
{
	acceptor_.close();
	using namespace std; // For sprintf and memcpy.
	char total[4 + 1] = "";
	sprintf(total, "%4d", static_cast<int>(connections_.size()));

	std::vector<std::string> campGroup = { "R","B" };
	char map;
	if (getMapselect() == LOSTTEMP) {
		map = 'L';
	}
	else if (getMapselect() == SNOWMAP) {
		map = 'S';
	}


	for (auto i = 0; i < connections_.size(); i++)
	{
		connections_[i]->write_data("PLAYER" + campGroup.at(i) + map);
	}
	connection_num_ = connections_.size();
	cocos2d::log("ConnectionSize %d\n", connection_num_);
	this->button_thread_.reset(new std::thread(std::bind(&SocketServer::loop_process, this)));
	button_thread_->detach();
}

bool SocketServer::error() const
{
	return error_flag_;
}

int SocketServer::connection_num() const
{
	return connections_.size();
}

SocketServer::SocketServer(int port) :
	acceptor_(*io_service_, tcp::endpoint(tcp::v4(), port))
{
	start_accept();
}

void SocketServer::loop_process()
{
	while (true)
	{
        static int times = 0;
        try{
            if(stop){
                break;
            }
            if (connections_.size() != connection_num_)
            {
                error_flag_ = true;
                break;
            }
            //            throw std::exception{"lost connection"};
            std::unique_lock<std::mutex> lock(delete_mutex_);
            std::vector<std::string> ret;
            for (auto r : connections_)
            {
                if (r->error())
                    //                break;
                    error_flag_ |= r->error();
                ret.push_back(r->read_data());
            }
            auto game_msg = GameMessageOperation::combineMessage(ret);
            
            for (auto r : connections_)
                r->write_data(game_msg);
        }
        catch(std::exception &e){
            times++;
            std::cerr << "Connection Lost No. " << times << std::endl;
        }
	}
}

std::vector<TcpConnection::pointer> SocketServer::get_connection() const
{
	return connections_;
}

void SocketServer::remove_connection(TcpConnection::pointer p)
{
	//		connections_.erase(std::remove(connections_.begin(), connections_.end(), p), connections_.end());
	std::unique_lock<std::mutex> lock(delete_mutex_);
	auto position = std::find(connections_.begin(), connections_.end(), p);

	if (position == connections_.end())
		std::cout << "delete not succ\n";
	else
		connections_.erase(position);
	std::cout << "delete succ\n";
}


void SocketServer::start_accept()
{
	TcpConnection::pointer new_connection =
		TcpConnection::create(acceptor_.get_io_service(), this);

	acceptor_.async_accept(new_connection->socket(),
		std::bind(&SocketServer::handle_accept, this, new_connection,
			std::placeholders::_1));
	std::cout << "start accept " << std::endl;
}

void SocketServer::handle_accept(TcpConnection::pointer new_connection, const error_code& error)
{
	std::cout << "handle_accept\n";
	if (!error)
	{
		cocos2d::log("connection + 1");
		connections_.push_back(new_connection);
		std::cout << new_connection->socket().remote_endpoint().address()
			<< ":" << new_connection->socket().remote_endpoint().port() << std::endl;
		new_connection->start();
	}
	start_accept();
	//			std::cout << "handle accept\n";
}



