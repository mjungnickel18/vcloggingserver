#include "Log2ConsoleClient.h"
#include <chrono>

Log2ConsoleClient::Log2ConsoleClient(const std::string& serverHost, int serverPort, bool useXmlFormat)
    : m_serverHost(serverHost)
    , m_serverPort(serverPort)
    , m_socket(INVALID_SOCKET)
    , m_connected(false)
    , m_running(false)
    , m_useXmlFormat(useXmlFormat)
    , m_autoReconnect(true)
    , m_reconnectDelay(5)
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

Log2ConsoleClient::~Log2ConsoleClient() {
    Disconnect();
    WSACleanup();
}

bool Log2ConsoleClient::Connect() {
    if (m_connected) {
        return true;
    }

    if (!m_running) {
        m_running = true;
        m_workerThread = std::thread(&Log2ConsoleClient::WorkerThread, this);
    }

    // Wait a bit for connection
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return m_connected;
}

void Log2ConsoleClient::Disconnect() {
    m_running = false;
    m_autoReconnect = false;
    
    if (m_socket != INVALID_SOCKET) {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }
    
    m_connected = false;
    m_queueCv.notify_all();
    
    if (m_workerThread.joinable()) {
        m_workerThread.join();
    }
}

void Log2ConsoleClient::Log(LogLevel level, const std::string& category, const std::string& message) {
    if (!m_running) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_logQueue.push({level, category, message});
    }
    m_queueCv.notify_one();
}

void Log2ConsoleClient::WorkerThread() {
    while (m_running) {
        if (!m_connected) {
            if (TryConnect()) {
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

            SendMessage(formattedMessage);
            lock.lock();
        }
    }
}

bool Log2ConsoleClient::TryConnect() {
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET) {
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(m_serverPort);
    
    // Convert host to IP
    struct hostent* host = gethostbyname(m_serverHost.c_str());
    if (host == nullptr) {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
        return false;
    }
    
    memcpy(&serverAddr.sin_addr, host->h_addr_list[0], host->h_length);

    if (connect(m_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
        return false;
    }

    return true;
}

void Log2ConsoleClient::SendMessage(const std::string& message) {
    if (m_socket == INVALID_SOCKET || !m_connected) {
        return;
    }

    int totalSent = 0;
    int messageLen = static_cast<int>(message.length());

    while (totalSent < messageLen) {
        int sent = send(m_socket, message.c_str() + totalSent, messageLen - totalSent, 0);
        if (sent == SOCKET_ERROR) {
            closesocket(m_socket);
            m_socket = INVALID_SOCKET;
            m_connected = false;
            break;
        }
        totalSent += sent;
    }
}