#pragma once

//!REMOVE THIS DEFINE BEFORE SHIPPING
#define LOGGING

#ifdef LOGGING

#include <cstdio>
#include <ctime>
#include <mutex>

enum class LogPriority {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    CRITICAL,
};

struct Logger {
private:
    LogPriority min_priority = LogPriority::INFO;
    std::mutex log_mutex;
    const char* filepath = nullptr;
    FILE* file = nullptr;

public:
    static void set_priority(LogPriority new_priority)
    {
        get_instance().min_priority = new_priority;
    }

    static void enable_file_output()
    {
        Logger& logger_instance = get_instance();
        logger_instance.filepath = "log.txt";
        logger_instance.open_file();
    }

    static void enable_file_output(const char* new_filepath)
    {
        Logger& logger_instance = get_instance();
        logger_instance.filepath = new_filepath;
        logger_instance.open_file();
    }

    template<typename... Args>
    static void trace(const char* message, Args... args)
    {
        get_instance().log("[Trace]\t", LogPriority::TRACE, message, args...);
    }

    template<typename... Args>
    static void debug(const char* message, Args... args)
    {
        get_instance().log("[Debug]\t", LogPriority::DEBUG, message, args...);
    }

    template<typename... Args>
    static void info(const char* message, Args... args)
    {
        get_instance().log("[Info]\t", LogPriority::INFO, message, args...);
    }

    template<typename... Args>
    static void warn(const char* message, Args... args)
    {
        get_instance().log("[Warn]\t", LogPriority::WARN, message, args...);
    }

    template<typename... Args>
    static void error(const char* message, Args... args)
    {
        get_instance().log("[Error]\t", LogPriority::ERROR, message, args...);
    }

    template<typename... Args>
    static void critical(const char* message, Args... args)
    {
        get_instance().log("[Critical]\t", LogPriority::CRITICAL, message, args...);
    }


    template<typename... Args>
    static void trace(int line, const char* source_file, const char* message, Args... args)
    {
        get_instance().log(line, source_file, "[Trc]\t", LogPriority::TRACE, message, args...);
    }

    template<typename... Args>
    static void debug(int line, const char* source_file, const char* message, Args... args)
    {
        get_instance().log(line, source_file, "[Dbg]\t", LogPriority::DEBUG, message, args...);
    }

    template<typename... Args>
    static void info(int line, const char* source_file, const char* message, Args... args)
    {
        get_instance().log(line, source_file, "[Info]\t", LogPriority::INFO, message, args...);
    }

    template<typename... Args>
    static void warn(int line, const char* source_file, const char* message, Args... args)
    {
        get_instance().log(line, source_file, "[Warn]\t", LogPriority::WARN, message, args...);
    }

    template<typename... Args>
    static void error(int line, const char* source_file, const char* message, Args... args)
    {
        get_instance().log(line, source_file, "[Err]\t", LogPriority::ERROR, message, args...);
    }

    template<typename... Args>
    static void critical(int line, const char* source_file, const char* message, Args... args)
    {
        get_instance().log(line, source_file, "[Crit]\t", LogPriority::CRITICAL, message, args...);
    }

private:
    Logger() {
        printf("Instance created\n");
    }

    Logger(Logger const&) = delete;
    Logger& operator=(Logger const&) = delete;

    ~Logger() {
        close_file();
    }

    static Logger& get_instance() {
        static Logger logger;
        return logger;
    }

    template<typename... Args>
    void log(const char* message_priority_str, LogPriority message_priority, const char* message, Args... args) {
        if (min_priority > message_priority) {
            return;
        }

        std::time_t current_time = std::time(0);
        std::tm* timestamp = std::localtime(&current_time);
        char buffer_fd[80];
        char buffer_t[80];
        std::strftime(buffer_fd, 80, "%F %T", timestamp);
        std::strftime(buffer_t, 80, "%T", timestamp);

        log_mutex.lock();
        printf("%s ", buffer_t);
        printf(message_priority_str);
        printf(message, args...);
        printf("\n");
        log_mutex.unlock();

        if (file)
        {
            fprintf(file, "%s\t", buffer_fd);
            fprintf(file, message_priority_str);
            fprintf(file, message, args...);
            fprintf(file, "\n");
        }

    }

    template<typename... Args>
    void log(int line_number, const char* source_file, const char* message_priority_str, LogPriority message_priority, const char* message, Args... args)
    {
        if (min_priority <= message_priority)
        {
            std::time_t current_time = std::time(0);
            std::tm* timestamp = std::localtime(&current_time);
            char buffer_fd[80];
            char buffer_t[80];
            std::strftime(buffer_fd, 80, "%F %T", timestamp);
            std::strftime(buffer_t, 80, "%T", timestamp);

            log_mutex.lock();
            printf("%s ", buffer_t);
            printf(message_priority_str);
            printf(message, args...);
            printf(" on line %d in %s", line_number, source_file);
            printf("\n");
            log_mutex.unlock();

            if (file)
            {
                fprintf(file, "%s\t", buffer_fd);
                fprintf(file, message_priority_str);
                fprintf(file, message, args...);
                fprintf(file, " on line %d in %s", line_number, source_file);
                fprintf(file, "\n");
            }
        }
    }

    bool open_file()
    {
        close_file();

        file = std::fopen(filepath, "a");

        if (!file)
        {
            return false;
        }

        return true;
    }

    void close_file()
    {
        if (file)
        {
            fclose(file);
            file = 0;
        }
    }

};

#define LOG_TRACE(Message, ...) (Logger::trace(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_DEBUG(Message, ...) (Logger::debug(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_INFO(Message, ...) (Logger::info(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_WARN(Message, ...) (Logger::warn(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_ERROR(Message, ...) (Logger::error(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_CRITICAL(Message, ...) (Logger::critical(__LINE__, __FILE__, Message, __VA_ARGS__))
#else
#define LOG_TRACE(Message, ...)
#define LOG_DEBUG(Message, ...)
#define LOG_INFO(Message, ...)
#define LOG_WARN(Message, ...)
#define LOG_ERROR(Message, ...)
#define LOG_CRITICAL(Message, ...)

#endif