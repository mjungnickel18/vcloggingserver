#pragma once

#include <string>

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
    #define LTC_PLATFORM_WINDOWS
#elif defined(__linux__)
    #define LTC_PLATFORM_LINUX
#else
    #error "Unsupported platform"
#endif

// Platform-specific includes
#ifdef LTC_PLATFORM_WINDOWS
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#else
    #include <unistd.h>
    #include <sys/syscall.h>
    #include <pthread.h>
#endif

namespace PlatformUtils {
    // Get current thread ID
    unsigned long GetCurrentThreadId();
    
    // Get hostname
    std::string GetHostName();
    
    // Get username
    std::string GetUserName();
}