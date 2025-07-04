# Cross-Platform UDP Logging Library for Log2Console

A C++ UDP client library for sending log messages to [Log2Console](https://github.com/Statyk7/log2console) using the log4j XML format. Compatible with Windows (Visual C++ 2019) and Linux (GCC).

## Features

- **Log2ConsoleUdpClient**: UDP client that sends logs to Log2Console  
- Supports log4j XML format (required by Log2Console)
- Optional plain text format for debugging
- Thread-safe UDP message sending
- Multiple log levels: L_TRACE, L_DEBUG, L_INFO, L_WARN, L_ERROR, L_FATAL
- Cross-platform: Windows (Winsock2) and Linux (BSD sockets)
- No external dependencies
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

### Using CMake (Recommended)

#### Windows
```bash
# Using build script
build.bat              # Build static library
build.bat --shared     # Build shared library (DLL)
build.bat --debug      # Build in debug mode
build.bat --vs2022     # Use Visual Studio 2022

# Or manually
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
```

#### Linux
```bash
# Using build script
./build.sh             # Build static library
./build.sh --shared    # Build shared library (.so)
./build.sh --debug     # Build in debug mode

# Or manually
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Manual Build (without CMake)

#### Windows
- Requires Visual C++ 2019 or later with C++14 support
- Compile all .cpp files into a library or directly into your project

#### Linux
- Requires GCC with C++14 support
- Compile with: `g++ -std=c++14 -pthread *.cpp -o logger`

### Design Features
- **PIMPL idiom**: Minimal header dependencies for faster compilation
- **C++14 compliant**: Uses modern C++ features while maintaining compatibility
- **Move semantics**: Efficient resource management
- **Thread-safe**: Proper synchronization for multi-threaded environments

## Using in Your CMake Project

After installing the library, you can use it in your CMake project:

```cmake
find_package(log2console REQUIRED)

add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE log2console::log2console)

# Optional: Enable logging (or control via compiler flags)
target_compile_definitions(myapp PRIVATE ENABLE_LTC_LOGGING)
```

In your code:
```cpp
#include <log2console/LoggerWrapper.h>

int main() {
    Logger::GetInstance().Initialize("localhost", 4445, true);
    LTC_INFO("MyApp", "Hello from Log2Console!");
    Logger::GetInstance().Cleanup();
    return 0;
}
```

## Log2Console Configuration

### For UDP Client Mode:
1. Open Log2Console
2. Add a new UDP receiver
3. Set the host to `localhost` (or your client IP)
4. Set the port to `4445` (or your configured port)
5. Start the receiver
6. Your application will send UDP messages to Log2Console

## Conditional Logging

The library includes `LoggerWrapper.h` for conditional compilation of logging functionality:

```cpp
// Enable logging
#define ENABLE_LTC_LOGGING
#include "LoggerWrapper.h"

// Use logging macros - will send to Log2Console
LTC_INFO("MyApp", "This message will be logged");
```

```cpp
// Disable logging (don't define ENABLE_LTC_LOGGING)
#include "LoggerWrapper.h"

// Use same logging macros - will be compiled out (no-ops)
LTC_INFO("MyApp", "This message will be ignored");
```

**Benefits:**
- Zero runtime overhead when logging is disabled
- Same code works with logging enabled/disabled
- Easy to toggle logging for release builds
- No need to remove log statements from code

## Files

- `Log2ConsoleCommon.h/cpp` - Shared formatting and log level definitions
- `Log2ConsoleUdpClient.h/cpp` - UDP client implementation
- `Logger.h/cpp` - Singleton logger with convenient macros
- `LoggerWrapper.h` - Conditional logging wrapper (enable/disable via define)
- `PlatformUtils.h/cpp` - Platform abstraction for thread ID, hostname, username
- `SocketPlatform.h/cpp` - Platform abstraction for socket operations
- `example.cpp` - Example demonstrating UDP client and singleton logger
- `example_wrapper.cpp` - Example demonstrating conditional logging

## Note on Log Level Enum

All log levels have been prefixed with `L_` (L_TRACE, L_DEBUG, L_INFO, L_WARN, L_ERROR, L_FATAL) to avoid conflicts with Windows macros and other common definitions. The string representations remain standard (TRACE, DEBUG, INFO, etc.) for compatibility with Log2Console.