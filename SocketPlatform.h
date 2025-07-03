#pragma once

#include "PlatformUtils.h"

// Platform-specific socket includes and definitions
#ifdef LTC_PLATFORM_WINDOWS
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    
    typedef SOCKET socket_t;
    #define INVALID_SOCKET_VALUE INVALID_SOCKET
    #define SOCKET_ERROR_VALUE SOCKET_ERROR
    #define closesocket_platform closesocket
    
#else // Linux/Unix
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <errno.h>
    
    typedef int socket_t;
    #define INVALID_SOCKET_VALUE -1
    #define SOCKET_ERROR_VALUE -1
    #define closesocket_platform close
    #define SOCKET_ERROR -1
    #define INVALID_SOCKET -1
#endif

namespace SocketPlatform {
    // Initialize socket library (Windows only)
    bool Initialize();
    
    // Cleanup socket library (Windows only)
    void Cleanup();
    
    // Get last socket error
    int GetLastError();
    
    // Check if error is "would block"
    bool IsWouldBlock(int error);
}