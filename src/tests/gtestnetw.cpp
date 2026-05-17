#include <gtest/gtest.h>
#include <thread>
#include <atomic>
#include "server.hpp"
#include "Worker.hpp"
#include "test_client.cpp"

class NetworkTest: public  ::testing::Test{
protected:
    void SetUp() override{
        running = true;
        
        server_thread = std::thread([this](){
        boost::asio::io_context io;
        Server server(io, 55555, queue);
        server.startAccept();
        io.run();
        });

        worker_thread = std::thread([this]{Worker::Run(running, queue);});

        WaitForServer();
    }

    void TearDown() override{
        running = false;
        if(server_thread.joinable()) server_thread.join();
        if(worker_thread.joinable()) worker_thread.join();
    }

    void WaitForServer(){
        auto start = std::chrono::steady_clock::now();

        while(true){
            try{
                TestClient c("127.0.0.1", 55555);
                return;
            }catch(...){}

            if(std::chrono::steady_clock::now() - start > std::chrono::seconds(5)){
                throw std::runtime_error("Server not ready");
            }
        }
    }

private:
    std::atomic_bool running;
    TaskQueue queue;
    std::thread server_thread;
    std::thread worker_thread;
};

TEST_F(NetworkTest, AdditionTest){
    TestClient client("127.0.0.1", 55555);

    std::string request = R"({
        "operation": "+",
        "args": [3, 5]
    })";

    std::string response = client.Send(request);

    EXPECT_NE(response.find("8"), std::string::npos);
}