//
//  SocketClient.h
//  BattleField_RTS
//
//  Created by Khazeus on 2018/6/10.
//

#ifndef __SOCKET_CLIENT_H__
#define __SOCKET_CLIENT_H__

#include "include/boost/asio.hpp"
//#include "GameMessage.pb.h"
#include "GameMessageInterface.h"
#include <thread>
#include <memory>
#include <string>
#include <mutex>

using boost::asio::ip::tcp;
using boost::system::error_code;

class SocketClient{
public:
    ~SocketClient();
    static SocketClient* create(std::string server_ip, int port_number);
    
private:
    SocketClient(std::string server_ip, int port_number): _socket(_io), _endpoint(boost::asio::ip::address_v4::from_string(server_ip), port_number){}
    
    void startClient();
    void startConnect();
    void checkStop();
    void stopClient();
    void readMessages();
    void pushMessageSet(const error_code &err);
    void writeMessages(std::string message_set);
    
    void connectHandle(const error_code &err);
    
    std::mutex _mutex;
    std::condition_variable _cond;
    boost::asio::io_service _io;
    tcp::socket _socket;
    tcp::endpoint _endpoint;
    std::unique_ptr<std::thread> _exchange_thread, _read_thread;
    bool _error_flag{false};
    bool _cancel_flag{false};
    bool _stop_flag{false};
    
    std::deque<std::string> _message_set_deque;
    std::string _message_set_buffer;
    
};

#endif /* __SOCKET_CLIENT_H__ */
