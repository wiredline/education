#pragma once

#include <atomic>
#include <chrono>
#include <csignal>
#include <thread>

namespace
{
inline std::atomic_bool *g_running = nullptr;
void HandleSignal([[maybe_unused]] int signal)
{
    if (g_running->load())
    {
        g_running->store(false); // off anything
    }
}
} // namespace

class Signalhandler
{
  public:
    static void WaitSignal(std::atomic_bool &running)
    {
        g_running = &running;

        std::signal(SIGINT, HandleSignal);
        std::signal(SIGTERM, HandleSignal);

        while (running.load())
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};