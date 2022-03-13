#include "logger.hpp"

#define color_red 15

void print_test2() {
    const char* name = "User1";
    LOGGER_STATEF({ Logger::TIME | Logger::LEVEL | Logger::STRING });
    LOG_INFO("Hello %s", name);

    LOG_CRITICAL("OH NO!", name);

    LOG_WARN("Hello %d %d", 15, 39);
}

void print_test() {
    const char* name = "User1";
    LOGGER_STATEF({Logger::TIME | Logger::LEVEL | Logger::STRING | Logger::LINE});
    LOG_INFO("Hello %s", name);

    LOG_CRITICAL("OH NO!", name);

    LOG_WARN("Hello %d %d", 15, 36);
}

int main() {
    const char* name = "User";
    LOG_TRACE("This is a trace");

    LOG_DEBUG("This is a debug");
    LOG_INFO("Hello %s", name);

    LOG_CRITICAL("OH NO!", name);

    LOG_WARN("Hello %d %d", 15, 28);


    printf("Hello default color\n");

    //print_test();
    LOGGER_STATEP(Logger::Priority::TRACE);
    LOG_TRACE("This is a trace");

    LOG_DEBUG("This is a debug");

    LOG_INFO("Hello %s", name);

    LOG_CRITICAL("OH NO!", name);

    LOG_WARN("Hello %d %d", 15, 28);

    printf(STRINGIFY_EXPANDED(color_red));

    return 0;
}