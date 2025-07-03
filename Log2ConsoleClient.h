#pragma once

#include "Log2ConsoleCommon.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>
#include <mutex>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>

#pragma comment(lib, "ws2_32.lib")

class Log2ConsoleClient {
public:
    Log2ConsoleClient(const std::string& serverHost = "localhost", int serverPort = 4445, bool useXmlFormat = true);
    ~Log2ConsoleClient();

    bool Connect();
    void Disconnect();
    bool IsConnected() const { return m_connected; }

    void Log(LogLevel level, const std::string& category, const std::string& message);
    void SetXmlFormat(bool useXml) { m_useXmlFormat = useXml; }
    void SetAutoReconnect(bool autoReconnect) { m_autoReconnect = autoReconnect; }
    void SetReconnectDelay(int seconds) { m_reconnectDelay = seconds; }

private:
    void WorkerThread();
    bool TryConnect();
    void SendMessage(const std::string& message);
    
    struct LogEntry {
        LogLevel level;
        std::string category;
        std::string message;
    };

    std::string m_serverHost;
    int m_serverPort;
    SOCKET m_socket;
    std::atomic<bool> m_connected;
    std::atomic<bool> m_running;
    bool m_useXmlFormat;
    bool m_autoReconnect;
    int m_reconnectDelay;
    
    std::thread m_workerThread;
    std::mutex m_queueMutex;
    std::condition_variable m_queueCv;
    std::queue<LogEntry> m_logQueue;
};