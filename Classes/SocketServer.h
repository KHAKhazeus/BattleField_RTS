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
#include "include/boost/asio.hpp"
#include "GameMessageInterface.h"
using boost::asio::ip::tcp;
using boost::system::error_code;

class ClientConnection{
public:
    tcp::socket* getSocket();
    std::string* getBuffer();
    int& getSuccessTimes();
    int& getFailureTimes();
    int& mutableID();
    const int& getID();
    static std::shared_ptr<ClientConnection> create(boost::asio::io_service &io_service);
private:
    ClientConnection(boost::asio::io_service &io_service);
    int _ID;
    tcp::socket _socket;
    std::string _message_buffer;
    int _success_times;
    int _failure_times;
};

class SocketServer{
public:
    static SocketServer* create(int port_number);
    void startService();
    ~SocketServer();
    
private:
    SocketServer(int port_number);
    void startServerListen();
    void startAccept();
    void stopAccept();
    bool checkStop();
    void stopServer();
    void handleAccept(std::shared_ptr<ClientConnection> new_connection, const error_code &err);
    void readFromClient(std::shared_ptr<ClientConnection> client_connection);
    void readHandler(std::shared_ptr<ClientConnection> client_connection, const error_code &err);
    void startSend();
    void combineMessages();
    void sendMessages(std::string message);
    
    bool _error_flag{false}, _cancel_flag{false}, _stop_flag{false}, _stop_connect{false};
    boost::asio::io_service _io;
    tcp::acceptor _acceptor;
    std::mutex _mut;
    std::condition_variable _cond;
    std::vector<std::shared_ptr<ClientConnection>> _connections;
    std::vector<std::shared_ptr<std::thread>> _io_run_threads_vec;
    std::shared_ptr<std::thread> _accept_thread{static_cast<std::thread*>(nullptr), [](std::thread*){}};
    std::vector<std::shared_ptr<std::thread>> _contact_threads_vector;
    std::shared_ptr<std::thread> _send_thread{static_cast<std::thread*>(nullptr), [](std::thread*){}};
    std::vector<std::list<std::string>> _stored_lists;
    std::vector<std::string> _combined_messages;
    boost::asio::io_service::work _work;
};

#endif /* __SOCKET_SERVER_H__ */
