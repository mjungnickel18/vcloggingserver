#pragma once

// Prevent Windows.h from including winsock.h which conflicts with winsock2.h
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Include winsock2 before windows.h
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

// Standard library includes
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <functional>
#include <vector>
#include <iomanip>
#include <ctime>