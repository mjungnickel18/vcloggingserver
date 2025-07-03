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
    void Log(LogLevel level, const std::string& category, const std::string& message, 
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
    Logger::GetInstance().Log(LogLevel::L_TRACE, category, message, __FILE__, __FUNCTION__, __LINE__)

#define LTC_DEBUG(category, message) \
    Logger::GetInstance().Log(LogLevel::L_DEBUG, category, message, __FILE__, __FUNCTION__, __LINE__)

#define LTC_INFO(category, message) \
    Logger::GetInstance().Log(LogLevel::L_INFO, category, message, __FILE__, __FUNCTION__, __LINE__)

#define LTC_WARN(category, message) \
    Logger::GetInstance().Log(LogLevel::L_WARN, category, message, __FILE__, __FUNCTION__, __LINE__)

#define LTC_ERROR(category, message) \
    Logger::GetInstance().Log(LogLevel::L_ERROR, category, message, __FILE__, __FUNCTION__, __LINE__)

#define LTC_FATAL(category, message) \
    Logger::GetInstance().Log(LogLevel::L_FATAL, category, message, __FILE__, __FUNCTION__, __LINE__)

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