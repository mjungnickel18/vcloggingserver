#include "Log2ConsoleServer.h"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

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
        ? FormatLog4jXmlMessage(level, category, message)
        : FormatLog2ConsoleMessage(level, category, message);
    
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

std::string Log2ConsoleServer::FormatLog2ConsoleMessage(LogLevel level, const std::string& category, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    
    std::tm tm{};
    localtime_s(&tm, &time_t_now);
    
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();
    ss << " [" << LogLevelToString(level) << "] ";
    ss << "[" << category << "] ";
    ss << message;
    ss << "\r\n";
    
    return ss.str();
}

const char* Log2ConsoleServer::LogLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE: return "TRACE";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

const char* Log2ConsoleServer::LogLevelToLog4jString(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE: return "TRACE";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "INFO";
    }
}

std::string Log2ConsoleServer::EscapeXml(const std::string& text) {
    std::string result;
    result.reserve(text.size());
    
    for (char c : text) {
        switch (c) {
            case '&':  result += "&amp;"; break;
            case '<':  result += "&lt;"; break;
            case '>':  result += "&gt;"; break;
            case '"':  result += "&quot;"; break;
            case '\'': result += "&apos;"; break;
            default:   result += c; break;
        }
    }
    
    return result;
}

std::string Log2ConsoleServer::FormatLog4jXmlMessage(LogLevel level, const std::string& category, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto ms_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    
    std::stringstream ss;
    ss << "<log4j:event logger=\"" << EscapeXml(category) << "\" ";
    ss << "timestamp=\"" << ms_since_epoch << "\" ";
    ss << "level=\"" << LogLevelToLog4jString(level) << "\" ";
    ss << "thread=\"" << GetCurrentThreadId() << "\">\r\n";
    ss << "<log4j:message><![CDATA[" << message << "]]></log4j:message>\r\n";
    ss << "<log4j:properties>\r\n";
    ss << "<log4j:data name=\"log4net:HostName\" value=\"" << EscapeXml("localhost") << "\"/>\r\n";
    ss << "</log4j:properties>\r\n";
    ss << "</log4j:event>\r\n";
    
    return ss.str();
}