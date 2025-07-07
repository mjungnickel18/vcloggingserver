#pragma once

#include "Log2ConsoleUdpClient.h"
#include <memory>
#include <mutex>
#include <unordered_map>

class Logger {
public:
    static Logger& GetInstance(); // Auto-initializes with default settings on first call

    // Initialize or reconfigure the logger with server details
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

    // Token-based logging to reduce repetition - only logs when message changes
    void LogToken(const std::string& tokenId, LogLevel level, const std::string& category, const std::string& message);
    void LogTokenWithLocation(const std::string& tokenId, LogLevel level, const std::string& category, const std::string& message,
                             const char* file, const char* function, int line);

    // Token-based printf-style log methods
    template<typename T>
    void LogToken(const std::string& tokenId, LogLevel level, const std::string& category, const std::string& format, T value);
    
    template<typename T>
    void LogTokenWithLocation(const std::string& tokenId, LogLevel level, const std::string& category, const std::string& format, T value,
                             const char* file, const char* function, int line);

    template<typename T1, typename T2>
    void LogToken(const std::string& tokenId, LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2);
    
    template<typename T1, typename T2>
    void LogTokenWithLocation(const std::string& tokenId, LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2,
                             const char* file, const char* function, int line);

    template<typename T1, typename T2, typename T3>
    void LogToken(const std::string& tokenId, LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2, T3 value3);
    
    template<typename T1, typename T2, typename T3>
    void LogTokenWithLocation(const std::string& tokenId, LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2, T3 value3,
                             const char* file, const char* function, int line);

private:
    Logger() = default;
    ~Logger() = default;

    // Delete copy and move constructors/operators
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    // Helper functions for fmt::format style formatting
    template<typename T>
    static std::string FormatValue(T value, const std::string& specifier);
    
    template<typename... Args>
    static std::string FormatMessage(const std::string& format, Args... args);
    
    template<typename Tuple>
    static std::string GetFormattedValueByIndex(const Tuple& tuple, int index, const std::string& specifier);
    
    template<typename Tuple, std::size_t... Is>
    static std::string GetFormattedValueByIndexImpl(const Tuple& tuple, int index, const std::string& specifier, std::index_sequence<Is...>);
    
    // C++14 compatibility helpers
    template<typename T>
    static void FormatHex(std::ostringstream& oss, T value, bool uppercase, bool prefix, std::true_type);
    template<typename T>
    static void FormatHex(std::ostringstream& oss, T value, bool uppercase, bool prefix, std::false_type);
    template<typename T>
    static void FormatPrecision(std::ostringstream& oss, T value, const std::string& specifier, std::true_type);
    template<typename T>
    static void FormatPrecision(std::ostringstream& oss, T value, const std::string& specifier, std::false_type);

    std::unique_ptr<Log2ConsoleUdpClient> m_client;
    mutable std::mutex m_mutex;
    bool m_initialized = false;
    
    // Token-based logging storage
    std::unordered_map<std::string, std::size_t> m_tokenHashes;
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


// Printf-style macros with automatic file/function/line info
#define LTC_TRACE_F1(category, format, value) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_TRACE, category, format, value, __FILE__, __FUNCTION__, __LINE__)

// Printf-style macro with manual file/function/line info
#define LTC_TRACE_F1_POS(category, format, value, file, function, line) \
    Logger::GetInstance().LogWithLocation(LogLevel::L_TRACE, category, format, value, file, function, line)

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


// Token-based logging macros (no parameters)
#define LTC_TRACE_TOKEN(tokenId, category, message) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_TRACE, category, message, __FILE__, __FUNCTION__, __LINE__)

#define LTC_DEBUG_TOKEN(tokenId, category, message) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_DEBUG, category, message, __FILE__, __FUNCTION__, __LINE__)

#define LTC_INFO_TOKEN(tokenId, category, message) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_INFO, category, message, __FILE__, __FUNCTION__, __LINE__)

#define LTC_WARN_TOKEN(tokenId, category, message) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_WARN, category, message, __FILE__, __FUNCTION__, __LINE__)

#define LTC_ERROR_TOKEN(tokenId, category, message) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_ERROR, category, message, __FILE__, __FUNCTION__, __LINE__)

#define LTC_FATAL_TOKEN(tokenId, category, message) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_FATAL, category, message, __FILE__, __FUNCTION__, __LINE__)


// Token-based logging macros with one parameter
#define LTC_TRACE_TOKEN_F1(tokenId, category, format, value) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_TRACE, category, format, value, __FILE__, __FUNCTION__, __LINE__)

#define LTC_DEBUG_TOKEN_F1(tokenId, category, format, value) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_DEBUG, category, format, value, __FILE__, __FUNCTION__, __LINE__)

#define LTC_INFO_TOKEN_F1(tokenId, category, format, value) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_INFO, category, format, value, __FILE__, __FUNCTION__, __LINE__)

#define LTC_WARN_TOKEN_F1(tokenId, category, format, value) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_WARN, category, format, value, __FILE__, __FUNCTION__, __LINE__)

#define LTC_ERROR_TOKEN_F1(tokenId, category, format, value) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_ERROR, category, format, value, __FILE__, __FUNCTION__, __LINE__)

#define LTC_FATAL_TOKEN_F1(tokenId, category, format, value) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_FATAL, category, format, value, __FILE__, __FUNCTION__, __LINE__)


// Token-based logging macros with two parameters
#define LTC_TRACE_TOKEN_F2(tokenId, category, format, value1, value2) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_TRACE, category, format, value1, value2, __FILE__, __FUNCTION__, __LINE__)

#define LTC_DEBUG_TOKEN_F2(tokenId, category, format, value1, value2) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_DEBUG, category, format, value1, value2, __FILE__, __FUNCTION__, __LINE__)

#define LTC_INFO_TOKEN_F2(tokenId, category, format, value1, value2) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_INFO, category, format, value1, value2, __FILE__, __FUNCTION__, __LINE__)

#define LTC_WARN_TOKEN_F2(tokenId, category, format, value1, value2) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_WARN, category, format, value1, value2, __FILE__, __FUNCTION__, __LINE__)

#define LTC_ERROR_TOKEN_F2(tokenId, category, format, value1, value2) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_ERROR, category, format, value1, value2, __FILE__, __FUNCTION__, __LINE__)

#define LTC_FATAL_TOKEN_F2(tokenId, category, format, value1, value2) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_FATAL, category, format, value1, value2, __FILE__, __FUNCTION__, __LINE__)


// Token-based logging macros with three parameters
#define LTC_TRACE_TOKEN_F3(tokenId, category, format, value1, value2, value3) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_TRACE, category, format, value1, value2, value3, __FILE__, __FUNCTION__, __LINE__)

#define LTC_DEBUG_TOKEN_F3(tokenId, category, format, value1, value2, value3) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_DEBUG, category, format, value1, value2, value3, __FILE__, __FUNCTION__, __LINE__)

#define LTC_INFO_TOKEN_F3(tokenId, category, format, value1, value2, value3) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_INFO, category, format, value1, value2, value3, __FILE__, __FUNCTION__, __LINE__)

#define LTC_WARN_TOKEN_F3(tokenId, category, format, value1, value2, value3) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_WARN, category, format, value1, value2, value3, __FILE__, __FUNCTION__, __LINE__)

#define LTC_ERROR_TOKEN_F3(tokenId, category, format, value1, value2, value3) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_ERROR, category, format, value1, value2, value3, __FILE__, __FUNCTION__, __LINE__)

#define LTC_FATAL_TOKEN_F3(tokenId, category, format, value1, value2, value3) \
    Logger::GetInstance().LogTokenWithLocation(tokenId, LogLevel::L_FATAL, category, format, value1, value2, value3, __FILE__, __FUNCTION__, __LINE__)


// Template implementations for fmt::format style logging
#include <sstream>
#include <iomanip>
#include <type_traits>
#include <tuple>
#include <utility>
#include <functional>

template<typename T>
void Logger::Log(LogLevel level, const std::string& category, const std::string& format, T value) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    std::string message = FormatMessage(format, value);
    m_client->Log(level, category, message);
}

template<typename T>
void Logger::LogWithLocation(LogLevel level, const std::string& category, const std::string& format, T value,
                            const char* file, const char* function, int line) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    std::string message = FormatMessage(format, value);
    m_client->Log(level, category, message, file, function, line);
}

// Template implementations for fmt::format style logging with two parameters
template<typename T1, typename T2>
void Logger::Log(LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    std::string message = FormatMessage(format, value1, value2);
    m_client->Log(level, category, message);
}

template<typename T1, typename T2>
void Logger::LogWithLocation(LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2,
                            const char* file, const char* function, int line) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    std::string message = FormatMessage(format, value1, value2);
    m_client->Log(level, category, message, file, function, line);
}

// Template implementations for fmt::format style logging with three parameters
template<typename T1, typename T2, typename T3>
void Logger::Log(LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2, T3 value3) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    std::string message = FormatMessage(format, value1, value2, value3);
    m_client->Log(level, category, message);
}

template<typename T1, typename T2, typename T3>
void Logger::LogWithLocation(LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2, T3 value3,
                            const char* file, const char* function, int line) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    std::string message = FormatMessage(format, value1, value2, value3);
    m_client->Log(level, category, message, file, function, line);
}

// FormatValue helper function implementation
template<typename T>
std::string Logger::FormatValue(T value, const std::string& specifier) {
    std::ostringstream oss;
    
    if (specifier.empty()) {
        // Default formatting: {}
        oss << value;
    } else if (specifier == "x") {
        // Hexadecimal lowercase: {x}
        FormatHex(oss, value, false, false, std::is_integral<T>());
    } else if (specifier == "X") {
        // Hexadecimal uppercase: {X}
        FormatHex(oss, value, true, false, std::is_integral<T>());
    } else if (specifier.substr(0, 2) == "#x") {
        // Hexadecimal with prefix: {#x}
        FormatHex(oss, value, false, true, std::is_integral<T>());
    } else if (specifier.substr(0, 2) == "#X") {
        // Hexadecimal with prefix uppercase: {#X}
        FormatHex(oss, value, true, true, std::is_integral<T>());
    } else if (specifier[0] == ':' && specifier[1] == '.') {
        // Precision formatting: {:.4}
        FormatPrecision(oss, value, specifier, std::is_floating_point<T>());
    } else {
        // Unknown specifier, use default
        oss << value;
    }
    
    return oss.str();
}

// FormatMessage unified helper function implementation
template<typename... Args>
std::string Logger::FormatMessage(const std::string& format, Args... args) {
    std::string message = format;
    size_t pos = 0;
    int replacements = 0;
    
    // Pack arguments into a tuple for indexed access
    auto argTuple = std::make_tuple(args...);
    constexpr size_t argCount = sizeof...(args);
    
    while ((pos = message.find('{', pos)) != std::string::npos && replacements < argCount) {
        size_t endPos = message.find('}', pos);
        if (endPos != std::string::npos) {
            std::string specifier = message.substr(pos + 1, endPos - pos - 1);
            std::string valueStr = GetFormattedValueByIndex(argTuple, replacements, specifier);
            message.replace(pos, endPos - pos + 1, valueStr);
            pos += valueStr.length();
            replacements++;
        } else {
            pos++;
        }
    }
    
    return message;
}

// Helper to get formatted value by index from tuple
template<typename Tuple>
std::string Logger::GetFormattedValueByIndex(const Tuple& tuple, int index, const std::string& specifier) {
    return GetFormattedValueByIndexImpl(tuple, index, specifier, std::make_index_sequence<std::tuple_size<Tuple>::value>{});
}

// Implementation detail for tuple index access  
template<typename Tuple, std::size_t... Is>
std::string Logger::GetFormattedValueByIndexImpl(const Tuple& tuple, int index, const std::string& specifier, std::index_sequence<Is...>) {
    std::string result;
    auto dummy = {(index == Is ? (result = FormatValue(std::get<Is>(tuple), specifier), 0) : 0)...};
    (void)dummy; // Suppress unused variable warning
    return result;
}

// C++14 compatibility helper implementations
template<typename T>
void Logger::FormatHex(std::ostringstream& oss, T value, bool uppercase, bool prefix, std::true_type) {
    if (prefix) {
        oss << (uppercase ? "0X" : "0x");
    }
    if (uppercase) {
        oss << std::hex << std::uppercase << value;
    } else {
        oss << std::hex << value;
    }
}

template<typename T>
void Logger::FormatHex(std::ostringstream& oss, T value, bool uppercase, bool prefix, std::false_type) {
    // Not an integral type, just output as default
    oss << value;
}

template<typename T>
void Logger::FormatPrecision(std::ostringstream& oss, T value, const std::string& specifier, std::true_type) {
    try {
        int precision = std::stoi(specifier.substr(2));
        oss << std::fixed << std::setprecision(precision) << value;
    } catch (...) {
        oss << value;
    }
}

template<typename T>
void Logger::FormatPrecision(std::ostringstream& oss, T value, const std::string& specifier, std::false_type) {
    // Not a floating point type, just output as default
    oss << value;
}

// Token-based template implementations
template<typename T>
void Logger::LogToken(const std::string& tokenId, LogLevel level, const std::string& category, const std::string& format, T value) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    std::string message = FormatMessage(format, value);
    
    // Calculate hash of the formatted message
    std::size_t messageHash = std::hash<std::string>{}(message);
    
    // Check if we've seen this message for this token before
    auto it = m_tokenHashes.find(tokenId);
    if (it != m_tokenHashes.end() && it->second == messageHash) {
        // Same message, don't log
        return;
    }
    
    // New or different message, update hash and log
    m_tokenHashes[tokenId] = messageHash;
    m_client->Log(level, category, message);
}

template<typename T>
void Logger::LogTokenWithLocation(const std::string& tokenId, LogLevel level, const std::string& category, const std::string& format, T value,
                                 const char* file, const char* function, int line) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    std::string message = FormatMessage(format, value);
    
    // Calculate hash of the formatted message
    std::size_t messageHash = std::hash<std::string>{}(message);
    
    // Check if we've seen this message for this token before
    auto it = m_tokenHashes.find(tokenId);
    if (it != m_tokenHashes.end() && it->second == messageHash) {
        // Same message, don't log
        return;
    }
    
    // New or different message, update hash and log
    m_tokenHashes[tokenId] = messageHash;
    m_client->Log(level, category, message, file, function, line);
}

template<typename T1, typename T2>
void Logger::LogToken(const std::string& tokenId, LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    std::string message = FormatMessage(format, value1, value2);
    
    // Calculate hash of the formatted message
    std::size_t messageHash = std::hash<std::string>{}(message);
    
    // Check if we've seen this message for this token before
    auto it = m_tokenHashes.find(tokenId);
    if (it != m_tokenHashes.end() && it->second == messageHash) {
        // Same message, don't log
        return;
    }
    
    // New or different message, update hash and log
    m_tokenHashes[tokenId] = messageHash;
    m_client->Log(level, category, message);
}

template<typename T1, typename T2>
void Logger::LogTokenWithLocation(const std::string& tokenId, LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2,
                                 const char* file, const char* function, int line) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    std::string message = FormatMessage(format, value1, value2);
    
    // Calculate hash of the formatted message
    std::size_t messageHash = std::hash<std::string>{}(message);
    
    // Check if we've seen this message for this token before
    auto it = m_tokenHashes.find(tokenId);
    if (it != m_tokenHashes.end() && it->second == messageHash) {
        // Same message, don't log
        return;
    }
    
    // New or different message, update hash and log
    m_tokenHashes[tokenId] = messageHash;
    m_client->Log(level, category, message, file, function, line);
}

template<typename T1, typename T2, typename T3>
void Logger::LogToken(const std::string& tokenId, LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2, T3 value3) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    std::string message = FormatMessage(format, value1, value2, value3);
    
    // Calculate hash of the formatted message
    std::size_t messageHash = std::hash<std::string>{}(message);
    
    // Check if we've seen this message for this token before
    auto it = m_tokenHashes.find(tokenId);
    if (it != m_tokenHashes.end() && it->second == messageHash) {
        // Same message, don't log
        return;
    }
    
    // New or different message, update hash and log
    m_tokenHashes[tokenId] = messageHash;
    m_client->Log(level, category, message);
}

template<typename T1, typename T2, typename T3>
void Logger::LogTokenWithLocation(const std::string& tokenId, LogLevel level, const std::string& category, const std::string& format, T1 value1, T2 value2, T3 value3,
                                 const char* file, const char* function, int line) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    std::string message = FormatMessage(format, value1, value2, value3);
    
    // Calculate hash of the formatted message
    std::size_t messageHash = std::hash<std::string>{}(message);
    
    // Check if we've seen this message for this token before
    auto it = m_tokenHashes.find(tokenId);
    if (it != m_tokenHashes.end() && it->second == messageHash) {
        // Same message, don't log
        return;
    }
    
    // New or different message, update hash and log
    m_tokenHashes[tokenId] = messageHash;
    m_client->Log(level, category, message, file, function, line);
}
