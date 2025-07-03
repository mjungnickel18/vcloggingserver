#pragma once

#include <string>
#include <memory>

enum class LogLevel {
    L_TRACE = 0,
    L_DEBUG = 1,
    L_INFO = 2,
    L_WARN = 3,
    L_ERROR = 4,
    L_FATAL = 5
};

class Log2ConsoleFormatter {
public:
    static std::string FormatPlainText(LogLevel level, const std::string& category, const std::string& message);
    static std::string FormatLog4jXml(LogLevel level, const std::string& category, const std::string& message);
    static std::string FormatLog4jXml(LogLevel level, const std::string& category, const std::string& message, 
                                      const char* file, const char* function, int line);
    
    static const char* LogLevelToString(LogLevel level);
    static const char* LogLevelToLog4jString(LogLevel level);
    
private:
    static std::string EscapeXml(const std::string& text);
};