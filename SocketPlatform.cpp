#include "SocketPlatform.h"

namespace SocketPlatform {

bool Initialize() {
#ifdef LTC_PLATFORM_WINDOWS
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
#else
    // No initialization needed on Linux
    return true;
#endif
}

void Cleanup() {
#ifdef LTC_PLATFORM_WINDOWS
    WSACleanup();
#else
    // No cleanup needed on Linux
#endif
}

int GetLastError() {
#ifdef LTC_PLATFORM_WINDOWS
    return WSAGetLastError();
#else
    return errno;
#endif
}

bool IsWouldBlock(int error) {
#ifdef LTC_PLATFORM_WINDOWS
    return error == WSAEWOULDBLOCK;
#else
    return error == EWOULDBLOCK || error == EAGAIN;
#endif
}

} // namespace SocketPlatform