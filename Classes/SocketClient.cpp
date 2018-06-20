//
//  SocketClient.cpp
//  BattleField_RTS
//
//  Created by Khazeus on 2018/6/10.
//

#include "SocketClient.h"
#include <functional>
#include <iostream>
#include <cocos2d.h>
#include <algorithm>
#define MAX_LENGTH 8000

SocketClient* SocketClient::create(std::string ip, int port)
{
	auto s = new SocketClient(ip, port);
	s->thread_ = new std::thread(
		std::bind(static_cast<std::size_t(boost::asio::io_service::*)()>(&boost::asio::io_service::run),
			&s->io_service_));
	//	s->thread_->detach();
	return s;
}

std::vector<GameMessage> SocketClient::get_game_messages()
{
	auto game_message_set_stirng = read_data();
	return GameMessageOperation::stringToVector(game_message_set_stirng);
}

void SocketClient::send_game_message(const std::vector<GameMessage>& vec_game_msg)
{
	auto set_string = GameMessageOperation::vectorToString(vec_game_msg);
	write_data(set_string);
}

void SocketClient::send_string(std::string s)
{
	if (error_flag_)
		return;
	write_data(s);
}

std::string SocketClient::get_string()
{
	return read_data();
}

void SocketClient::do_close()
{
	try {
		std::lock_guard<std::mutex> lk{ mut };
		error_flag_ = true;
		SocketMessage empty_msg;
		memcpy(empty_msg.data(), "0001\0", 5);
		read_msg_deque_.push_back(empty_msg);
		data_cond_.notify_one();
		io_service_.stop();
		error_code ec;
		socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
		if (!ec)
			throw boost::system::error_code(ec);
		socket_.close();
		thread_->join();


	}
	catch (std::exception&e)
	{

		e.what();
	}

}

int SocketClient::camp() const
{
	//while (!start_flag_);
	return camp_;
}

int SocketClient::total() const
{
	while (!start_flag_);
	return total_;
}

void SocketClient::write_data(std::string s)
{
    static int times = 0;
    try{
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
    catch(boost::system::system_error){
        times++;
        std::cerr << "Lost Connection, No. " << times << std::endl;
    }
}

void SocketClient::start_connect()
{
	socket_.async_connect(endpoint_,
		std::bind(&SocketClient::handle_connect, this,
			std::placeholders::_1));
}

void SocketClient::handle_connect(const error_code& error)
{
	try
	{
		if (!error)
		{

			std::cout << "connected\n";
			char data[30] = { 0 };
			error_code error;
			size_t length = socket_.read_some(boost::asio::buffer(data, 30), error);
			if (error || length < 10) {
				cocos2d::log("Empty Message\n");
				throw boost::system::error_code(error);
			}
			char header[4 + 1] = "";
			strncat(header, data + 10, 4);
			if (data[10] == 'R') {
				camp_ = REDCAMP;
			}
			else if (data[10] == 'B') {
				camp_ = BLUECAMP;
			}
			if (data[11] == 'L') {
				setMapselect(LOSTTEMP);
				
			}
			else if (data[11] == 'S') {
				setMapselect(SNOWMAP);
			}
			
			
			//total_ = atoi(header);
			//camp_ = atoi(data + 14);
			cocos2d::log("GettheCamp %d", camp_);
			start_flag_ = true;
			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.data(), SocketMessage::header_length),
				std::bind(&SocketClient::handle_read_header, this,
					std::placeholders::_1));

		}
		else
		{
			std::cerr << "failed to connect" << std::endl;
			//			throw asio::system_error(error);
			error_flag_ = true;

		}
	}
	catch (std::exception& e)
	{
		//		std::terminate();
		//		do_close();
		std::cerr << "Exception in connection: " << e.what() << "\n";
	}
}

void SocketClient::handle_read_header(const error_code& error)
{
	if (!error && read_msg_.decode_header())
	{
		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
			std::bind(&SocketClient::handle_read_body, this,
				std::placeholders::_1));
	}
	else
	{
		do_close();
	}
}

void SocketClient::handle_read_body(const error_code& error)
{
	if (!error)
	{
		std::lock_guard<std::mutex> lk{ mut };
		read_msg_deque_.push_back(read_msg_);
		data_cond_.notify_one();
		std::cout << "read completed\n";

		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.data(), SocketMessage::header_length),
			std::bind(&SocketClient::handle_read_header, this,
				std::placeholders::_1));
	}
	else
	{
		do_close();
	}
}

std::string SocketClient::read_data()
{
	if (error_flag_)
		return "";
	std::unique_lock<std::mutex> lk{ mut };
	while (read_msg_deque_.empty())
		data_cond_.wait(lk);
	auto read_msg = read_msg_deque_.front();
	read_msg_deque_.pop_front();
	lk.unlock();
	auto ret = std::string(read_msg.body(), read_msg.body_length());
	return ret;
}

void SocketClient::close()
{
	do_close();
}

