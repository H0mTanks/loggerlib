#pragma once

//TODO: Cross platform level colors
//TODO: cout printing support
//TODO: Dynamic message_length
//TODO: File output
//TODO: High precision time (atleast ms)
//?Use less includes
//TODO: Release build

//!REMOVE THIS DEFINE BEFORE SHIPPING
#define LOGGING

#ifdef LOGGING

#include <cstdio>
#include <cassert>
#include <ctime>
#include <mutex>
#include <sstream>
#include <iostream>


namespace Logger {
constexpr int MESSAGE_LENGTH = 500;

enum class Priority : unsigned char {
    TRACE = 0,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    CRITICAL,
};

enum Items : unsigned short {
    TIME = 1 << 0,
    LEVEL = 1 << 1,
    STRING = 1 << 2,
    LINE = 1 << 3,
    FILE = 1 << 4,
};

union Format {
    struct {
        unsigned int time : 1;
        unsigned int level : 1;
        unsigned int string : 1;
        unsigned int line : 1;
        unsigned int file : 1;
        unsigned int unused : 3;
    };
    unsigned int print_state = 0;

    Format() : print_state(0) {}
    Format(int ps) : print_state(ps) {}
};

struct State {
private:
    Priority min_priority = Priority::INFO;
    Format curr_format = { Items::LEVEL | Items::STRING };

    friend struct Output;

public:
    static void set_state(Priority new_priority) {
        get_instance().min_priority = new_priority;
    }

    static void set_state(Format new_format) {
        get_instance().curr_format = new_format;
    }

    static void set_state(Priority new_priority, Format new_format) {
        get_instance().curr_format = new_format;
        get_instance().min_priority = new_priority;
    }

private:
    State() {
        std::cout << "Created state instance on thread " << std::this_thread::get_id << std::endl;
    }

    State(State const&) = delete;
    State& operator=(State const&) = delete;

    ~State() {
        std::cout << "Deleted state instance on thread " << std::this_thread::get_id << std::endl;
    }

    static State& get_instance() {
        thread_local State state;
        return state;
    }

};


struct Output {
private:
    std::mutex output_mutex;
public:
    template<typename... Args>
    static void trace(int line, const char* source_file, const char* message, Args... args)
    {
        get_instance().print(line, source_file, "[Trc]\t", Logger::Priority::TRACE, message, args...);
    }
    
    template<typename... Args>
    static void debug(int line, const char* source_file, const char* message, Args... args)
    {
        get_instance().print(line, source_file, "[Dbg]\t", Logger::Priority::DEBUG, message, args...);
    }

    template<typename... Args>
    static void info(int line, const char* source_file, const char* message, Args... args)
    {
        get_instance().print(line, source_file, "[Info]\t", Logger::Priority::INFO, message, args...);
    }

    template<typename... Args>
    static void warn(int line, const char* source_file, const char* message, Args... args)
    {
        get_instance().print(line, source_file, "[Warn]\t", Logger::Priority::WARN, message, args...);
    }

    template<typename... Args>
    static void error(int line, const char* source_file, const char* message, Args... args)
    {
        get_instance().print(line, source_file, "[Err]\t", Logger::Priority::ERROR, message, args...);
    }

    template<typename... Args>
    static void critical(int line, const char* source_file, const char* message, Args... args)
    {
        get_instance().print(line, source_file, "[Crit]\t", Logger::Priority::CRITICAL, message, args...);
    }

private:
    Output() {
        printf("Output Instance Created\n");
    }

    Output(Output const&) = delete;
    Output& operator=(Output const&) = delete;

    ~Output() {
        printf("Output Instance destroyed");
    }

    static Output& get_instance() {
        static Output output;
        return output;
    }

    template<typename... Args>
    void print(int line_number, const char* source_file, const char* message_priority_str, Logger::Priority message_priority, const char* message, Args... args)
    {
        if (State::get_instance().min_priority > message_priority) {
            return;
        }

        //printf("Using state instance: %llu\n", (unsigned long long)(&State::get_instance()));
        //printf("Format: %d", State::get_instance().curr_format.print_state);
        
        // char buffer_fd[80];
        // std::strftime(buffer_fd, 80, "%F %T", timestamp);

        std::stringstream ss;
        if (State::get_instance().curr_format.time) {
            std::time_t current_time = std::time(0);
            std::tm* timestamp = std::localtime(&current_time);
            char buffer_time[40];
            std::strftime(buffer_time, 80, "%T", timestamp);
            
            ss << buffer_time << " \t";
        }

        if (State::get_instance().curr_format.level) {
            ss << message_priority_str << " ";
        }

        if (State::get_instance().curr_format.string) {
            char buffer_m[MESSAGE_LENGTH];
            snprintf(buffer_m, MESSAGE_LENGTH, message, args...);
            ss << buffer_m << " \t";
        }

        if (State::get_instance().curr_format.line) {
            ss << "on line number " << line_number << " in source file " << source_file;
        }
        ss << std::endl;

        std::string output_string = ss.str();
        
        output_mutex.lock();
        printf(output_string.c_str());
        output_mutex.unlock();

        // if (file)
        // {
        //     fprintf(file, "%s\t", buffer_fd);
        //     fprintf(file, message_priority_str);
        //     fprintf(file, message, args...);
        //     fprintf(file, " on line %d in %s", line_number, source_file);
        //     fprintf(file, "\n");
        // }
    }
};

}

// struct Logger {
// private:
//     LogPriority min_priority = LogPriority::INFO;
//     std::mutex log_mutex;
//     const char* filepath = nullptr;
//     FILE* file = nullptr;

// public:
//     static void set_priority(LogPriority new_priority)
//     {
//         get_instance().min_priority = new_priority;
//     }

//     static void enable_file_output()
//     {
//         Logger& logger_instance = get_instance();
//         logger_instance.filepath = "log.txt";
//         logger_instance.open_file();
//     }

//     static void enable_file_output(const char* new_filepath)
//     {
//         Logger& logger_instance = get_instance();
//         logger_instance.filepath = new_filepath;
//         logger_instance.open_file();
//     }

//     template<typename... Args>
//     static void trace(const char* message, Args... args)
//     {
//         get_instance().log("[Trace]\t", LogPriority::TRACE, message, args...);
//     }

//     template<typename... Args>
//     static void debug(const char* message, Args... args)
//     {
//         get_instance().log("[Debug]\t", LogPriority::DEBUG, message, args...);
//     }

//     template<typename... Args>
//     static void info(const char* message, Args... args)
//     {
//         get_instance().log("[Info]\t", LogPriority::INFO, message, args...);
//     }

//     template<typename... Args>
//     static void warn(const char* message, Args... args)
//     {
//         get_instance().log("[Warn]\t", LogPriority::WARN, message, args...);
//     }

//     template<typename... Args>
//     static void error(const char* message, Args... args)
//     {
//         get_instance().log("[Error]\t", LogPriority::ERROR, message, args...);
//     }

//     template<typename... Args>
//     static void critical(const char* message, Args... args)
//     {
//         get_instance().log("[Critical]\t", LogPriority::CRITICAL, message, args...);
//     }


//     template<typename... Args>
//     static void trace(int line, const char* source_file, const char* message, Args... args)
//     {
//         get_instance().log(line, source_file, "[Trc]\t", LogPriority::TRACE, message, args...);
//     }

//     template<typename... Args>
//     static void debug(int line, const char* source_file, const char* message, Args... args)
//     {
//         get_instance().log(line, source_file, "[Dbg]\t", LogPriority::DEBUG, message, args...);
//     }

//     template<typename... Args>
//     static void info(int line, const char* source_file, const char* message, Args... args)
//     {
//         get_instance().log(line, source_file, "[Info]\t", LogPriority::INFO, message, args...);
//     }

//     template<typename... Args>
//     static void warn(int line, const char* source_file, const char* message, Args... args)
//     {
//         get_instance().log(line, source_file, "[Warn]\t", LogPriority::WARN, message, args...);
//     }

//     template<typename... Args>
//     static void error(int line, const char* source_file, const char* message, Args... args)
//     {
//         get_instance().log(line, source_file, "[Err]\t", LogPriority::ERROR, message, args...);
//     }

//     template<typename... Args>
//     static void critical(int line, const char* source_file, const char* message, Args... args)
//     {
//         get_instance().log(line, source_file, "[Crit]\t", LogPriority::CRITICAL, message, args...);
//     }

// private:
//     Logger() {
//         printf("Instance created\n");
//     }

//     Logger(Logger const&) = delete;
//     Logger& operator=(Logger const&) = delete;

//     ~Logger() {
//         close_file();
//     }

//     static Logger& get_instance() {
//         static Logger logger;
//         return logger;
//     }

//     template<typename... Args>
//     void log(const char* message_priority_str, LogPriority message_priority, const char* message, Args... args) {
//         if (min_priority > message_priority) {
//             return;
//         }

//         std::time_t current_time = std::time(0);
//         std::tm* timestamp = std::localtime(&current_time);
//         char buffer_fd[80];
//         char buffer_t[80];
//         std::strftime(buffer_fd, 80, "%F %T", timestamp);
//         std::strftime(buffer_t, 80, "%T", timestamp);

//         log_mutex.lock();
//         printf("%s ", buffer_t);
//         printf(message_priority_str);
//         printf(message, args...);
//         printf("\n");
//         log_mutex.unlock();

//         if (file)
//         {
//             fprintf(file, "%s\t", buffer_fd);
//             fprintf(file, message_priority_str);
//             fprintf(file, message, args...);
//             fprintf(file, "\n");
//         }

//     }

//     template<typename... Args>
//     void log(int line_number, const char* source_file, const char* message_priority_str, LogPriority message_priority, const char* message, Args... args)
//     {
//         if (min_priority <= message_priority)
//         {
//             std::time_t current_time = std::time(0);
//             std::tm* timestamp = std::localtime(&current_time);
//             char buffer_fd[80];
//             char buffer_t[80];
//             std::strftime(buffer_fd, 80, "%F %T", timestamp);
//             std::strftime(buffer_t, 80, "%T", timestamp);

//             log_mutex.lock();
//             printf("%s ", buffer_t);
//             printf(message_priority_str);
//             printf(message, args...);
//             printf(" on line %d in %s", line_number, source_file);
//             printf("\n");
//             log_mutex.unlock();

//             if (file)
//             {
//                 fprintf(file, "%s\t", buffer_fd);
//                 fprintf(file, message_priority_str);
//                 fprintf(file, message, args...);
//                 fprintf(file, " on line %d in %s", line_number, source_file);
//                 fprintf(file, "\n");
//             }
//         }
//     }

//     bool open_file()
//     {
//         close_file();

//         file = std::fopen(filepath, "a");

//         if (!file)
//         {
//             return false;
//         }

//         return true;
//     }

//     void close_file()
//     {
//         if (file)
//         {
//             fclose(file);
//             file = 0;
//         }
//     }

// };

#define LOG_TRACE(Message, ...) (Logger::Output::trace(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_DEBUG(Message, ...) (Logger::Output::debug(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_INFO(Message, ...) (Logger::Output::info(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_WARN(Message, ...) (Logger::Output::warn(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_ERROR(Message, ...) (Logger::Output::error(__LINE__, __FILE__, Message, __VA_ARGS__))
#define LOG_CRITICAL(Message, ...) (Logger::Output::critical(__LINE__, __FILE__, Message, __VA_ARGS__))
#else
#define LOG_TRACE(Message, ...)
#define LOG_DEBUG(Message, ...)
#define LOG_INFO(Message, ...)
#define LOG_WARN(Message, ...)
#define LOG_ERROR(Message, ...)
#define LOG_CRITICAL(Message, ...)

#endif