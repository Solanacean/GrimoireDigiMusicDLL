#pragma once

#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <ctime>
#include <ios>

#define LOG(lineLevel, ...) Logger::Instance().Log(lineLevel, __FUNCTION__, __VA_ARGS__)

#define LOG_CRITICAL(...) LOG(Logger::Level::critical, __VA_ARGS__)
#define LOG_ERROR(...) LOG(Logger::Level::error, __VA_ARGS__)
#define LOG_WARNING(...) LOG(Logger::Level::warning, __VA_ARGS__)
#define LOG_INFO(...) LOG(Logger::Level::info, __VA_ARGS__)
#define LOG_DEBUG(...) LOG(Logger::Level::debug, __VA_ARGS__)
#define LOG_VERBOSE(...) LOG(Logger::Level::verbose, __VA_ARGS__)

#define LOGC(...) LOG_CRITICAL(__VA_ARGS__)
#define LOGE(...) LOG_ERROR(__VA_ARGS__)
#define LOGW(...) LOG_WARNING(__VA_ARGS__)
#define LOGI(...) LOG_INFO(__VA_ARGS__)
#define LOGD(...) LOG_DEBUG(__VA_ARGS__)
#define LOGV(...) LOG_VERBOSE(__VA_ARGS__)

#define UNEXPECTEDMETHODCALL do { \
LOG_CRITICAL("Unexpected method call!"); \
return 0; } while (0)
//_asm int 3

#ifdef _DEBUG
#define LOGFUNCTIONCALL LOG_DEBUG("...")
#else
#define LOGFUNCTIONCALL
#endif // _DEBUG

class Logger
{

public:
    enum class Level
    {
        info,
        critical,
        error,
        warning,
        //info,
        debug,
        verbose
    };

    Logger(Logger const&) = delete;
    void operator= (Logger const&) = delete;

    static Logger& Instance()
    {
        static Logger logger;
        return logger;
    }

    bool Init(const char *logFileName, Level loggerLevel)
    {
        if (!m_initialized)
        {
            m_ofStream.open(logFileName);
            m_loggerLevel = loggerLevel;
            m_initialized = true;
            return true;
        }
        return false;
    }

    template <typename... TArgs>
    void Log(Level lineLevel, const char *funcName, const TArgs&... args)
    {
        // Logging started
        if (lineLevel <= m_loggerLevel /*&& m_initialized*/ && m_ofStream.is_open())
        {
            std::lock_guard<std::mutex> lock(m_mutex);

            m_ofStream << GetTimeDate().c_str()
                       << GetLevelStr(lineLevel) << ' '
                       << '[' << std::this_thread::get_id() << ']'
                       << " [" << funcName << "] ";

            Log_Recursive(args...);

            m_ofStream << std::endl;
        }
        // Logging ended
    }

private:
    Logger() {}

    ~Logger()
    {
        if (/*m_initialized &&*/ m_ofStream.is_open())
        {
            //m_ofStream.flush();
            m_ofStream.close();
        }
    }

    template <typename TArg>
    void Log_Recursive(const TArg& arg)
    {
        m_ofStream << arg;
    }

    template <typename TArg, typename... TArgs>
    void Log_Recursive(const TArg& arg, const TArgs&... args)
    {
        m_ofStream << arg;
        Log_Recursive(args...);
    }

    std::string GetTimeDate()
    {
        auto now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm ptm;
        localtime_s(&ptm, &now_c);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%F %T ", &ptm);
        return buffer;
    }

    std::string GetLevelStr(Level lineLevel)
    {
        switch (lineLevel)
        {
        case Level::critical:
            return "CRT";

        case Level::error:
            return "ERR";

        case Level::warning:
            return "WRN";

        case Level::info:
            return "INF";

        case Level::debug:
            return "DBG";

        case Level::verbose:
            return "VRB";

        default:
            return "???";
        }
    }

    std::mutex m_mutex;
    std::ofstream m_ofStream;
    Level m_loggerLevel;
    bool m_initialized;
};
