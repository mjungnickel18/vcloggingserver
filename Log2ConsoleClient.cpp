#include "Log2ConsoleClient.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

struct LogEntry {
    LogLevel level;
    std::string category;
    std::string message;
};

class Log2ConsoleClient::Impl {
public:
    Impl(const std::string& serverHost, int serverPort, bool useXmlFormat)
        : m_serverHost(serverHost)
        , m_serverPort(serverPort)
        , m_socket(INVALID_SOCKET)
        , m_connected(false)
        , m_running(false)
        , m_useXmlFormat(useXmlFormat)
        , m_autoReconnect(true)
        , m_reconnectDelay(5)
        , m_connectionAttempted(false)
    {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    }

    ~Impl() {
        Disconnect();
        WSACleanup();
    }

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
    
    std::mutex m_connectionMutex;
    std::condition_variable m_connectionCv;
    std::atomic<bool> m_connectionAttempted;

    void WorkerThread();
    bool TryConnect();
    bool SendMessage(const std::string& message);
    bool Connect();
    void Disconnect();
};

Log2ConsoleClient::Log2ConsoleClient(const std::string& serverHost, int serverPort, bool useXmlFormat)
    : m_pImpl(std::make_unique<Impl>(serverHost, serverPort, useXmlFormat))
{
}

Log2ConsoleClient::~Log2ConsoleClient() = default;

Log2ConsoleClient::Log2ConsoleClient(Log2ConsoleClient&&) noexcept = default;
Log2ConsoleClient& Log2ConsoleClient::operator=(Log2ConsoleClient&&) noexcept = default;

bool Log2ConsoleClient::Connect() {
    return m_pImpl->Connect();
}

void Log2ConsoleClient::Disconnect() {
    m_pImpl->Disconnect();
}

bool Log2ConsoleClient::IsConnected() const {
    return m_pImpl->m_connected;
}

void Log2ConsoleClient::Log(LogLevel level, const std::string& category, const std::string& message) {
    if (!m_pImpl->m_running) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(m_pImpl->m_queueMutex);
        m_pImpl->m_logQueue.push({level, category, message});
    }
    m_pImpl->m_queueCv.notify_one();
}

void Log2ConsoleClient::SetXmlFormat(bool useXml) {
    m_pImpl->m_useXmlFormat = useXml;
}

void Log2ConsoleClient::SetAutoReconnect(bool autoReconnect) {
    m_pImpl->m_autoReconnect = autoReconnect;
}

void Log2ConsoleClient::SetReconnectDelay(int seconds) {
    m_pImpl->m_reconnectDelay = seconds;
}

// Implementation methods
bool Log2ConsoleClient::Impl::Connect() {
    if (m_connected) {
        return true;
    }

    if (!m_running) {
        m_connectionAttempted = false;
        m_running = true;
        m_workerThread = std::thread(&Impl::WorkerThread, this);
    }

    // Wait for the worker thread to make at least one connection attempt
    std::unique_lock<std::mutex> lock(m_connectionMutex);
    m_connectionCv.wait(lock, [this] { return m_connectionAttempted.load(); });
    
    return m_connected;
}

void Log2ConsoleClient::Impl::Disconnect() {
    m_running = false;
    m_autoReconnect = false;
    
    if (m_socket != INVALID_SOCKET) {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }
    
    m_connected = false;
    m_connectionAttempted = false;
    m_queueCv.notify_all();
    m_connectionCv.notify_all();
    
    if (m_workerThread.joinable()) {
        m_workerThread.join();
    }
}

void Log2ConsoleClient::Impl::WorkerThread() {
    while (m_running) {
        if (!m_connected) {
            bool connectionResult = TryConnect();
            
            // Signal that a connection attempt has been made
            if (!m_connectionAttempted.load()) {
                {
                    std::lock_guard<std::mutex> lock(m_connectionMutex);
                    m_connectionAttempted = true;
                }
                m_connectionCv.notify_one();
            }
            
            if (connectionResult) {
                m_connected = true;
            } else if (m_autoReconnect) {
                std::this_thread::sleep_for(std::chrono::seconds(m_reconnectDelay));
                continue;
            } else {
                break;
            }
        }

        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_queueCv.wait(lock, [this] { return !m_logQueue.empty() || !m_running; });

        while (!m_logQueue.empty() && m_connected) {
            LogEntry entry = m_logQueue.front();
            m_logQueue.pop();
            lock.unlock();

            std::string formattedMessage = m_useXmlFormat
                ? Log2ConsoleFormatter::FormatLog4jXml(entry.level, entry.category, entry.message)
                : Log2ConsoleFormatter::FormatPlainText(entry.level, entry.category, entry.message);

            // Try to send the message
            if (!SendMessage(formattedMessage)) {
                // If send failed, put message back in queue for retry
                lock.lock();
                m_logQueue.push(entry);
                break; // Exit the loop to trigger reconnection
            }
            
            lock.lock();
        }
    }
}

bool Log2ConsoleClient::Impl::TryConnect() {
    // Use getaddrinfo for better compatibility and IPv6 support
    struct addrinfo hints{};
    struct addrinfo* result = nullptr;
    
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    std::string portStr = std::to_string(m_serverPort);
    int res = getaddrinfo(m_serverHost.c_str(), portStr.c_str(), &hints, &result);
    if (res != 0) {
        return false;
    }
    
    // Try to connect using the resolved address
    for (struct addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (m_socket == INVALID_SOCKET) {
            continue;
        }
        
        if (connect(m_socket, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen)) != SOCKET_ERROR) {
            // Disable Nagle's algorithm to ensure immediate transmission
            int flag = 1;
            setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
            
            freeaddrinfo(result);
            return true;
        }
        
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }
    
    freeaddrinfo(result);
    return false;
}

bool Log2ConsoleClient::Impl::SendMessage(const std::string& message) {
    if (m_socket == INVALID_SOCKET || !m_connected) {
        return false;
    }

    int totalSent = 0;
    int messageLen = static_cast<int>(message.length());

    while (totalSent < messageLen) {
        int sent = send(m_socket, message.c_str() + totalSent, messageLen - totalSent, 0);
        if (sent == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if (error == WSAEWOULDBLOCK) {
                // Socket buffer is full, wait a bit and retry
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            // Other errors mean connection is lost
            closesocket(m_socket);
            m_socket = INVALID_SOCKET;
            m_connected = false;
            return false;
        }
        totalSent += sent;
    }
    
    // The socket is already configured with TCP_NODELAY in TryConnect()
    // so messages are sent immediately without buffering
    
    return true;
}