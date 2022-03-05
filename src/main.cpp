#include "logger.hpp"
#include <iostream>

int main() {
    const char* name = "User";
    LOG_INFO("Hello %s", name);

    LOG_CRITICAL("OH NO!", name);

    LOG_WARN("Hello %d %d", 15, 28);

    return 0;
}