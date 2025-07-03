#include "Log2ConsoleCommon.h"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <windows.h>

std::string Log2ConsoleFormatter::FormatPlainText(LogLevel level, const std::string& category, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    
    std::tm tm{};
    localtime_s(&tm, &time_t_now);
    
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();
    ss << " [" << LogLevelToString(level) << "] ";
    ss << "[" << category << "] ";
    ss << message;
    ss << "\r\n";
    
    return ss.str();
}

std::string Log2ConsoleFormatter::FormatLog4jXml(LogLevel level, const std::string& category, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto ms_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    
    std::stringstream ss;
    ss << "<log4j:event logger=\"" << EscapeXml(category) << "\" ";
    ss << "timestamp=\"" << ms_since_epoch << "\" ";
    ss << "level=\"" << LogLevelToLog4jString(level) << "\" ";
    ss << "thread=\"" << GetCurrentThreadId() << "\">\r\n";
    ss << "<log4j:message><![CDATA[" << message << "]]></log4j:message>\r\n";
    ss << "<log4j:properties>\r\n";
    ss << "<log4j:data name=\"log4net:HostName\" value=\"" << EscapeXml("localhost") << "\"/>\r\n";
    ss << "</log4j:properties>\r\n";
    ss << "</log4j:event>\r\n";
    
    return ss.str();
}

const char* Log2ConsoleFormatter::LogLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE: return "TRACE";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::L_ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

const char* Log2ConsoleFormatter::LogLevelToLog4jString(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE: return "TRACE";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::L_ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "INFO";
    }
}

std::string Log2ConsoleFormatter::EscapeXml(const std::string& text) {
    std::string result;
    result.reserve(text.size());
    
    for (char c : text) {
        switch (c) {
            case '&':  result += "&amp;"; break;
            case '<':  result += "&lt;"; break;
            case '>':  result += "&gt;"; break;
            case '"':  result += "&quot;"; break;
            case '\'': result += "&apos;"; break;
            default:   result += c; break;
        }
    }
    
    return result;
}