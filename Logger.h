#pragma once

#include "Log2ConsoleUdpClient.h"
#include <memory>
#include <mutex>

class Logger {
public:
    static Logger& GetInstance();

    // Initialize the logger with server details
    bool Initialize(const std::string& serverHost = "localhost", int serverPort = 4445, bool useXmlFormat = true);
    void Cleanup();
    bool IsInitialized() const;

    // Standard log method
    void Log(LogLevel level, const std::string& category, const std::string& message);

    // Extended log method with file, function, and line information
    void LogWithLocation(LogLevel level, const std::string& category, const std::string& message, 
                        const char* file, const char* function, int line);

    // Printf-style log methods with one parameter
    template<typename T>
    void Log(LogLevel level, const std::string& category, const std::string& format, T value);
    
    template<typename T>
    void LogWithLocation(LogLevel level, const std::string& category, const std::string& format, T value,
                        const char* file, const char* function, int line);

    // Printf-style log methods with two parameters
    template<typename T1, typename T2>
    void Log(LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2);
    
    template<typename T1, typename T2>
    void LogWithLocation(LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2,
                        const char* file, const char* function, int line);

    // Printf-style log methods with three parameters
    template<typename T1, typename T2, typename T3>
    void Log(LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2, T3 value3);
    
    template<typename T1, typename T2, typename T3>
    void LogWithLocation(LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2, T3 value3,
                        const char* file, const char* function, int line);

    // Set XML format preference
    void SetXmlFormat(bool useXml);

private:
    Logger() = default;
    ~Logger() = default;

    // Delete copy and move constructors/operators
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    std::unique_ptr<Log2ConsoleUdpClient> m_client;
    mutable std::mutex m_mutex;
    bool m_initialized = false;
};

// Convenience macros for logging with automatic file/function/line info
#define LTC_TRACE(category, message) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_TRACE, category, message, __FILE__, __FUNCTION__, __LINE__)

#define LTC_DEBUG(category, message) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_DEBUG, category, message, __FILE__, __FUNCTION__, __LINE__)

#define LTC_INFO(category, message) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_INFO, category, message, __FILE__, __FUNCTION__, __LINE__)

#define LTC_WARN(category, message) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_WARN, category, message, __FILE__, __FUNCTION__, __LINE__)

#define LTC_ERROR(category, message) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_ERROR, category, message, __FILE__, __FUNCTION__, __LINE__)

#define LTC_FATAL(category, message) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_FATAL, category, message, __FILE__, __FUNCTION__, __LINE__)

// Simple macros without file/function/line info
#define LTCS_TRACE(category, message) \
    Logger::GetInstance().Log(LogLevel::L_TRACE, category, message)

#define LTCS_DEBUG(category, message) \
    Logger::GetInstance().Log(LogLevel::L_DEBUG, category, message)

#define LTCS_INFO(category, message) \
    Logger::GetInstance().Log(LogLevel::L_INFO, category, message)

#define LTCS_WARN(category, message) \
    Logger::GetInstance().Log(LogLevel::L_WARN, category, message)

#define LTCS_ERROR(category, message) \
    Logger::GetInstance().Log(LogLevel::L_ERROR, category, message)

#define LTCS_FATAL(category, message) \
    Logger::GetInstance().Log(LogLevel::L_FATAL, category, message)

// Printf-style macros with automatic file/function/line info
#define LTC_TRACE_F1(category, format, value) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_TRACE, category, format, value, __FILE__, __FUNCTION__, __LINE__)

#define LTC_DEBUG_F1(category, format, value) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_DEBUG, category, format, value, __FILE__, __FUNCTION__, __LINE__)

#define LTC_INFO_F1(category, format, value) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_INFO, category, format, value, __FILE__, __FUNCTION__, __LINE__)

#define LTC_WARN_F1(category, format, value) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_WARN, category, format, value, __FILE__, __FUNCTION__, __LINE__)

#define LTC_ERROR_F1(category, format, value) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_ERROR, category, format, value, __FILE__, __FUNCTION__, __LINE__)

#define LTC_FATAL_F1(category, format, value) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_FATAL, category, format, value, __FILE__, __FUNCTION__, __LINE__)

// Simple printf-style macros without file/function/line info
#define LTCS_TRACE_F1(category, format, value) \
    Logger::GetInstance().Log(LogLevel::L_TRACE, category, format, value)

#define LTCS_DEBUG_F1(category, format, value) \
    Logger::GetInstance().Log(LogLevel::L_DEBUG, category, format, value)

#define LTCS_INFO_F1(category, format, value) \
    Logger::GetInstance().Log(LogLevel::L_INFO, category, format, value)

#define LTCS_WARN_F1(category, format, value) \
    Logger::GetInstance().Log(LogLevel::L_WARN, category, format, value)

#define LTCS_ERROR_F1(category, format, value) \
    Logger::GetInstance().Log(LogLevel::L_ERROR, category, format, value)

#define LTCS_FATAL_F1(category, format, value) \
    Logger::GetInstance().Log(LogLevel::L_FATAL, category, format, value)

// Printf-style macros with two parameters (with file/function/line info)
#define LTC_TRACE_F2(category, format, value1, value2) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_TRACE, category, format, value1, value2, __FILE__, __FUNCTION__, __LINE__)

#define LTC_DEBUG_F2(category, format, value1, value2) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_DEBUG, category, format, value1, value2, __FILE__, __FUNCTION__, __LINE__)

#define LTC_INFO_F2(category, format, value1, value2) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_INFO, category, format, value1, value2, __FILE__, __FUNCTION__, __LINE__)

#define LTC_WARN_F2(category, format, value1, value2) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_WARN, category, format, value1, value2, __FILE__, __FUNCTION__, __LINE__)

#define LTC_ERROR_F2(category, format, value1, value2) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_ERROR, category, format, value1, value2, __FILE__, __FUNCTION__, __LINE__)

#define LTC_FATAL_F2(category, format, value1, value2) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_FATAL, category, format, value1, value2, __FILE__, __FUNCTION__, __LINE__)

// Simple printf-style macros with two parameters (without source location)
#define LTCS_TRACE_F2(category, format, value1, value2) \
    Logger::GetInstance().Log(LogLevel::L_TRACE, category, format, value1, value2)

#define LTCS_DEBUG_F2(category, format, value1, value2) \
    Logger::GetInstance().Log(LogLevel::L_DEBUG, category, format, value1, value2)

#define LTCS_INFO_F2(category, format, value1, value2) \
    Logger::GetInstance().Log(LogLevel::L_INFO, category, format, value1, value2)

#define LTCS_WARN_F2(category, format, value1, value2) \
    Logger::GetInstance().Log(LogLevel::L_WARN, category, format, value1, value2)

#define LTCS_ERROR_F2(category, format, value1, value2) \
    Logger::GetInstance().Log(LogLevel::L_ERROR, category, format, value1, value2)

#define LTCS_FATAL_F2(category, format, value1, value2) \
    Logger::GetInstance().Log(LogLevel::L_FATAL, category, format, value1, value2)

// Printf-style macros with three parameters (with file/function/line info)
#define LTC_TRACE_F3(category, format, value1, value2, value3) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_TRACE, category, format, value1, value2, value3, __FILE__, __FUNCTION__, __LINE__)

#define LTC_DEBUG_F3(category, format, value1, value2, value3) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_DEBUG, category, format, value1, value2, value3, __FILE__, __FUNCTION__, __LINE__)

#define LTC_INFO_F3(category, format, value1, value2, value3) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_INFO, category, format, value1, value2, value3, __FILE__, __FUNCTION__, __LINE__)

#define LTC_WARN_F3(category, format, value1, value2, value3) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_WARN, category, format, value1, value2, value3, __FILE__, __FUNCTION__, __LINE__)

#define LTC_ERROR_F3(category, format, value1, value2, value3) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_ERROR, category, format, value1, value2, value3, __FILE__, __FUNCTION__, __LINE__)

#define LTC_FATAL_F3(category, format, value1, value2, value3) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_FATAL, category, format, value1, value2, value3, __FILE__, __FUNCTION__, __LINE__)

// Simple printf-style macros with three parameters (without source location)
#define LTCS_TRACE_F3(category, format, value1, value2, value3) \
    Logger::GetInstance().Log(LogLevel::L_TRACE, category, format, value1, value2, value3)

#define LTCS_DEBUG_F3(category, format, value1, value2, value3) \
    Logger::GetInstance().Log(LogLevel::L_DEBUG, category, format, value1, value2, value3)

#define LTCS_INFO_F3(category, format, value1, value2, value3) \
    Logger::GetInstance().Log(LogLevel::L_INFO, category, format, value1, value2, value3)

#define LTCS_WARN_F3(category, format, value1, value2, value3) \
    Logger::GetInstance().Log(LogLevel::L_WARN, category, format, value1, value2, value3)

#define LTCS_ERROR_F3(category, format, value1, value2, value3) \
    Logger::GetInstance().Log(LogLevel::L_ERROR, category, format, value1, value2, value3)

#define LTCS_FATAL_F3(category, format, value1, value2, value3) \
    Logger::GetInstance().Log(LogLevel::L_FATAL, category, format, value1, value2, value3)

// Template implementations for printf-style logging
#include <sstream>
#include <iomanip>

template<typename T>
void Logger::Log(LogLevel level, const std::string& category, const std::string& format, T value) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    // Simple string replacement - find %d, %s, %f, etc. and replace with value
    std::string message = format;
    std::ostringstream oss;
    oss << value;
    std::string valueStr = oss.str();
    
    // Find the first format specifier and replace it
    size_t pos = message.find('%');
    if (pos != std::string::npos && pos + 1 < message.length()) {
        char specifier = message[pos + 1];
        if (specifier == 'd' || specifier == 'i' || specifier == 'f' || specifier == 'g' || 
            specifier == 's' || specifier == 'c' || specifier == 'x' || specifier == 'X') {
            message.replace(pos, 2, valueStr);
        }
    }

    m_client->Log(level, category, message);
}

template<typename T>
void Logger::LogWithLocation(LogLevel level, const std::string& category, const std::string& format, T value,
                            const char* file, const char* function, int line) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    // Simple string replacement - find %d, %s, %f, etc. and replace with value
    std::string message = format;
    std::ostringstream oss;
    oss << value;
    std::string valueStr = oss.str();
    
    // Find the first format specifier and replace it
    size_t pos = message.find('%');
    if (pos != std::string::npos && pos + 1 < message.length()) {
        char specifier = message[pos + 1];
        if (specifier == 'd' || specifier == 'i' || specifier == 'f' || specifier == 'g' || 
            specifier == 's' || specifier == 'c' || specifier == 'x' || specifier == 'X') {
            message.replace(pos, 2, valueStr);
        }
    }

    m_client->Log(level, category, message, file, function, line);
}

// Template implementations for printf-style logging with two parameters
template<typename T1, typename T2>
void Logger::Log(LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    // Convert values to strings
    std::ostringstream oss1, oss2;
    oss1 << value1;
    oss2 << value2;
    std::string valueStr1 = oss1.str();
    std::string valueStr2 = oss2.str();
    
    // Replace format specifiers in order
    std::string message = format;
    size_t pos = 0;
    int replacements = 0;
    
    while ((pos = message.find('%', pos)) != std::string::npos && pos + 1 < message.length() && replacements < 2) {
        char specifier = message[pos + 1];
        if (specifier == 'd' || specifier == 'i' || specifier == 'f' || specifier == 'g' || 
            specifier == 's' || specifier == 'c' || specifier == 'x' || specifier == 'X') {
            std::string replacement = (replacements == 0) ? valueStr1 : valueStr2;
            message.replace(pos, 2, replacement);
            pos += replacement.length();
            replacements++;
        } else {
            pos++;
        }
    }

    m_client->Log(level, category, message);
}

template<typename T1, typename T2>
void Logger::LogWithLocation(LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2,
                            const char* file, const char* function, int line) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    // Convert values to strings
    std::ostringstream oss1, oss2;
    oss1 << value1;
    oss2 << value2;
    std::string valueStr1 = oss1.str();
    std::string valueStr2 = oss2.str();
    
    // Replace format specifiers in order
    std::string message = format;
    size_t pos = 0;
    int replacements = 0;
    
    while ((pos = message.find('%', pos)) != std::string::npos && pos + 1 < message.length() && replacements < 2) {
        char specifier = message[pos + 1];
        if (specifier == 'd' || specifier == 'i' || specifier == 'f' || specifier == 'g' || 
            specifier == 's' || specifier == 'c' || specifier == 'x' || specifier == 'X') {
            std::string replacement = (replacements == 0) ? valueStr1 : valueStr2;
            message.replace(pos, 2, replacement);
            pos += replacement.length();
            replacements++;
        } else {
            pos++;
        }
    }

    m_client->Log(level, category, message, file, function, line);
}

// Template implementations for printf-style logging with three parameters
template<typename T1, typename T2, typename T3>
void Logger::Log(LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2, T3 value3) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    // Convert values to strings
    std::ostringstream oss1, oss2, oss3;
    oss1 << value1;
    oss2 << value2;
    oss3 << value3;
    std::string valueStr1 = oss1.str();
    std::string valueStr2 = oss2.str();
    std::string valueStr3 = oss3.str();
    
    // Replace format specifiers in order
    std::string message = format;
    size_t pos = 0;
    int replacements = 0;
    
    while ((pos = message.find('%', pos)) != std::string::npos && pos + 1 < message.length() && replacements < 3) {
        char specifier = message[pos + 1];
        if (specifier == 'd' || specifier == 'i' || specifier == 'f' || specifier == 'g' || 
            specifier == 's' || specifier == 'c' || specifier == 'x' || specifier == 'X') {
            std::string replacement = (replacements == 0) ? valueStr1 : 
                                    (replacements == 1) ? valueStr2 : valueStr3;
            message.replace(pos, 2, replacement);
            pos += replacement.length();
            replacements++;
        } else {
            pos++;
        }
    }

    m_client->Log(level, category, message);
}

template<typename T1, typename T2, typename T3>
void Logger::LogWithLocation(LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2, T3 value3,
                            const char* file, const char* function, int line) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    // Convert values to strings
    std::ostringstream oss1, oss2, oss3;
    oss1 << value1;
    oss2 << value2;
    oss3 << value3;
    std::string valueStr1 = oss1.str();
    std::string valueStr2 = oss2.str();
    std::string valueStr3 = oss3.str();
    
    // Replace format specifiers in order
    std::string message = format;
    size_t pos = 0;
    int replacements = 0;
    
    while ((pos = message.find('%', pos)) != std::string::npos && pos + 1 < message.length() && replacements < 3) {
        char specifier = message[pos + 1];
        if (specifier == 'd' || specifier == 'i' || specifier == 'f' || specifier == 'g' || 
            specifier == 's' || specifier == 'c' || specifier == 'x' || specifier == 'X') {
            std::string replacement = (replacements == 0) ? valueStr1 : 
                                    (replacements == 1) ? valueStr2 : valueStr3;
            message.replace(pos, 2, replacement);
            pos += replacement.length();
            replacements++;
        } else {
            pos++;
        }
    }

    m_client->Log(level, category, message, file, function, line);
}
