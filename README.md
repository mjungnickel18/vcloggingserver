# VC++ Logging Library for Log2Console

A C++ library for sending and receiving log messages with [Log2Console](https://github.com/Statyk7/log2console) using the log4j XML format. Compatible with Visual C++ 2019.

## Features

- **Log2ConsoleServer**: TCP server that accepts connections from Log2Console
- **Log2ConsoleClient**: TCP client that sends logs to Log2Console  
- Supports log4j XML format (required by Log2Console)
- Optional plain text format for debugging
- Thread-safe with automatic reconnection
- Multiple log levels: TRACE, DEBUG, INFO, WARN, ERROR, FATAL
- No external dependencies (uses Windows sockets)

## Server Usage

```cpp
#include "Log2ConsoleServer.h"

// Create server that Log2Console can connect to
Log2ConsoleServer server(4445, true);

// Start the server
if (!server.Start()) {
    std::cerr << "Failed to start log server" << std::endl;
    return 1;
}

// Send log messages to connected Log2Console
server.Log(LogLevel::INFO, "MyApp", "Application started");
server.Log(LogLevel::L_ERROR, "Database", "Connection failed");

// Stop the server
server.Stop();
```

## Client Usage

```cpp
#include "Log2ConsoleClient.h"

// Create client that connects to Log2Console
Log2ConsoleClient client("localhost", 4445, true);

// Connect to Log2Console
if (!client.Connect()) {
    std::cerr << "Failed to connect" << std::endl;
    return 1;
}

// Send log messages
client.Log(LogLevel::INFO, "MyApp", "Application started");
client.Log(LogLevel::L_ERROR, "Database", "Connection failed");

// Disconnect
client.Disconnect();
```

## Building

Requires Visual C++ 2019 or later. The project uses Windows sockets (Winsock2) which is linked automatically via `#pragma comment(lib, "ws2_32.lib")`.

## Log2Console Configuration

### For Server Mode:
1. Open Log2Console
2. Add a new TCP receiver
3. Set the host to `localhost` (or your server IP)
4. Set the port to `4445` (or your configured port)
5. Start the receiver

### For Client Mode:
1. Your application connects to Log2Console's TCP receiver
2. Log2Console must be running first with TCP receiver active

## Files

- `Log2ConsoleCommon.h/cpp` - Shared formatting and log level definitions
- `Log2ConsoleServer.h/cpp` - TCP server implementation
- `Log2ConsoleClient.h/cpp` - TCP client implementation  
- `example.cpp` - Example demonstrating both server and client modes

## Note on ERROR Enum

The ERROR log level has been renamed to `L_ERROR` in the enum to avoid conflicts with Windows macros. The string representation remains "ERROR" for compatibility with Log2Console.