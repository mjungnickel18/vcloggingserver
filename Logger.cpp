#include "Logger.h"
#include <sstream>

Logger& Logger::GetInstance() {
    static Logger instance;
    return instance;
}

bool Logger::Initialize(const std::string& serverHost, int serverPort, bool useXmlFormat) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (m_initialized) {
        return true;
    }

    m_client = std::make_unique<Log2ConsoleUdpClient>(serverHost, serverPort, useXmlFormat);
    
    if (!m_client->Initialize()) {
        m_client.reset();
        return false;
    }

    m_initialized = true;
    return true;
}

void Logger::Cleanup() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (m_client) {
        m_client->Cleanup();
        m_client.reset();
    }
    
    m_initialized = false;
}

bool Logger::IsInitialized() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_initialized;
}

void Logger::Log(LogLevel level, const std::string& category, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    m_client->Log(level, category, message);
}

void Logger::Log(LogLevel level, const std::string& category, const std::string& message, 
                 const char* file, const char* function, int line) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    // Extract just the filename from the full path
    const char* filename = file;
    const char* lastSlash = file;
    while (*lastSlash) {
        if (*lastSlash == '\\' || *lastSlash == '/') {
            filename = lastSlash + 1;
        }
        lastSlash++;
    }

    // Create enhanced message with file/function/line info
    std::ostringstream oss;
    oss << message << " [" << filename << ":" << function << ":" << line << "]";

    m_client->Log(level, category, oss.str());
}

void Logger::SetXmlFormat(bool useXml) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (m_client) {
        m_client->SetXmlFormat(useXml);
    }
}