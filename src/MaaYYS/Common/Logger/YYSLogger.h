#pragma once

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

namespace asst::yys {

enum class LogLevel {
    Debug = 0,
    Info = 1,
    Warn = 2,
    Error = 3,
    Off = 4,
};

class YYSLogger {
public:
    static YYSLogger& instance()
    {
        static YYSLogger logger;
        return logger;
    }

    void set_level(LogLevel level) { m_level = level; }
    LogLevel level() const { return m_level; }

    void log(LogLevel level, const char* file, int line, const char* func,
              const char* fmt, ...) const
    {
        if (level < m_level) return;

        const char* level_str = "DEBUG";
        switch (level) {
        case LogLevel::Debug: level_str = "DEBUG"; break;
        case LogLevel::Info:  level_str = "INFO";  break;
        case LogLevel::Warn:  level_str = "WARN";  break;
        case LogLevel::Error: level_str = "ERROR"; break;
        default: return;
        }

        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        std::tm tm_buf{};
#ifdef _WIN32
        localtime_s(&tm_buf, &time_t);
#else
        localtime_r(&time_t, &tm_buf);
#endif

        char time_buf[32];
        std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &tm_buf);

        const char* short_file = file;
        if (file) {
            const char* last_slash = strrchr(file, '/');
            const char* last_backslash = strrchr(file, '\\');
            const char* last_sep = last_slash > last_backslash ? last_slash : last_backslash;
            if (last_sep) short_file = last_sep + 1;
        }

        va_list args;
        va_start(args, fmt);

        fprintf(stdout, "[%s.%03d][%s][%s:%d][%s] ",
                time_buf, static_cast<int>(ms.count()),
                level_str, short_file ? short_file : "", line,
                func ? func : "");
        vfprintf(stdout, fmt, args);
        fprintf(stdout, "\n");
        fflush(stdout);

        va_end(args);
    }

private:
    YYSLogger() = default;
    LogLevel m_level = LogLevel::Debug;
};

} // namespace asst::yys

#define YYS_LOG_DEBUG(fmt, ...) \
    ::asst::yys::YYSLogger::instance().log(::asst::yys::LogLevel::Debug, \
        __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

#define YYS_LOG_INFO(fmt, ...) \
    ::asst::yys::YYSLogger::instance().log(::asst::yys::LogLevel::Info, \
        __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

#define YYS_LOG_WARN(fmt, ...) \
    ::asst::yys::YYSLogger::instance().log(::asst::yys::LogLevel::Warn, \
        __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

#define YYS_LOG_ERROR(fmt, ...) \
    ::asst::yys::YYSLogger::instance().log(::asst::yys::LogLevel::Error, \
        __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
