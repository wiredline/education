#include "Signalhandler.hpp"
#include "Worker.hpp"
#include "logger.hpp"
#include "task_queue.hpp"
#include "server.hpp"
#include <atomic>
#include <thread>
#include <string>
#include <boost/asio.hpp>

//int main(int argc, char *argv[]) //'{"operation": "+", "args": [3, 5]}'
int main(){
    Logger::Instance().LogInfo("Program started");

    std::atomic_bool running = true;
    TaskQueue queue;
    boost::asio::io_context io;
    try{
    auto server = std::make_shared<Server>(io, 55555, queue);
    server->startAccept();
    }
    catch(std::exception& e){
        Logger::Instance().LogError(e.what());
    }
    
    //Legacy for manual input
    /*if(argc != 0){ 
        queue.Push(Runner::GetOptParse(argc, argv));
    }*/

    std::thread worker([&]{Worker::Run(running, queue);}); // second thread on main worker
    std::thread signalThread([&]{Signalhandler::WaitSignal(running);}); // first thread on signal from system
    std::thread networkThread([&]{io.run();}); //third thread for network

    Logger::Instance().LogInfo("Signal thread destruct");
    signalThread.join();
    Logger::Instance().LogInfo("queue destruct");
    queue.Stop();
    Logger::Instance().LogInfo("IO server destruct");
    io.stop();
    Logger::Instance().LogInfo("Worker thread destruct");
    worker.join();
    Logger::Instance().LogInfo("Network thread destruct");
    networkThread.join();

    Logger::Instance().LogInfo("Program Closed");
    return 0;
}