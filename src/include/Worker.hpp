#pragma once

#include "logger.hpp"
#include "runner.hpp"
#include "task_queue.hpp"
#include <atomic>
#include <chrono>
#include <thread>

class Worker
{
  public:
    static void Run(std::atomic_bool &running, TaskQueue& queue)
    {
        Logger::Instance().LogInfo("Worker started");
        while (running.load())
        {            
            std::string task;
            if(!queue.Pop(task)){
                break;
            }
            // start
            try
            {
                Runner runner(task);
                runner.StartTask();
            }
            catch (const std::exception &error)
            {

                Logger::Instance().LogError(error.what());
            }

            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        Logger::Instance().LogInfo("worker stopped");
    }
};