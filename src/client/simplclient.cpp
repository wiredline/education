#include <boost/asio.hpp>
#include <iostream>
#include <getopt.h>
#include <string>

using boost::asio::ip::tcp;


std::string GetOptParse(int argc, char *argv[]) { // -j R"({"operation":"+","args":[10,11]})"
    std::string json_input;
    int opt;

    while ((opt = getopt(argc, argv, "j:")) != -1) {
      switch (opt) {
      case 'j':
        json_input = optarg;
        return json_input;
      default:
        throw std::runtime_error("Invalid options");
      }
    }
    throw std::runtime_error("JSON input not provided. Use -j '<json>'");
  }



int main(int argc, char *argv[]){
    std::string msg;
    //std::string msg = R"({"operation":"+","args":[10,11]})"; //for test
    try{
    msg = GetOptParse(argc, argv);
    }catch(std::exception& e){
        std::cout << e.what() << std::endl;
    }
    boost::asio::io_context io;
    tcp::socket socket(io);

    try{
    socket.connect({boost::asio::ip::make_address("127.0.0.1"), 55555});
    }catch(std::exception& e){
        std::cout << e.what() << std::endl;
    }

    try{
    boost::asio::write(socket, boost::asio::buffer(msg));
    }catch(std::exception& e){
        std::cout << e.what() << std::endl;
    }

    char reply[128];
    size_t len = socket.read_some(boost::asio::buffer(reply));

    std::cout << std::string(reply, len) << std::endl;

}