#include "Log2ConsoleServer.h"

Log2ConsoleServer::Log2ConsoleServer(int port, bool useXmlFormat)
    : m_port(port)
    , m_serverSocket(INVALID_SOCKET)
    , m_clientSocket(INVALID_SOCKET)
    , m_running(false)
    , m_useXmlFormat(useXmlFormat)
{
}

Log2ConsoleServer::~Log2ConsoleServer() {
    Stop();
}

bool Log2ConsoleServer::Start() {
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
    serverAddr.sin_port = htons(m_port);

    if (bind(m_serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
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
    m_acceptThread = std::thread(&Log2ConsoleServer::AcceptThread, this);

    return true;
}

void Log2ConsoleServer::Stop() {
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

void Log2ConsoleServer::AcceptThread() {
    while (m_running) {
        sockaddr_in clientAddr{};
        int clientAddrLen = sizeof(clientAddr);
        
        SOCKET newClient = accept(m_serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
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

void Log2ConsoleServer::Log(LogLevel level, const std::string& category, const std::string& message) {
    SendLog2ConsoleMessage(level, category, message);
}

void Log2ConsoleServer::SendLog2ConsoleMessage(LogLevel level, const std::string& category, const std::string& message) {
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

