// Example demonstrating the LoggerWrapper usage
//
// To enable logging: #define ENABLE_LTC_LOGGING before including LoggerWrapper.h
// To disable logging: Don't define ENABLE_LTC_LOGGING (or #undef it)

// Uncomment the next line to enable logging
// #define ENABLE_LTC_LOGGING

#include "LoggerWrapper.h"
#include <iostream>

void TestLogging() {
    std::cout << "=== LOGGER WRAPPER EXAMPLE ===" << std::endl;
    
#ifdef ENABLE_LTC_LOGGING
    std::cout << "Logging is ENABLED - initializing logger..." << std::endl;
    
    // Initialize the logger (only works when logging is enabled)
    if (!Logger::GetInstance().Initialize("localhost", 4445, true)) {
        std::cerr << "Failed to initialize logger" << std::endl;
        return;
    }
    
    std::cout << "Logger initialized successfully!" << std::endl;
#else
    std::cout << "Logging is DISABLED - all log calls will be no-ops" << std::endl;
#endif
    
    // These logging calls work regardless of whether logging is enabled or disabled
    LTC_INFO("TestApp", "Application started");
    LTC_DEBUG("Database", "Connecting to database...");
    LTC_WARN("Network", "High latency detected");
    LTC_ERROR("FileSystem", "File not found");
    LTC_FATAL("Security", "Authentication failed");
    
    // Additional logging examples
    LTC_INFO("TestApp", "Additional log message");
    LTC_WARN("TestApp", "Warning message example");
    
    // Direct Logger usage (commented out - use wrapper macros instead)
    // Logger::GetInstance().Log(LogLevel::L_INFO, "DirectCall", "Direct logger call");
    
    std::cout << "All logging calls completed" << std::endl;
    
#ifdef ENABLE_LTC_LOGGING
    std::cout << "Cleaning up logger..." << std::endl;
    Logger::GetInstance().Cleanup();
#endif
}

int main() {
    std::cout << "LoggerWrapper Example" << std::endl;
    
#ifdef ENABLE_LTC_LOGGING
    std::cout << "Compiled WITH logging support" << std::endl;
#else
    std::cout << "Compiled WITHOUT logging support (all logs are no-ops)" << std::endl;
#endif
    
    TestLogging();
    
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    
    return 0;
}