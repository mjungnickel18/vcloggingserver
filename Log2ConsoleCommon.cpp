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
    
    // Get sequence number for this log message
    unsigned long sequenceNumber = GetNextSequenceNumber();
    
    std::stringstream ss;
    ss << "<log4j:event logger=\"" << EscapeXml(category) << "\" ";
    ss << "timestamp=\"" << ms_since_epoch << "\" ";
    ss << "level=\"" << LogLevelToLog4jString(level) << "\" ";
    ss << "thread=\"" << GetCurrentThreadId() << "\">";
    ss << "<log4j:message><![CDATA[" << message << "]]></log4j:message>";
    ss << "<log4j:properties>";
    ss << "<log4j:data name=\"log4net:HostName\" value=\"" << EscapeXml("localhost") << "\"/>";
    ss << "<log4j:data name=\"nlog:eventSequenceNumber\" value=\"" << sequenceNumber << "\"/>";
    ss << "</log4j:properties>";
    ss << "</log4j:event>\0";
    
    return ss.str();
}

std::string Log2ConsoleFormatter::FormatLog4jXml(LogLevel level, const std::string& category, const std::string& message,
                                                  const char* file, const char* function, int line) {
    auto now = std::chrono::system_clock::now();
    auto ms_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    
    // Get sequence number for this log message
    unsigned long sequenceNumber = GetNextSequenceNumber();
    
    // Extract just the filename from the full path
    const char* filename = file;
    const char* lastSlash = file;
    while (*lastSlash) {
        if (*lastSlash == '\\' || *lastSlash == '/') {
            filename = lastSlash + 1;
        }
        lastSlash++;
    }
    
    std::stringstream ss;
    ss << "<log4j:event logger=\"" << EscapeXml(category) << "\" ";
    ss << "timestamp=\"" << ms_since_epoch << "\" ";
    ss << "level=\"" << LogLevelToLog4jString(level) << "\" ";
    ss << "thread=\"" << GetCurrentThreadId() << "\">";
    ss << "<log4j:message><![CDATA[" << message << "]]></log4j:message>";
    ss << "<log4j:locationInfo class=\"" << EscapeXml(category) << "\" ";
    ss << "method=\"" << EscapeXml(function) << "\" ";
    ss << "file=\"" << EscapeXml(filename) << "\" ";
    ss << "line=\"" << line << "\"/>";
    ss << "<log4j:properties>";
    ss << "<log4j:data name=\"log4net:HostName\" value=\"" << EscapeXml("localhost") << "\"/>";
    ss << "<log4j:data name=\"log4net:UserName\" value=\"" << EscapeXml("User") << "\"/>";
    ss << "<log4j:data name=\"nlog:eventSequenceNumber\" value=\"" << sequenceNumber << "\"/>";
    ss << "</log4j:properties>";
    ss << "</log4j:event>\0";
    
    return ss.str();
}

const char* Log2ConsoleFormatter::LogLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::L_TRACE: return "TRACE";
        case LogLevel::L_DEBUG: return "DEBUG";
        case LogLevel::L_INFO:  return "INFO";
        case LogLevel::L_WARN:  return "WARN";
        case LogLevel::L_ERROR: return "ERROR";
        case LogLevel::L_FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

const char* Log2ConsoleFormatter::LogLevelToLog4jString(LogLevel level) {
    switch (level) {
        case LogLevel::L_TRACE: return "TRACE";
        case LogLevel::L_DEBUG: return "DEBUG";
        case LogLevel::L_INFO:  return "INFO";
        case LogLevel::L_WARN:  return "WARN";
        case LogLevel::L_ERROR: return "ERROR";
        case LogLevel::L_FATAL: return "FATAL";
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

unsigned long Log2ConsoleFormatter::GetNextSequenceNumber() {
    static unsigned long sequenceCounter = 0;
    return ++sequenceCounter;
}