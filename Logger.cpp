#include "Logger.h"
#include <sstream>

Logger& Logger::GetInstance() {
    static Logger instance;
    
    // Auto-initialize with default settings if not already initialized
    if (!instance.m_initialized) {
        instance.Initialize();
    }
    
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

void Logger::LogWithLocation(LogLevel level, const std::string& category, const std::string& message, 
                             const char* file, const char* function, int line) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    // Use the overloaded UDP client method that handles file/function/line info
    m_client->Log(level, category, message, file, function, line);
}

void Logger::SetXmlFormat(bool useXml) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (m_client) {
        m_client->SetXmlFormat(useXml);
    }
}

void Logger::LogToken(const std::string& tokenId, LogLevel level, const std::string& category, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    // Calculate hash of the message
    std::size_t messageHash = std::hash<std::string>{}(message);
    
    // Check if we've seen this message for this token before
    auto it = m_tokenHashes.find(tokenId);
    if (it != m_tokenHashes.end() && it->second == messageHash) {
        // Same message, don't log
        return;
    }
    
    // New or different message, update hash and log
    m_tokenHashes[tokenId] = messageHash;
    m_client->Log(level, category, message);
}

void Logger::LogTokenWithLocation(const std::string& tokenId, LogLevel level, const std::string& category, const std::string& message,
                                 const char* file, const char* function, int line) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (!m_initialized || !m_client) {
        return;
    }

    // Calculate hash of the message
    std::size_t messageHash = std::hash<std::string>{}(message);
    
    // Check if we've seen this message for this token before
    auto it = m_tokenHashes.find(tokenId);
    if (it != m_tokenHashes.end() && it->second == messageHash) {
        // Same message, don't log
        return;
    }
    
    // New or different message, update hash and log
    m_tokenHashes[tokenId] = messageHash;
    m_client->Log(level, category, message, file, function, line);
}