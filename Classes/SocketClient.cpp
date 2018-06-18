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
#define MAX_LENGTH 8000

boost::asio::io_service* SocketClient::_io = new boost::asio::io_service;

SocketClient::~SocketClient(){
    stopClient();
}

SocketClient* SocketClient::create(std::string server_ip, int port_nunber){
    try{
        auto new_client = new SocketClient(server_ip, port_nunber);
		if(new_client){
            new_client->startClient();	
        }
        return new_client;
    }
    catch(...){
        std::cerr << "Failed to Create Client" << std::endl;
        return static_cast<SocketClient*>(nullptr);
    }
}

void SocketClient::startClient(){
    auto new_thread = new std::thread(std::bind(static_cast<std::size_t(boost::asio::io_service::*)()>(&boost::asio::io_service::run),&(*_io)));
    //new_thread->detach();
    _exchange_thread.reset(new_thread);
    _exchange_thread->detach();
    startConnect();
}

bool SocketClient::checkStop(){
    if(_error_flag || _cancel_flag || _stop_flag){
        stopClient();
        return true;
    }
    else{
        return false;
    }
}

void SocketClient::stopClient(){
    if(!_stop_flag){
        try{
            std::unique_lock<std::mutex> lock(_mutex);
            _stop_flag = true;
            if(_error_flag){
				cocos2d::log("Client Error!\n");
                std::cerr << "Client Error!" << std::endl;
                std::string error_message = "Error";
                _message_set_deque.push_back(error_message);
            }
            else if(_cancel_flag){
				cocos2d::log("Client Successfully Cancelled!\n");
                std::cout << "Client Successfully Cancelled" << std::endl;
                std::string cancel_message = "Cancelled";
                _message_set_deque.push_back(cancel_message);
            }
            else{
                std::cerr << "Unknown Errors" << std::endl;
                std::string error_message = "Error";
                _message_set_deque.push_back(error_message);
            }
            //_exchange_thread will exit after work destruct
            _io->stop();
            _cond.notify_one();
            _socket.close();
            if(_read_thread){
                _read_thread->join();
            }
            _socket.shutdown(tcp::socket::shutdown_both);
        }
        catch(...){
            std::cerr << "Client Shutdown Error!" << std::endl;
        }
    }
}

std::string SocketClient::read_data_once(){
    std::unique_lock<std::mutex> lk(_mutex);
	while(_message_set_deque.empty()) {
        _cond.wait(lk);
    }
    auto new_message = _message_set_deque.front();
    _message_set_deque.pop_front();
	lk.unlock();
    return new_message;
}

void SocketClient::readMessages(){
    if(!checkStop()){
        boost::asio::async_read(_socket, boost::asio::buffer(&_message_set_buffer, MAX_LENGTH), std::bind(&SocketClient::pushMessageSet, this, std::placeholders::_1));
    }
}

void SocketClient::pushMessageSet(const error_code &err){
    if(!err){
        _message_set_deque.push_back(_message_set_buffer);
        readMessages();
    }
    else{
        std::unique_lock<std::mutex> lock(_mutex);
        _error_flag = true;
    }
}

void SocketClient::connectHandle(const error_code &err){
    if(!err){
		cocos2d::log("Client Connected!\n");
        std::cout << "Client Connected" << std::endl;
        auto new_thread = new std::thread(std::bind(&SocketClient::readMessages, this));
        _read_thread.reset(new_thread);
    }
    else{
		cocos2d::log("Failed to Connected!\n");
        std::cerr << "Failed to Connect" << std::endl;
        std::unique_lock<std::mutex> lock(_mutex);
        _error_flag = true;
        lock.unlock();
        checkStop();
    }
}

//if successfully called the async_write, then return true. Not to say transmitting would be successful.
bool SocketClient::writeMessages(std::string message_set){
    if(!checkStop()){
        static int times = 0;
        static int error_times = 0;
        boost::asio::async_write(_socket, boost::asio::buffer(message_set), [](const error_code &err, std::size_t bytes_transferred){
            if(!err){
                times ++;
                std::cout << times << ".: Successfully Upload Messages, bytes transferred: " << bytes_transferred << std::endl;
            }
            else{
                error_times ++;
                std::cout << error_times << ".: Error Upload Messages, bytes transferred: " << bytes_transferred << "Missing Bytes: Unknown" << std::endl;
            }
        });
        return true;
    }
    else{
        return false;
    }
}

void SocketClient::startConnect(){
	cocos2d::log("start Connection!\n");
    std::cout << "Client Start Connecting" << std::endl;
    //std::cout << _endpoint.port() << _endpoint.address() << std::endl;
    _socket.async_connect(_endpoint, std::bind(&SocketClient::connectHandle, this, std::placeholders::_1));
}
