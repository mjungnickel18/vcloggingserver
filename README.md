# VC++ UDP Logging Library for Log2Console

A C++ UDP client library for sending log messages to [Log2Console](https://github.com/Statyk7/log2console) using the log4j XML format. Compatible with Visual C++ 2019.

## Features

- **Log2ConsoleUdpClient**: UDP client that sends logs to Log2Console  
- Supports log4j XML format (required by Log2Console)
- Optional plain text format for debugging
- Thread-safe UDP message sending
- Multiple log levels: L_TRACE, L_DEBUG, L_INFO, L_WARN, L_ERROR, L_FATAL
- No external dependencies (uses Windows sockets)
- Fire-and-forget UDP messaging for high performance

## UDP Client Usage

```cpp
#include "Log2ConsoleUdpClient.h"

// Create UDP client that sends to Log2Console
Log2ConsoleUdpClient client("localhost", 4445, true);

// Initialize the UDP client
if (!client.Initialize()) {
    std::cerr << "Failed to initialize UDP client" << std::endl;
    return 1;
}

// Send log messages (fire-and-forget)
client.Log(LogLevel::L_INFO, "MyApp", "Application started");
client.Log(LogLevel::L_ERROR, "Database", "Connection failed");

// Cleanup
client.Cleanup();
```

## Building

Requires Visual C++ 2019 or later with C++14 support. The project uses Windows sockets (Winsock2) which is linked automatically via `#pragma comment(lib, "ws2_32.lib")`.

### Design Features
- **PIMPL idiom**: Minimal header dependencies for faster compilation
- **C++14 compliant**: Uses modern C++ features while maintaining compatibility
- **Move semantics**: Efficient resource management
- **Thread-safe**: Proper synchronization for multi-threaded environments

## Log2Console Configuration

### For UDP Client Mode:
1. Open Log2Console
2. Add a new UDP receiver
3. Set the host to `localhost` (or your client IP)
4. Set the port to `4445` (or your configured port)
5. Start the receiver
6. Your application will send UDP messages to Log2Console

## Files

- `Log2ConsoleCommon.h/cpp` - Shared formatting and log level definitions
- `Log2ConsoleUdpClient.h/cpp` - UDP client implementation  
- `example.cpp` - Example demonstrating UDP client usage

## Note on Log Level Enum

All log levels have been prefixed with `L_` (L_TRACE, L_DEBUG, L_INFO, L_WARN, L_ERROR, L_FATAL) to avoid conflicts with Windows macros and other common definitions. The string representations remain standard (TRACE, DEBUG, INFO, etc.) for compatibility with Log2Console.