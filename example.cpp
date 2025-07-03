#include "Log2ConsoleUdpClient.h"
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
    client.Log(LogLevel::INFO, "UdpClientApp", "UDP client application started");
    client.Log(LogLevel::DEBUG, "Database", "Connecting to database...");
    client.Log(LogLevel::WARN, "Network", "High latency detected: 250ms");
    client.Log(LogLevel::L_ERROR, "FileSystem", "Configuration file not found");
    client.Log(LogLevel::FATAL, "Security", "Authentication failed");
    client.Log(LogLevel::TRACE, "Performance", "Method execution time: 42ms");
    
    // Demonstrate burst logging
    std::cout << "Sending burst of UDP messages..." << std::endl;
    for (int i = 0; i < 10; i++) {
        client.Log(LogLevel::INFO, "UdpBurstTest", "UDP burst message #" + std::to_string(i));
    }
    
    std::cout << "Press Enter to cleanup..." << std::endl;
    std::cin.get();
    
    client.Cleanup();
    std::cout << "UDP client cleaned up." << std::endl;
}

int main() {
    std::cout << "Log2Console C++ UDP Client Example" << std::endl;
    std::cout << "Starting UDP client..." << std::endl;
    
    RunUdpClient();
    
    return 0;
}