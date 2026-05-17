#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <iostream>

#include "task_queue.hpp"
#include "logger.hpp"

using boost::asio::ip::tcp;

class Server : public std::enable_shared_from_this<Server>{
public:
    Server(boost::asio::io_context& io, int port, TaskQueue& queue)
        :acceptor_(io), queue_(queue){
            Logger::Instance().LogInfo("Server constructor");
            tcp::endpoint endpoint(tcp::v4(), port);
            acceptor_.open(endpoint.protocol());
            acceptor_.set_option(boost::asio::socket_base::reuse_address(true));
            acceptor_.bind(endpoint);
            Logger::Instance().LogInfo("Bind success");
            acceptor_.listen();
            Logger::Instance().LogInfo("Listen success");
            
            Logger::Instance().LogInfo("Server started");
        }

        void startAccept(){
            DoAccept();
        }
private:
        void DoAccept(){
            Logger::Instance().LogInfo("Waiting for connection");
            auto socket = std::make_shared<tcp::socket>(acceptor_.get_executor());

            auto self = shared_from_this(); 
            acceptor_.async_accept(*socket, [self, socket](boost::system::error_code ec){
                if(!ec) {
                    self->Read(socket);
                }
                else {
                        Logger::Instance().LogError(ec.message());
                    }
                self->DoAccept();
            });
        }

        void Read(std::shared_ptr<tcp::socket> socket){
            auto buffer = std::make_shared<std::string>(1024, '\0');

            socket->async_read_some(
                boost::asio::buffer(*buffer),
                [this, socket, buffer](boost::system::error_code ec, std::size_t len){
                    if(!ec){
                        std::string task = buffer->substr(0, len);
                        queue_.Push(task);
                        Write(socket, "OK");
                    }
                    else {
                        Logger::Instance().LogError(ec.message());
                    }
                });
        }

        void Write(std::shared_ptr<tcp::socket> socket ,const std::string& msg){
            auto response = std::make_shared<std::string>(msg);

            boost::asio::async_write(
                *socket,
                boost::asio::buffer(*response),
                [socket, response](boost::system::error_code, std::size_t){}
            );
        }
    
private:
    tcp::acceptor acceptor_;
    TaskQueue& queue_;
};