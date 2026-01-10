#include "Logger.h"

#include <iostream>
#include <ostream>

void Logger::Info(const std::string& message)
{
    std::cout << "[Info] " << message << '\n';
}

void Logger::Warn(const std::string& message)
{
    std::cout << "[Warn] " << message << '\n';
}

void Logger::Error(const std::string& message, const std::string& error)
{
    std::cout << "[Error] " << message << " | " << error << '\n';
}

