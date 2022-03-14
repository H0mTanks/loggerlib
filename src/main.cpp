#include "logger.hpp"
#include <sstream>
#include <iostream>

#define color_red 15

void print_test2() {
    const char* name = "User1";
    LOGGER_FORMAT({ Logger::TIME | Logger::LEVEL | Logger::STRING });
    LOG_INFO("Hello %s", name);

    LOG_CRITICAL("OH NO!", name);

    LOG_WARN("Hello %d %d", 15, 39);
}

void print_test() {
    const char* name = "User1";
    LOGGER_FORMAT({Logger::TIME | Logger::LEVEL | Logger::STRING | Logger::LINE});
    LOG_INFO("Hello %s", name);

    LOG_CRITICAL("OH NO!", name);

    LOG_WARN("Hello %d %d", 15, 36);
}

int main() {
    const char* name = "User";
    //LOGGER_FORMAT({ Logger::TIME | Logger::LEVEL | Logger::STRING | Logger::LINE });

    LOG_TRACE("This is a trace");

    LOG_DEBUG("This is a debug");
    LOG_INFO("Hello %s", name);

    LOG_CRITICAL("OH NO!", name);

    LOG_WARN("Hello %d %d", 15, 28);


    printf("Hello default color\n");

    //print_test();
    LOGGER_PRIORITY(Logger::Priority::TRACE);
    LOG_TRACE("This is a trace");

    LOG_DEBUG("This is a debug");

    LOG_INFO("Hello %s", name);

    LOG_CRITICAL("OH NO!", name);

    LOG_WARN("Hello %d %d", 15, 28);

    //printf(STRINGIFY_EXPANDED(color_red));

    printf("\n");

    CLOG_INFO("hello");
    CLOG_INFO(1, 2, 3, "hello", name, 76);
    CLOG_TRACE(1, 2, 3, "hello", name, 76);
    CLOG_WARN(1, 2, 3, "hello", name, 76);
    CLOG_DEBUG(1, 2, 3, "hello", name, 76);
    CLOG_ERROR(1, 2, 3, "hello", name, 76);
    CLOG_CRITICAL(1, 2, 3, "hello", name, 76);

    return 0;
}