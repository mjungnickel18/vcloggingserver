// Example application using log2console library
#define ENABLE_LTC_LOGGING  // Enable logging
#include <log2console/LoggerWrapper.h>
#include <iostream>

void ProcessData(int value) {
    LTC_DEBUG("Processing", "Starting data processing for value: " + std::to_string(value));
    
    if (value < 0) {
        LTC_ERROR("Processing", "Invalid negative value: " + std::to_string(value));
        return;
    }
    
    // Simulate some work
    LTC_INFO("Processing", "Processing value: " + std::to_string(value));
    
    if (value > 100) {
        LTC_WARN("Processing", "Value exceeds recommended range: " + std::to_string(value));
    }
    
    LTC_DEBUG("Processing", "Data processing completed");
}

int main() {
    std::cout << "Log2Console Library Example Application" << std::endl;
    
    // Initialize the logger
    if (!Logger::GetInstance().Initialize("localhost", 4445, true)) {
        std::cerr << "Failed to initialize logger!" << std::endl;
        // Continue anyway - logging will be no-op
    }
    
    LTC_INFO("Application", "Application started");
    
    // Process some data
    ProcessData(50);
    ProcessData(-10);
    ProcessData(150);
    
    // Log with source location
    LTC_TRACE("Application", "This message includes file/function/line info");
    
    LTC_INFO("Application", "Application shutting down");
    
    // Cleanup
    Logger::GetInstance().Cleanup();
    
    return 0;
}