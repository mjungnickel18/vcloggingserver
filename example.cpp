#include "Log2ConsoleServer.h"
#include "Log2ConsoleClient.h"
#include <iostream>
#include <thread>
#include <chrono>

void RunServer() {
    std::cout << "=== LOG2CONSOLE SERVER EXAMPLE ===" << std::endl;
    
    // Create server with XML format enabled by default (for log2console compatibility)
    Log2ConsoleServer server(4445, true);
    
    server.SetClientConnectedCallback([](bool connected) {
        std::cout << "Client " << (connected ? "connected" : "disconnected") << std::endl;
    });
    
    if (!server.Start()) {
        std::cerr << "Failed to start log server" << std::endl;
        return;
    }
    
    std::cout << "Server listening on port 4445..." << std::endl;
    std::cout << "Connect with log2console TCP receiver on localhost:4445" << std::endl;
    std::cout << "Press Enter to start sending test logs..." << std::endl;
    std::cin.get();
    
    for (int i = 0; i < 5; i++) {
        server.Log(LogLevel::INFO, "ServerApp", "Server test message " + std::to_string(i));
        server.Log(LogLevel::L_ERROR, "ServerApp", "Server error test " + std::to_string(i));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "Press Enter to stop server..." << std::endl;
    std::cin.get();
    server.Stop();
}

void RunClient() {
    std::cout << "=== LOG2CONSOLE CLIENT EXAMPLE ===" << std::endl;
    
    // Create client that connects to log2console server
    Log2ConsoleClient client("localhost", 4445, true);
    
    std::cout << "Connecting to log2console server on localhost:4445..." << std::endl;
    if (!client.Connect()) {
        std::cerr << "Failed to connect to server" << std::endl;
        std::cout << "Make sure log2console is running with a TCP receiver on port 4445" << std::endl;
        return;
    }
    
    std::cout << "Connected! Sending test logs..." << std::endl;
    
    // Send various log messages
    client.Log(LogLevel::INFO, "ClientApp", "Client application started");
    client.Log(LogLevel::DEBUG, "Database", "Connecting to database...");
    client.Log(LogLevel::WARN, "Network", "High latency detected: 250ms");
    client.Log(LogLevel::L_ERROR, "FileSystem", "Configuration file not found");
    client.Log(LogLevel::FATAL, "Security", "Authentication failed");
    client.Log(LogLevel::TRACE, "Performance", "Method execution time: 42ms");
    
    // Demonstrate burst logging
    std::cout << "Sending burst of messages..." << std::endl;
    for (int i = 0; i < 10; i++) {
        client.Log(LogLevel::INFO, "BurstTest", "Burst message #" + std::to_string(i));
    }
    
    std::cout << "Press Enter to disconnect..." << std::endl;
    std::cin.get();
    
    client.Disconnect();
    std::cout << "Client disconnected." << std::endl;
}

int main() {
    std::cout << "Log2Console C++ Example" << std::endl;
    std::cout << "1. Run as Server (accepts log2console connections)" << std::endl;
    std::cout << "2. Run as Client (sends to log2console)" << std::endl;
    std::cout << "Choose (1 or 2): ";
    
    char choice;
    std::cin >> choice;
    std::cin.ignore(); // Clear newline
    
    if (choice == '1') {
        RunServer();
    } else if (choice == '2') {
        RunClient();
    } else {
        std::cout << "Invalid choice" << std::endl;
    }
    
    return 0;
}