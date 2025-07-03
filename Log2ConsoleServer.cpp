#include "Log2ConsoleServer.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <thread>
#include <atomic>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")

class Log2ConsoleServer::Impl {
public:
    Impl(int port, bool useXmlFormat)
        : m_port(port)
        , m_serverSocket(INVALID_SOCKET)
        , m_clientSocket(INVALID_SOCKET)
        , m_running(false)
        , m_useXmlFormat(useXmlFormat)
    {
    }

    ~Impl() {
        Stop();
    }

    int m_port;
    SOCKET m_serverSocket;
    SOCKET m_clientSocket;
    std::atomic<bool> m_running;
    std::thread m_acceptThread;
    std::mutex m_socketMutex;
    std::function<void(bool)> m_clientConnectedCallback;
    bool m_useXmlFormat;

    void AcceptThread();
    void SendLog2ConsoleMessage(LogLevel level, const std::string& category, const std::string& message);
    bool Start();
    void Stop();
};

Log2ConsoleServer::Log2ConsoleServer(int port, bool useXmlFormat)
    : m_pImpl(std::make_unique<Impl>(port, useXmlFormat))
{
}

Log2ConsoleServer::~Log2ConsoleServer() = default;

Log2ConsoleServer::Log2ConsoleServer(Log2ConsoleServer&&) noexcept = default;
Log2ConsoleServer& Log2ConsoleServer::operator=(Log2ConsoleServer&&) noexcept = default;

bool Log2ConsoleServer::Start() {
    return m_pImpl->Start();
}

void Log2ConsoleServer::Stop() {
    m_pImpl->Stop();
}

bool Log2ConsoleServer::IsRunning() const {
    return m_pImpl->m_running;
}

void Log2ConsoleServer::Log(LogLevel level, const std::string& category, const std::string& message) {
    m_pImpl->SendLog2ConsoleMessage(level, category, message);
}

void Log2ConsoleServer::SetClientConnectedCallback(std::function<void(bool)> callback) {
    m_pImpl->m_clientConnectedCallback = callback;
}

void Log2ConsoleServer::SetXmlFormat(bool useXml) {
    m_pImpl->m_useXmlFormat = useXml;
}

// Implementation methods
bool Log2ConsoleServer::Impl::Start() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        return false;
    }

    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_serverSocket == INVALID_SOCKET) {
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(static_cast<u_short>(m_port));

    if (bind(m_serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(m_serverSocket);
        WSACleanup();
        return false;
    }

    if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(m_serverSocket);
        WSACleanup();
        return false;
    }

    m_running = true;
    m_acceptThread = std::thread(&Impl::AcceptThread, this);

    return true;
}

void Log2ConsoleServer::Impl::Stop() {
    m_running = false;

    if (m_serverSocket != INVALID_SOCKET) {
        closesocket(m_serverSocket);
        m_serverSocket = INVALID_SOCKET;
    }

    {
        std::lock_guard<std::mutex> lock(m_socketMutex);
        if (m_clientSocket != INVALID_SOCKET) {
            closesocket(m_clientSocket);
            m_clientSocket = INVALID_SOCKET;
        }
    }

    if (m_acceptThread.joinable()) {
        m_acceptThread.join();
    }

    WSACleanup();
}

void Log2ConsoleServer::Impl::AcceptThread() {
    while (m_running) {
        sockaddr_in clientAddr{};
        int clientAddrLen = sizeof(clientAddr);
        
        SOCKET newClient = accept(m_serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
        if (newClient != INVALID_SOCKET) {
            {
                std::lock_guard<std::mutex> lock(m_socketMutex);
                if (m_clientSocket != INVALID_SOCKET) {
                    closesocket(m_clientSocket);
                }
                m_clientSocket = newClient;
            }
            
            if (m_clientConnectedCallback) {
                m_clientConnectedCallback(true);
            }
        }
    }
}

void Log2ConsoleServer::Impl::SendLog2ConsoleMessage(LogLevel level, const std::string& category, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_socketMutex);
    if (m_clientSocket == INVALID_SOCKET) {
        return;
    }

    std::string formattedMessage = m_useXmlFormat 
        ? Log2ConsoleFormatter::FormatLog4jXml(level, category, message)
        : Log2ConsoleFormatter::FormatPlainText(level, category, message);
    
    int totalSent = 0;
    int messageLen = static_cast<int>(formattedMessage.length());
    
    while (totalSent < messageLen) {
        int sent = send(m_clientSocket, formattedMessage.c_str() + totalSent, messageLen - totalSent, 0);
        if (sent == SOCKET_ERROR) {
            closesocket(m_clientSocket);
            m_clientSocket = INVALID_SOCKET;
            if (m_clientConnectedCallback) {
                m_clientConnectedCallback(false);
            }
            break;
        }
        totalSent += sent;
    }
}

