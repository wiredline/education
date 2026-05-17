#pragma once

#include <boost/asio.hpp>
#include <string>

class TestClient{
public:
    TestClient(const std::string& host, int port) : io_(), socket_(io_){
        boost::asio::ip::tcp::resolver resolver(io_);
        auto endpoints = resolver.resolve(host, std::to_string(port));
        boost::asio::connect(socket_, endpoints);
    }

    std::string Send(const std::string& msg){
        boost::asio::write(socket_, boost::asio::buffer(msg));
        
        char buffer[1024];
        size_t len = socket_.read_some(boost::asio::buffer(buffer));

        return std::string(buffer, len);
    }

private:
    boost::asio::io_context io_;
    boost::asio::ip::tcp::socket socket_;
};