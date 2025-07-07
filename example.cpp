#include "Log2ConsoleUdpClient.h"
#include "Logger.h"
#include <iostream>
#include <thread>
#include <chrono>


void RunUdpClient() {
    std::cout << "=== LOG2CONSOLE UDP CLIENT EXAMPLE ===" << std::endl;
    
    // Create UDP client that sends to log2console server
    Log2ConsoleUdpClient client("localhost", 4445, true);
    
    std::cout << "Initializing UDP client for localhost:4445..." << std::endl;
    if (!client.Initialize()) {
        std::cerr << "Failed to initialize UDP client" << std::endl;
        std::cout << "Make sure log2console is running with a UDP receiver on port 4445" << std::endl;
        return;
    }
    
    std::cout << "UDP client initialized! Sending test logs..." << std::endl;
    
    // Send various log messages
    client.Log(LogLevel::L_INFO, "UdpClientApp", "UDP client application started");
    client.Log(LogLevel::L_DEBUG, "Database", "Connecting to database...");
    client.Log(LogLevel::L_WARN, "Network", "High latency detected: 250ms");
    client.Log(LogLevel::L_ERROR, "FileSystem", "Configuration file not found");
    client.Log(LogLevel::L_FATAL, "Security", "Authentication failed");
    client.Log(LogLevel::L_TRACE, "Performance", "Method execution time: 42ms");
    
    // Demonstrate burst logging
    std::cout << "Sending burst of UDP messages..." << std::endl;
    for (int i = 0; i < 10; i++) {
        client.Log(LogLevel::L_INFO, "UdpBurstTest", "UDP burst message #" + std::to_string(i));
    }
    
    std::cout << "Press Enter to cleanup..." << std::endl;
    std::cin.get();
    
    client.Cleanup();
    std::cout << "UDP client cleaned up." << std::endl;
}

void RunSingletonLogger() {
    std::cout << "=== SINGLETON LOGGER EXAMPLE ===" << std::endl;
    
    // Initialize the singleton logger
    std::cout << "Initializing singleton logger for localhost:4445..." << std::endl;
    if (!Logger::GetInstance().Initialize("localhost", 4445, true)) {
        std::cerr << "Failed to initialize singleton logger" << std::endl;
        std::cout << "Make sure log2console is running with a UDP receiver on port 4445" << std::endl;
        return;
    }
    
    std::cout << "Singleton logger initialized! Sending test logs..." << std::endl;
    
    // Test basic logging through direct Logger calls
    Logger::GetInstance().Log(LogLevel::L_INFO, "SingletonApp", "Singleton logger application started");
    Logger::GetInstance().Log(LogLevel::L_DEBUG, "Database", "Connecting to database...");
    Logger::GetInstance().Log(LogLevel::L_WARN, "Network", "High latency detected: 250ms");
    Logger::GetInstance().Log(LogLevel::L_ERROR, "FileSystem", "Configuration file not found");
    Logger::GetInstance().Log(LogLevel::L_FATAL, "Security", "Authentication failed");
    Logger::GetInstance().Log(LogLevel::L_TRACE, "Performance", "Method execution time: 42ms");
    
    std::cout << "Testing enhanced logging with file/function/line info..." << std::endl;
    
    // Test enhanced logging macros (with file/function/line info)
    LTC_INFO("SingletonApp", "Enhanced log with source location");
    LTC_DEBUG("Database", "Debug message with location info");
    LTC_WARN("Network", "Warning with source tracking");
    LTC_ERROR("FileSystem", "Error with detailed location");
    LTC_FATAL("Security", "Fatal error with source info");
    LTC_TRACE("Performance", "Trace with location details");
    
    // Demonstrate burst logging
    std::cout << "Sending burst of singleton messages..." << std::endl;
    for (int i = 0; i < 5; i++) {
        LTC_INFO("BurstTest", "Singleton burst message #" + std::to_string(i));
    }
    
    std::cout << "Press Enter to cleanup..." << std::endl;
    std::cin.get();
    
    Logger::GetInstance().Cleanup();
    std::cout << "Singleton logger cleaned up." << std::endl;
}

int main() {
    std::cout << "Log2Console C++ Examples" << std::endl;
    std::cout << "1. Run UDP Client Example" << std::endl;
    std::cout << "2. Run Singleton Logger Example" << std::endl;
    std::cout << "Choose (1 or 2): ";
    
    char choice;
    std::cin >> choice;
    std::cin.ignore(); // Clear newline
    
    if (choice == '1') {
        RunUdpClient();
    } else if (choice == '2') {
        RunSingletonLogger();
    } else {
        std::cout << "Invalid choice, running singleton logger example..." << std::endl;
        RunSingletonLogger();
    }
    
    return 0;
}