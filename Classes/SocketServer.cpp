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
#define MAX_LENGTH 8000

boost::asio::io_service* SocketServer::_io = new boost::asio::io_service;

ClientConnection::ClientConnection(boost::asio::io_service &io_service):_socket(io_service){};

std::shared_ptr<ClientConnection> ClientConnection::create(boost::asio::io_service &io_service){
    auto ptr = new ClientConnection(io_service);
    std::shared_ptr<ClientConnection> new_shared;
    new_shared.reset(ptr);
    return new_shared;
}

tcp::socket* ClientConnection::getSocket(){
    return &_socket;
}

std::string* ClientConnection::getBuffer(){
    return &_message_buffer;
}

int& ClientConnection::getSuccessTimes(){
    return _success_times;
}

int& ClientConnection::getFailureTimes(){
    return _failure_times;
}

const int& ClientConnection::getID(){
    return _ID;
}

int& ClientConnection::mutableID(){
    return _ID;
}

SocketServer::SocketServer(int port_number):_acceptor(*_io, tcp::endpoint(tcp::v4(), port_number)){
    _work = new boost::asio::io_service::work(*_io);
    (*_io).restart();
}

SocketServer::~SocketServer(){
    stopAccept();
    stopServer();
}

SocketServer* SocketServer::create(int port_number){
    try{
        port_number = 8080;
        auto new_server = new SocketServer(port_number);
        if(new_server){
            new_server->startServerListen();
        }
		cocos2d::log("Server Startup Success\n");
        std::cout << "Server Startup Success" << std::endl;
        return new_server;
    }
    catch(...){
		cocos2d::log("Server Startup Error\n");
        std::cerr << "Server Startup Error" << std::endl;
        return static_cast<SocketServer*>(nullptr);
    }
}

void SocketServer::startServerListen(){
    try{
        //!Extend
        auto new_thread = new std::thread(std::bind(static_cast<std::size_t(boost::asio::io_service::*)()>(&boost::asio::io_service::run),&(*_io)));
        new_thread->detach();
        std::shared_ptr<std::thread> new_ptr;
        new_ptr.reset(new_thread);
        _io_run_threads_vec.push_back(new_ptr);

        //_acceptor.listen();
		cocos2d::log("Server Listening\n");

        new_thread = new std::thread(std::bind(static_cast<std::size_t(boost::asio::io_service::*)()>(&boost::asio::io_service::run),&(*_io)));
        new_thread->detach();
        new_ptr.reset(new_thread);
        _io_run_threads_vec.push_back(new_ptr);
        _acceptor.listen();

        std::cout << "Server Listening" << std::endl;
        _accept_thread.reset(new std::thread(std::bind(&SocketServer::startAccept, this)));
    }
    catch(...){
		cocos2d::log("Server Acceptor Error\n");
        std::cerr << "Server Acceptor Error" << std::endl;
        _error_flag = true;
        checkStop();
    }
}

void SocketServer::startAccept(){
    if((!checkStop()) && (!_stop_connect)){
		cocos2d::log("Start Accept");
        auto new_connection = ClientConnection::create(*_io);

        _acceptor.async_accept(*(new_connection->getSocket()), std::bind(&SocketServer::handleAccept, this, new_connection, std::placeholders::_1));
    }
}

void SocketServer::stopAccept(){
    try{
        _stop_connect = true;
        _acceptor.close();
        _accept_thread->join();
    }
    catch(...){
        std::cerr << "No Acceptor" << std::endl;
    }
}

void SocketServer::handleAccept(std::shared_ptr<ClientConnection> new_connection, const error_code &err){
    static int times = 0;
    static int error_times = 0;
    if(!err){
        new_connection->mutableID() = times;
        _connections.push_back(new_connection);
        _stored_lists.push_back(std::list<std::string>());
        times++;
		cocos2d::log("Client No.%d Connected.", times);
        std::cout << "Client No."<< times << " Connected. Info: IP " << new_connection->getSocket()->remote_endpoint().address()
        << " ; Port " << new_connection->getSocket()->remote_endpoint().port() << std::endl;
        auto new_thread = new std::thread(std::bind(&SocketServer::readFromClient, this, new_connection));
        std::shared_ptr<std::thread> connection_thread(new_thread);
        _contact_threads_vector.push_back(connection_thread);
    }
    else{

		cocos2d::log("Client Connect Failed. No.\n");
		cocos2d::log("Server Handle Accept Error\n");
        error_times++;
        std::cerr << "Client Connect Failed. No." << error_times << std::endl;
        std::cerr << "Server Handle Accept Error" << std::endl;
    }
    if(_stop_connect || _stop_flag || _error_flag){
        startAccept();
    }
}

void SocketServer::readFromClient(std::shared_ptr<ClientConnection> client_connection){
    if(!checkStop()){
        boost::asio::async_read(*(client_connection->getSocket()), boost::asio::buffer(client_connection->getBuffer(), MAX_LENGTH), std::bind(&SocketServer::readHandler, this, client_connection, std::placeholders::_1));
    }
}

void SocketServer::readHandler(std::shared_ptr<ClientConnection> client_connection, const error_code &err){
    if(!err){
        auto &success_times = client_connection->getSuccessTimes();
		cocos2d::log("Client Message Fetch Succeeded No.\n");
        std::cout << "Client Message Fetch Succeeded No." << success_times << std::endl;
        std::unique_lock<std::mutex> lk(_mut);
        _stored_lists[client_connection->getID()].push_back(*(client_connection->getBuffer()));
        lk.unlock();
        readFromClient(client_connection);
    }
    else{
        ++(client_connection->getFailureTimes());
		cocos2d::log("Client Message Fetch Failed No.\n");
        std::cerr << "Client Message Fetch Failed No." << client_connection->getFailureTimes() << std::endl;
    }
}

void SocketServer::startService(){
    stopAccept();
    startSend();
}

void SocketServer::startSend(){
    _send_thread.reset(new std::thread(std::bind(&SocketServer::loopSend,this)));
}

void SocketServer::loopSend(){
    while(true){
        if(!checkStop()){
            auto message = combineMessages();
            sendMessages(message);
        }
        else{
            break;
        }
    }
}

std::string SocketServer::combineMessages(){
    std::vector<std::string> raw_messages;
    std::vector<GameMessage> processed_messages;
    std::vector<GameMessage> buffer;
    if(!checkStop()){
        for(auto list: _stored_lists){
            std::unique_lock<std::mutex> lk(_mut);
            if(list.empty()){
                _cond.wait(lk);
            }
            if(!list.empty()){
                auto message_string = list.front();
                list.pop_front();
                buffer = GameMessageInterface::parseMessagesFromString(message_string);
                //!
                std::copy(std::make_move_iterator(buffer.begin()), std::make_move_iterator(buffer.end()), std::back_inserter(processed_messages));
            }
            lk.unlock();
        }
    }
    if(!processed_messages.empty()){
        auto combined_message = GameMessageInterface::combineMessagesToString(processed_messages);
        return combined_message;
    }
    else{
        return "";
    }
}

void SocketServer::sendMessages(std::string message){
    static int write_success_times = 0;
    static int write_failure_times = 0;
    for(auto client_connection: _connections){
        boost::asio::async_write(*(client_connection->getSocket()), boost::asio::buffer(message), [=](const error_code &err, std::size_t bytes_transferred){
            if(!err){
                write_success_times++;
                std::cout << write_success_times << ".: Successfully Transferred Messages to No." << client_connection->getID() << "Client, bytes transferred: " << bytes_transferred << std::endl;
            }
            else{
                write_failure_times++;
                std::cout << write_failure_times << ".: Error Upload Messages to No." << client_connection->getID() << "Client, bytes transferred: " << bytes_transferred << "Missing Bytes: Unknown" << std::endl;
            }
        });
    }
}

bool SocketServer::checkStop(){
    if(_error_flag || _cancel_flag || _stop_flag){
        return true;
    }
    else{
        return false;
    }
}

void SocketServer::stopServer(){
    try{
        std::unique_lock<std::mutex> lk(_mut);
        _stop_flag = true;
        lk.unlock();
        if(_error_flag){
			cocos2d::log("Server Error\n");
            std::cerr << "Server Error" << std::endl;
        }
        else if(_cancel_flag){
			cocos2d::log("Server Cancelled.\n");
            std::cout << "Server Cancelled" << std::endl;
        }
        else{
            std::cout << "Server Shutdown" << std::endl;
        }
        delete _work;
        (*_io).stop();
//        for(auto thread: _io_run_threads_vec){
//            if(thread){
//                thread->join();
//            }
//        }   will exit after work destruct
        for(auto thread: _contact_threads_vector){
            if(thread){
                thread->join();
            }
        }
        if(_send_thread){
            _cond.notify_one();
            _send_thread->join();
        }
        if(_accept_thread && (!_stop_connect)){
            _accept_thread->join();
        }
        for(auto _connection: _connections){
            auto socket_ptr = _connection->getSocket();
            error_code err;
            socket_ptr->shutdown(tcp::socket::shutdown_both, err);
            if(!err){
                throw boost::system::system_error(err);
            }
            socket_ptr->close();
        }
    }
    catch(...){
		cocos2d::log("Server Shutdown Error\n");
        std::cerr << "Server Shutdown Error" << std::endl;
    }
}
