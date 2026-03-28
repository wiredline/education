#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class Logger {

    public:
        static Logger& Instance(){
            static Logger Instance;
            return Instance;
        }

    void LogInfo(const std::string& msg){ m_logger->info(msg); }
    void LogWarn(const std::string& msg){ m_logger->warn(msg); }
    void LogError(const std::string& msg){ m_logger->error(msg); }
    void LogDebug(const std::string& msg){ m_logger->debug(msg); }

        Logger(const Logger&) =delete;
        Logger& operator=(const Logger&) = delete;
    
    private:
    std::shared_ptr<spdlog::logger> m_logger;

    Logger() {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::debug);

        m_logger = std::make_shared<spdlog::logger>("console", console_sink);
        m_logger->set_level(spdlog::level::debug);
        m_logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
        spdlog::register_logger(m_logger);
    }
};

#endif