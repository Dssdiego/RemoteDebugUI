#include "Logger.h"

#include <iostream>
#include <ostream>

void Logger::Info(const std::string& message)
{
    std::cout << "[Info] " << message << std::endl;
}
