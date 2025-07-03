#include "Log2ConsoleServer.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // Create server with XML format enabled by default (for log2console compatibility)
    Log2ConsoleServer server(4445, true);
    
    // Optionally switch to plain text format
    // server.SetXmlFormat(false);
    
    server.SetClientConnectedCallback([](bool connected) {
        std::cout << "Client " << (connected ? "connected" : "disconnected") << std::endl;
    });
    
    if (!server.Start()) {
        std::cerr << "Failed to start log server" << std::endl;
        return 1;
    }
    
    std::cout << "Log2Console server listening on port 4445..." << std::endl;
    std::cout << "Server is sending log4j XML format (compatible with log2console)" << std::endl;
    std::cout << "Connect with log2console TCP receiver on localhost:4445" << std::endl;
    std::cout << "Press Enter to start sending test logs..." << std::endl;
    std::cin.get();
    
    int counter = 0;
    while (server.IsRunning()) {
        server.Log(LogLevel::INFO, "Application", "Application started successfully");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        server.Log(LogLevel::DEBUG, "Database", "Connecting to database...");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        server.Log(LogLevel::WARN, "Network", "Connection timeout detected");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        server.Log(LogLevel::ERROR, "FileSystem", "Failed to open config file");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        server.Log(LogLevel::TRACE, "Performance", "Function execution took 125ms");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        counter++;
        if (counter >= 3) {
            std::cout << "Press Enter to stop..." << std::endl;
            std::cin.get();
            break;
        }
    }
    
    server.Stop();
    return 0;
}