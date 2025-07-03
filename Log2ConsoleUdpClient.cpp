#include "Log2ConsoleUdpClient.h"
#include "SocketPlatform.h"
#include <mutex>
#include <cstring>
#include <string>

class Log2ConsoleUdpClient::Impl {
public:
    Impl(const std::string& serverHost, int serverPort, bool useXmlFormat)
        : m_serverHost(serverHost)
        , m_serverPort(serverPort)
        , m_socket(INVALID_SOCKET_VALUE_VALUE)
        , m_initialized(false)
        , m_useXmlFormat(useXmlFormat)
    {
        SocketPlatform::Initialize();
    }

    ~Impl() {
        Cleanup();
        SocketPlatform::Cleanup();
    }

    std::string m_serverHost;
    int m_serverPort;
    socket_t m_socket;
    bool m_initialized;
    bool m_useXmlFormat;
    
    struct sockaddr_in m_serverAddr;
    std::mutex m_sendMutex;

    bool Initialize();
    void Cleanup();
    bool SendMessage(const std::string& message);
};

Log2ConsoleUdpClient::Log2ConsoleUdpClient(const std::string& serverHost, int serverPort, bool useXmlFormat)
    : m_pImpl(std::make_unique<Impl>(serverHost, serverPort, useXmlFormat))
{
}

Log2ConsoleUdpClient::~Log2ConsoleUdpClient() = default;

Log2ConsoleUdpClient::Log2ConsoleUdpClient(Log2ConsoleUdpClient&&) noexcept = default;
Log2ConsoleUdpClient& Log2ConsoleUdpClient::operator=(Log2ConsoleUdpClient&&) noexcept = default;

bool Log2ConsoleUdpClient::Initialize() {
    return m_pImpl->Initialize();
}

void Log2ConsoleUdpClient::Cleanup() {
    m_pImpl->Cleanup();
}

bool Log2ConsoleUdpClient::IsInitialized() const {
    return m_pImpl->m_initialized;
}

void Log2ConsoleUdpClient::Log(LogLevel level, const std::string& category, const std::string& message) {
    if (!m_pImpl->m_initialized) {
        return;
    }

    std::string formattedMessage = m_pImpl->m_useXmlFormat
        ? Log2ConsoleFormatter::FormatLog4jXml(level, category, message)
        : Log2ConsoleFormatter::FormatPlainText(level, category, message);

    m_pImpl->SendMessage(formattedMessage);
}

void Log2ConsoleUdpClient::Log(LogLevel level, const std::string& category, const std::string& message, 
                               const char* file, const char* function, int line) {
    if (!m_pImpl->m_initialized) {
        return;
    }

    std::string formattedMessage = m_pImpl->m_useXmlFormat
        ? Log2ConsoleFormatter::FormatLog4jXml(level, category, message, file, function, line)
        : Log2ConsoleFormatter::FormatPlainText(level, category, message);

    m_pImpl->SendMessage(formattedMessage);
}

void Log2ConsoleUdpClient::SetXmlFormat(bool useXml) {
    m_pImpl->m_useXmlFormat = useXml;
}

// Implementation methods
bool Log2ConsoleUdpClient::Impl::Initialize() {
    if (m_initialized) {
        return true;
    }

    // Create UDP socket
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_socket == INVALID_SOCKET_VALUE) {
        return false;
    }

    // Resolve server address
    struct addrinfo hints{};
    struct addrinfo* result = nullptr;
    
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    
    std::string portStr = std::to_string(m_serverPort);
    int res = getaddrinfo(m_serverHost.c_str(), portStr.c_str(), &hints, &result);
    if (res != 0) {
        closesocket_platform(m_socket);
        m_socket = INVALID_SOCKET_VALUE;
        return false;
    }

    // Copy the server address
    memcpy(&m_serverAddr, result->ai_addr, sizeof(m_serverAddr));
    freeaddrinfo(result);

    m_initialized = true;
    return true;
}

void Log2ConsoleUdpClient::Impl::Cleanup() {
    m_initialized = false;
    
    if (m_socket != INVALID_SOCKET_VALUE) {
        closesocket_platform(m_socket);
        m_socket = INVALID_SOCKET_VALUE;
    }
}

bool Log2ConsoleUdpClient::Impl::SendMessage(const std::string& message) {
    if (!m_initialized || m_socket == INVALID_SOCKET_VALUE) {
        return false;
    }

    std::lock_guard<std::mutex> lock(m_sendMutex);
    
    int result = sendto(m_socket, 
                       message.c_str(), 
                       static_cast<int>(message.length()), 
                       0, 
                       (struct sockaddr*)&m_serverAddr, 
                       sizeof(m_serverAddr));
    
    return result != SOCKET_ERROR_VALUE;
}