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

// Simple macros (without source location) - do nothing
#define LTCS_TRACE(category, message) do { } while(0)
#define LTCS_DEBUG(category, message) do { } while(0)
#define LTCS_INFO(category, message) do { } while(0)
#define LTCS_WARN(category, message) do { } while(0)
#define LTCS_ERROR(category, message) do { } while(0)
#define LTCS_FATAL(category, message) do { } while(0)

// Printf-style macros (with file/function/line info) - do nothing
#define LTC_TRACE_F1(category, format, value) do { } while(0)
#define LTC_DEBUG_F1(category, format, value) do { } while(0)
#define LTC_INFO_F1(category, format, value) do { } while(0)
#define LTC_WARN_F1(category, format, value) do { } while(0)
#define LTC_ERROR_F1(category, format, value) do { } while(0)
#define LTC_FATAL_F1(category, format, value) do { } while(0)

// Simple printf-style macros (without source location) - do nothing
#define LTCS_TRACE_F1(category, format, value) do { } while(0)
#define LTCS_DEBUG_F1(category, format, value) do { } while(0)
#define LTCS_INFO_F1(category, format, value) do { } while(0)
#define LTCS_WARN_F1(category, format, value) do { } while(0)
#define LTCS_ERROR_F1(category, format, value) do { } while(0)
#define LTCS_FATAL_F1(category, format, value) do { } while(0)

// Printf-style macros with two parameters (with file/function/line info) - do nothing
#define LTC_TRACE_F2(category, format, value1, value2) do { } while(0)
#define LTC_DEBUG_F2(category, format, value1, value2) do { } while(0)
#define LTC_INFO_F2(category, format, value1, value2) do { } while(0)
#define LTC_WARN_F2(category, format, value1, value2) do { } while(0)
#define LTC_ERROR_F2(category, format, value1, value2) do { } while(0)
#define LTC_FATAL_F2(category, format, value1, value2) do { } while(0)

// Simple printf-style macros with two parameters (without source location) - do nothing
#define LTCS_TRACE_F2(category, format, value1, value2) do { } while(0)
#define LTCS_DEBUG_F2(category, format, value1, value2) do { } while(0)
#define LTCS_INFO_F2(category, format, value1, value2) do { } while(0)
#define LTCS_WARN_F2(category, format, value1, value2) do { } while(0)
#define LTCS_ERROR_F2(category, format, value1, value2) do { } while(0)
#define LTCS_FATAL_F2(category, format, value1, value2) do { } while(0)

// Printf-style macros with three parameters (with file/function/line info) - do nothing
#define LTC_TRACE_F3(category, format, value1, value2, value3) do { } while(0)
#define LTC_DEBUG_F3(category, format, value1, value2, value3) do { } while(0)
#define LTC_INFO_F3(category, format, value1, value2, value3) do { } while(0)
#define LTC_WARN_F3(category, format, value1, value2, value3) do { } while(0)
#define LTC_ERROR_F3(category, format, value1, value2, value3) do { } while(0)
#define LTC_FATAL_F3(category, format, value1, value2, value3) do { } while(0)

// Simple printf-style macros with three parameters (without source location) - do nothing
#define LTCS_TRACE_F3(category, format, value1, value2, value3) do { } while(0)
#define LTCS_DEBUG_F3(category, format, value1, value2, value3) do { } while(0)
#define LTCS_INFO_F3(category, format, value1, value2, value3) do { } while(0)
#define LTCS_WARN_F3(category, format, value1, value2, value3) do { } while(0)
#define LTCS_ERROR_F3(category, format, value1, value2, value3) do { } while(0)
#define LTCS_FATAL_F3(category, format, value1, value2, value3) do { } while(0)

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