#include "PlatformUtils.h"

#ifdef LTC_PLATFORM_WINDOWS
    #include <windows.h>
    #include <lmcons.h>  // For UNLEN
#endif

#ifdef LTC_PLATFORM_LINUX
    #include <limits.h>
    #include <pwd.h>
#endif

namespace PlatformUtils {

unsigned long GetCurrentThreadId() {
#ifdef LTC_PLATFORM_WINDOWS
    return static_cast<unsigned long>(::GetCurrentThreadId());
#else
    // Linux: Use syscall to get thread ID
    return static_cast<unsigned long>(syscall(SYS_gettid));
#endif
}

std::string GetHostName() {
#ifdef LTC_PLATFORM_WINDOWS
    char buffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
    if (GetComputerNameA(buffer, &size)) {
        return std::string(buffer);
    }
    return "localhost";
#else
    // Linux
    char buffer[HOST_NAME_MAX + 1];
    if (gethostname(buffer, HOST_NAME_MAX) == 0) {
        buffer[HOST_NAME_MAX] = '\0';
        return std::string(buffer);
    }
    return "localhost";
#endif
}

std::string GetUserName() {
#ifdef LTC_PLATFORM_WINDOWS
    char buffer[UNLEN + 1];
    DWORD size = UNLEN + 1;
    if (::GetUserNameA(buffer, &size)) {
        return std::string(buffer);
    }
    return "User";
#else
    // Linux
    struct passwd* pw = getpwuid(getuid());
    if (pw && pw->pw_name) {
        return std::string(pw->pw_name);
    }
    return "User";
#endif
}

} // namespace PlatformUtils