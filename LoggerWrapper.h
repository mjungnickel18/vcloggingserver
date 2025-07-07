#pragma once

// Check if logging is enabled via preprocessor define
#ifdef ENABLE_LTC_LOGGING

// Include full logger functionality
#include "Logger.h"

#else

// Include required headers for mock implementation
#include <string>

// Mock macros that do nothing when logging is disabled

// Enhanced macros (with file/function/line info) - do nothing
#define LTC_TRACE(category, message) do { } while(0)
#define LTC_DEBUG(category, message) do { } while(0)
#define LTC_INFO(category, message) do { } while(0)
#define LTC_WARN(category, message) do { } while(0)
#define LTC_ERROR(category, message) do { } while(0)
#define LTC_FATAL(category, message) do { } while(0)


// Printf-style macros (with file/function/line info) - do nothing
#define LTC_TRACE_F1(category, format, value) do { } while(0)
#define LTC_TRACE_F1_POS(category, format, value, file, function, line) do { } while(0)
#define LTC_DEBUG_F1(category, format, value) do { } while(0)
#define LTC_INFO_F1(category, format, value) do { } while(0)
#define LTC_WARN_F1(category, format, value) do { } while(0)
#define LTC_ERROR_F1(category, format, value) do { } while(0)
#define LTC_FATAL_F1(category, format, value) do { } while(0)


// Printf-style macros with two parameters (with file/function/line info) - do nothing
#define LTC_TRACE_F2(category, format, value1, value2) do { } while(0)
#define LTC_DEBUG_F2(category, format, value1, value2) do { } while(0)
#define LTC_INFO_F2(category, format, value1, value2) do { } while(0)
#define LTC_WARN_F2(category, format, value1, value2) do { } while(0)
#define LTC_ERROR_F2(category, format, value1, value2) do { } while(0)
#define LTC_FATAL_F2(category, format, value1, value2) do { } while(0)


// Printf-style macros with three parameters (with file/function/line info) - do nothing
#define LTC_TRACE_F3(category, format, value1, value2, value3) do { } while(0)
#define LTC_DEBUG_F3(category, format, value1, value2, value3) do { } while(0)
#define LTC_INFO_F3(category, format, value1, value2, value3) do { } while(0)
#define LTC_WARN_F3(category, format, value1, value2, value3) do { } while(0)
#define LTC_ERROR_F3(category, format, value1, value2, value3) do { } while(0)
#define LTC_FATAL_F3(category, format, value1, value2, value3) do { } while(0)


// Token-based logging macros (no parameters) - do nothing
#define LTC_TRACE_TOKEN(tokenId, category, message) do { } while(0)
#define LTC_DEBUG_TOKEN(tokenId, category, message) do { } while(0)
#define LTC_INFO_TOKEN(tokenId, category, message) do { } while(0)
#define LTC_WARN_TOKEN(tokenId, category, message) do { } while(0)
#define LTC_ERROR_TOKEN(tokenId, category, message) do { } while(0)
#define LTC_FATAL_TOKEN(tokenId, category, message) do { } while(0)


// Token-based logging macros with one parameter - do nothing
#define LTC_TRACE_TOKEN_F1(tokenId, category, format, value) do { } while(0)
#define LTC_DEBUG_TOKEN_F1(tokenId, category, format, value) do { } while(0)
#define LTC_INFO_TOKEN_F1(tokenId, category, format, value) do { } while(0)
#define LTC_WARN_TOKEN_F1(tokenId, category, format, value) do { } while(0)
#define LTC_ERROR_TOKEN_F1(tokenId, category, format, value) do { } while(0)
#define LTC_FATAL_TOKEN_F1(tokenId, category, format, value) do { } while(0)


// Token-based logging macros with two parameters - do nothing
#define LTC_TRACE_TOKEN_F2(tokenId, category, format, value1, value2) do { } while(0)
#define LTC_DEBUG_TOKEN_F2(tokenId, category, format, value1, value2) do { } while(0)
#define LTC_INFO_TOKEN_F2(tokenId, category, format, value1, value2) do { } while(0)
#define LTC_WARN_TOKEN_F2(tokenId, category, format, value1, value2) do { } while(0)
#define LTC_ERROR_TOKEN_F2(tokenId, category, format, value1, value2) do { } while(0)
#define LTC_FATAL_TOKEN_F2(tokenId, category, format, value1, value2) do { } while(0)


// Token-based logging macros with three parameters - do nothing
#define LTC_TRACE_TOKEN_F3(tokenId, category, format, value1, value2, value3) do { } while(0)
#define LTC_DEBUG_TOKEN_F3(tokenId, category, format, value1, value2, value3) do { } while(0)
#define LTC_INFO_TOKEN_F3(tokenId, category, format, value1, value2, value3) do { } while(0)
#define LTC_WARN_TOKEN_F3(tokenId, category, format, value1, value2, value3) do { } while(0)
#define LTC_ERROR_TOKEN_F3(tokenId, category, format, value1, value2, value3) do { } while(0)
#define LTC_FATAL_TOKEN_F3(tokenId, category, format, value1, value2, value3) do { } while(0)


// Mock Logger class for compatibility
namespace LoggerMock {
    class Logger {
    public:
        static Logger& GetInstance() {
            static Logger instance;
            return instance;
        }
        
        bool Initialize(const std::string& = "localhost", int = 4445, bool = true) { return true; }
        void Cleanup() { }
        bool IsInitialized() const { return false; }
        void SetXmlFormat(bool) { }
        
        // Mock log methods that do nothing
        template<typename... Args>
        void Log(Args&&...) { }
    };
}

// Make the mock Logger available in global scope
using Logger = LoggerMock::Logger;

#endif // ENABLE_LTC_LOGGING