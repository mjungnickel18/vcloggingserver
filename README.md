# VC++ Logging Server for Log2Console

A C++ TCP server class that sends log messages to [Log2Console](https://github.com/Statyk7/log2console) using the log4j XML format. Compatible with Visual C++ 2019.

## Features

- TCP server listening on configurable port (default: 4445)
- Supports log4j XML format (required by Log2Console)
- Optional plain text format for debugging
- Thread-safe client handling
- Multiple log levels: TRACE, DEBUG, INFO, WARN, ERROR, FATAL
- No external dependencies (uses Windows sockets)

## Usage

```cpp
#include "Log2ConsoleServer.h"

// Create server with XML format (default)
Log2ConsoleServer server(4445, true);

// Start the server
if (!server.Start()) {
    std::cerr << "Failed to start log server" << std::endl;
    return 1;
}

// Send log messages
server.Log(LogLevel::INFO, "MyApp", "Application started");
server.Log(LogLevel::ERROR, "Database", "Connection failed");

// Stop the server
server.Stop();
```

## Building

Requires Visual C++ 2019 or later. The project uses Windows sockets (Winsock2) which is linked automatically via `#pragma comment(lib, "ws2_32.lib")`.

## Log2Console Configuration

1. Open Log2Console
2. Add a new TCP receiver
3. Set the host to `localhost` (or your server IP)
4. Set the port to `4445` (or your configured port)
5. Start the receiver

## Files

- `Log2ConsoleServer.h` - Header file with class definition
- `Log2ConsoleServer.cpp` - Implementation
- `example.cpp` - Example usage demonstrating various log levels