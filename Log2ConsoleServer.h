#pragma once

#include "Log2ConsoleCommon.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <functional>

#pragma comment(lib, "ws2_32.lib")

class Log2ConsoleServer {
public:
    Log2ConsoleServer(int port = 4445, bool useXmlFormat = true);
    ~Log2ConsoleServer();

    bool Start();
    void Stop();
    bool IsRunning() const { return m_running; }

    void Log(LogLevel level, const std::string& category, const std::string& message);
    void SetClientConnectedCallback(std::function<void(bool)> callback) { m_clientConnectedCallback = callback; }
    void SetXmlFormat(bool useXml) { m_useXmlFormat = useXml; }

private:
    void AcceptThread();
    void SendLog2ConsoleMessage(LogLevel level, const std::string& category, const std::string& message);

    int m_port;
    SOCKET m_serverSocket;
    SOCKET m_clientSocket;
    std::atomic<bool> m_running;
    std::thread m_acceptThread;
    std::mutex m_socketMutex;
    std::function<void(bool)> m_clientConnectedCallback;
    bool m_useXmlFormat;
};