#pragma once

#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include "logger.hpp"

class TaskQueue{
public:
    void Push(std::string task){
        {
        std::lock_guard<std::mutex> lock(m_);
        
        queue_.push(std::move(task)); //add 
        }
        cv_.notify_one();
    }

    bool Pop(std::string& task){
        std::unique_lock<std::mutex> lock(m_);
        Logger::Instance().LogInfo("Pop waiting");
        cv_.wait(lock, [this] {return stopped_ || !queue_.empty();});
        Logger::Instance().LogInfo("Pop awakened");

        if(stopped_) {
            Logger::Instance().LogWarn("Queue stopped");
            return false;
        }

        task = std::move(queue_.front()); //back
        queue_.pop();
        
        return true;
    } 

    void Stop(){
        {
            std::lock_guard<std::mutex> lock(m_);
            stopped_ = true;
        }
        cv_.notify_all();
    }

private:
    std::queue<std::string> queue_;
    std::mutex m_;
    std::condition_variable cv_;
    bool stopped_ = false;
};