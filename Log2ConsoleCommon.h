#pragma once

#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <windows.h>

enum class LogLevel {
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    L_ERROR = 4,  // Renamed to avoid conflict with Windows ERROR macro
    FATAL = 5
};

class Log2ConsoleFormatter {
public:
    static std::string FormatPlainText(LogLevel level, const std::string& category, const std::string& message);
    static std::string FormatLog4jXml(LogLevel level, const std::string& category, const std::string& message);
    
    static const char* LogLevelToString(LogLevel level);
    static const char* LogLevelToLog4jString(LogLevel level);
    
private:
    static std::string EscapeXml(const std::string& text);
};