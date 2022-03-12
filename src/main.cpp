#include "logger.hpp"
#include <iostream>
#include <thread>

void print_test2() {
    const char* name = "User1";
    Logger::State::set_state({ Logger::TIME | Logger::LEVEL | Logger::STRING });
    LOG_INFO("Hello %s", name);

    LOG_CRITICAL("OH NO!", name);

    LOG_WARN("Hello %d %d", 15, 39);
}

void print_test() {
    const char* name = "User1";
    Logger::State::set_state({Logger::TIME | Logger::LEVEL | Logger::STRING | Logger::LINE});
    LOG_INFO("Hello %s", name);

    LOG_CRITICAL("OH NO!", name);

    LOG_WARN("Hello %d %d", 15, 36);
}

int main() {
    const char* name = "User";
    //Logger::State::set_state({(Logger::Items::TIME | Logger::Items::LEVEL | Logger::Items::STRING | Logger::Items::LINE)});
    LOG_INFO("Hello %s", name);

    LOG_CRITICAL("OH NO!", name);

    LOG_WARN("Hello %d %d", 15, 28);

    //print_test();

    std::cout << "-----------------" << std::endl;

    std::thread t(print_test);
    std::thread t2(print_test2);
    t.join();
    t2.join();

    std::cout << "-----------------" << std::endl;

    LOG_INFO("Hello %s", name);

    LOG_CRITICAL("OH NO!", name);

    LOG_WARN("Hello %d %d", 15, 28);

    return 0;
}