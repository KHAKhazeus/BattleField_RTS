//
//  SocketClient.cpp
//  BattleField_RTS
//
//  Created by Khazeus on 2018/6/10.
//

#include "SocketClient.h"
#include <functional>
#include <iostream>
#define MAX_LENGTH 8000


SocketClient* SocketClient::create(std::string server_ip, int port_nunber){
    try{
        auto new_client = new SocketClient(server_ip, port_nunber);
        new_client->startClient();
        return new_client;
    }
    catch(...){
        std::cerr << "Failed to Create Client" << std::endl;
    }
}

void SocketClient::startClient(){
    //run() returns count_type which is a std::size_t
    auto new_thread = new std::thread(std::bind(static_cast<std::size_t(boost::asio::io_service::*)()>(&boost::asio::io_service::run),
                                                &_io));
    _exchange_thread.reset(new_thread);
    startConnect();
}

void SocketClient::checkStop(){
    if(_error_flag || _cancel_flag){
        stopClient();
    }
}

void SocketClient::stopClient(){
    try{
        std::unique_lock<std::mutex> lock(_mutex);
        _stop_flag = true;
        _io.stop();
        auto empty_message = "";
        _message_set_deque.push_back(empty_message);
        _cond.notify_one();
        _socket.shutdown(tcp::socket::shutdown_both);
        _socket.close();
        _exchange_thread->join();
        _read_thread->join();
        if(_error_flag){
            std::cerr << "Client Error!" << std::endl;
            std::string error_message = "Error";
            _message_set_deque.push_back(error_message);
        }
        else if(_cancel_flag){
            std::cout << "Client Successfully Cancelled" << std::endl;
            std::string cancel_message = "Cancelled";
            _message_set_deque.push_back(cancel_message);
        }
    }
    catch(...){
        std::cerr << "Client Shutdown Error!" << std::endl;
    }
}

void SocketClient::readMessages(){
    boost::asio::async_read(_socket, boost::asio::buffer(&_message_set_buffer, MAX_LENGTH), std::bind(&SocketClient::pushMessageSet, this, std::placeholders::_1));
}

void SocketClient::pushMessageSet(const error_code &err){
    if(!err){
        _message_set_deque.push_back(_message_set_buffer);
        readMessages();
    }
    else{
        std::unique_lock<std::mutex> lock(_mutex);
        _error_flag = true;
        stopClient();
    }
}

void SocketClient::connectHandle(const error_code &err){
    if(!err){
        std::cout << "Client Connected" << std::endl;
        auto new_thread = new std::thread(std::bind(&SocketClient::readMessages, this));
        _read_thread.reset(new_thread);
    }
    else{
        std::cerr << "Failed to Connect" << std::endl;
        checkStop();
        std::unique_lock<std::mutex> lock(_mutex);
        _error_flag = true;
    }
}

void SocketClient::writeMessages(std::string message_set){
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
}

void SocketClient::startConnect(){
    std::cout << "Client Start Connecting" << std::endl;
    _socket.async_connect(_endpoint, std::bind(&SocketClient::connectHandle, this, std::placeholders::_1));
}