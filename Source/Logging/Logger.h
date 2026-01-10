#pragma once

#include <string>

class Logger
{
public:
    
    static void Info(const std::string& message);
    static void Warn(const std::string& message);
    static void Error(const std::string& message, const std::string& error = "");
};
