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
	s->_thread = new std::thread(
		std::bind(static_cast<std::size_t(boost::asio::io_service::*)()>(&boost::asio::io_service::run),
			&s->_io_service));
	s->_thread->detach();
	return s;
}


void SocketClient::doClose()
{
	try {
		std::lock_guard<std::mutex> lk{ _mut };
		_error_Flag = true;
		SocketMessage empty_msg;
		memcpy(empty_msg.data(), "0001\0", 5);
		_read_Msg_Deque.push_back(empty_msg);
		_data_cond.notify_one();
		error_code ec;
		_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
		_socket.close();
		_io_service.stop();
	//	_thread->join();
		delete _thread;


		
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
	}
	catch (std::exception&e)
	{
		e.what();
	}

}


void SocketClient::startConnect()
{
	_socket.async_connect(_endpoint,
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
			size_t length = _socket.read_some(boost::asio::buffer(data, 30), error);
			if (error || length < 10) {
				cocos2d::log("Empty Message\n");
				//throw boost::system::error_code(error);
			}
			char header[4 + 1] = "";
			strncat(header, data + 10, 4);
			if (data[10] == 'R') {
				_camp = REDCAMP;
			}
			else if (data[10] == 'B') {
				_camp = BLUECAMP;
			}
			if (data[11] == 'L') {
				setMapselect(LOSTTEMP);

			}
			else if (data[11] == 'S') {
				setMapselect(SNOWMAP);
			}


			//total_ = atoi(header);
			//_camp = atoi(data + 14);
			cocos2d::log("GettheCamp %d", _camp);
			_start_Flag = true;
			boost::asio::async_read(_socket,
				boost::asio::buffer(_read_msg.data(), SocketMessage::header_length),
				std::bind(&SocketClient::handle_read_header, this,
					std::placeholders::_1));

		}
		else
		{
			std::cerr << "failed to connect, Please Retry" << std::endl;
			//			throw asio::system_error(error);
			_error_Flag = true;

		}
	}
	catch (std::exception& e)
	{
		//		std::terminate();
		//		doClose();
		std::cerr << "Exception in connection: " << e.what() << "\n";
	}
}

void SocketClient::handle_read_header(const error_code& error)
{
	if (!error && _read_msg.decode_header())
	{
		boost::asio::async_read(_socket,
			boost::asio::buffer(_read_msg.body(), _read_msg.body_length()),
			std::bind(&SocketClient::handle_read_body, this,
				std::placeholders::_1));
	}
	else
	{
		doClose();
	}
}

void SocketClient::handle_read_body(const error_code& error)
{
	if (!error)
	{
		std::lock_guard<std::mutex> lk{ _mut };
		_read_Msg_Deque.push_back(_read_msg);
		_data_cond.notify_one();
		std::cout << "read completed\n";

		boost::asio::async_read(_socket,
			boost::asio::buffer(_read_msg.data(), SocketMessage::header_length),
			std::bind(&SocketClient::handle_read_header, this,
				std::placeholders::_1));
	}
	else
	{
		doClose();
	}
}

std::vector<GameMessage> SocketClient::getGameMessages()
{
	auto game_message_set_stirng = read_data();
	return GameMessageOperation::stringToVector(game_message_set_stirng);
}

std::string SocketClient::read_data()
{
	if (_error_Flag)
		return "";
	std::unique_lock<std::mutex> lk{ _mut };
	while (_read_Msg_Deque.empty())
		_data_cond.wait(lk);
	auto read_msg = _read_Msg_Deque.front();
	_read_Msg_Deque.pop_front();
	lk.unlock();
	auto ret = std::string(read_msg.body(), read_msg.body_length());
	return ret;
}

/*-------------------------------------------------------------------*/

void SocketClient::sendGameMessages(const std::vector<GameMessage>& vec_game_msg)
{
	auto set_string = GameMessageOperation::vectorToString(vec_game_msg);
	write_data(set_string);
}

void SocketClient::send_string(std::string s)
{
	if (_error_Flag)
		return;
	write_data(s);
}

/*std::string SocketClient::get_string()
{
	return read_data();
}*/

int SocketClient::camp() const
{
	//while (!_start_Flag);
	return _camp;
}
/*
int SocketClient::total() const
{
	while (!_start_Flag);
}*/

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
		else {
			msg.body_length(s.size());
		}
		std::copy(s.cbegin(),s.cend(),msg.body());
    //    memcpy(msg.body(), &s[0u], msg.body_length());
        msg.encode_header();
        boost::asio::write(_socket,
                           boost::asio::buffer(msg.data(), msg.length()));
    }
    catch(boost::system::system_error){
        times++;
        std::cerr << "Lost Connection, No. " << times << std::endl;
    }
}

void SocketClient::close()
{
	doClose();
}

