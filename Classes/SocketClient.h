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
    static SocketClient* create(std::string server_ip, int port_number);
    ~SocketClient();
private:
    SocketClient(std::string server_ip, int port_number): _socket(_io), _endpoint(boost::asio::ip::address_v4::from_string(server_ip), port_number),_work(_io){
        _exchange_thread.reset(static_cast<std::thread*>(nullptr), [](std::thread*){});
        _read_thread.reset(static_cast<std::thread*>(nullptr), [](std::thread*){});
        //_io.restart();
    }
    
    void startClient();
    void startConnect();
    bool checkStop();
    void stopClient();
    void readMessages();
    void pushMessageSet(const error_code &err);
    bool writeMessages(std::string message_set);
    std::string read_data_once();
    
    void connectHandle(const error_code &err);
    
    std::mutex _mutex;
    std::condition_variable _cond;
    boost::asio::io_service _io;
    tcp::socket _socket;
    tcp::endpoint _endpoint;
    //unique?
    std::shared_ptr<std::thread> _exchange_thread, _read_thread;
    bool _error_flag{false};
    bool _cancel_flag{false};
    bool _stop_flag{false};
    
    std::deque<std::string> _message_set_deque;
    std::string _message_set_buffer;
    boost::asio::io_service::work _work;
    
};

#endif /* __SOCKET_CLIENT_H__ */
