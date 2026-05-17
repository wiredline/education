#pragma once

#include "logger.hpp"
#include "runner.hpp"
#include <atomic>
#include <chrono>
#include <thread>

class Worker
{
  public:
    static void Run(std::atomic_bool &running, const std::string& input)
    {
        Logger::Instance().LogInfo("Worker started");
        while (running)
        {

            // start
            try
            {
                Runner runner(input);
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