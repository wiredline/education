#include "Signalhandler.hpp"
#include "Worker.hpp"
#include "logger.hpp"
#include <atomic>
#include <thread>
#include <string>


int main(int argc, char *argv[]) //'{"operation": "+", "args": [3, 5]}'
{
    Logger::Instance().LogInfo("Program started");

    std::atomic_bool running = true;

    std::string input = Runner::GetOptParse(argc, argv);

    std::thread signalThread(Signalhandler::WaitSignal,std::ref(running)); // first thread on signal from system

    std::thread worker_thread(Worker::Run, std::ref(running), std::cref(input)); // second thread on main worker

    signalThread.join();
    worker_thread.join();

    Logger::Instance().LogInfo("Program Closed");
    return 0;
}