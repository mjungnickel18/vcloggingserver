#include "Log2ConsoleCommon.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <windows.h>

std::string Log2ConsoleFormatter::FormatPlainText(LogLevel level, const std::string& category, const std::string& message) {
    SYSTEMTIME st;
    GetLocalTime(&st);
    
    std::stringstream ss;
    ss << std::setfill('0');
    ss << std::setw(4) << st.wYear << "-";
    ss << std::setw(2) << st.wMonth << "-";
    ss << std::setw(2) << st.wDay << " ";
    ss << std::setw(2) << st.wHour << ":";
    ss << std::setw(2) << st.wMinute << ":";
    ss << std::setw(2) << st.wSecond << ".";
    ss << std::setw(3) << st.wMilliseconds;
    ss << " [" << LogLevelToString(level) << "] ";
    ss << "[" << category << "] ";
    ss << message;
    ss << "\r\n";
    
    return ss.str();
}

std::string Log2ConsoleFormatter::FormatLog4jXml(LogLevel level, const std::string& category, const std::string& message) {
    // Get current time as milliseconds since epoch using Windows API
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    
    // Convert FILETIME to milliseconds since epoch
    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    
    // FILETIME is 100-nanosecond intervals since Jan 1, 1601
    // Unix epoch is Jan 1, 1970, which is 11644473600 seconds later
    const ULONGLONG EPOCH_DIFFERENCE = 11644473600ULL * 10000000ULL;
    ULONGLONG ms_since_epoch = (uli.QuadPart - EPOCH_DIFFERENCE) / 10000ULL;
    
    std::stringstream ss;
    ss << "<log4j:event logger=\"" << EscapeXml(category) << "\" ";
    ss << "timestamp=\"" << ms_since_epoch << "\" ";
    ss << "level=\"" << LogLevelToLog4jString(level) << "\" ";
    ss << "thread=\"" << GetCurrentThreadId() << "\">";
    ss << "<log4j:message><![CDATA[" << message << "]]></log4j:message>";
    ss << "<log4j:properties>";
    ss << "<log4j:data name=\"log4net:HostName\" value=\"" << EscapeXml("localhost") << "\"/>";
    ss << "</log4j:properties>";
    ss << "</log4j:event>\0";
    
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