#pragma once

#include "Log2ConsoleCommon.h"
#include <string>
#include <memory>

class Log2ConsoleClient {
public:
    Log2ConsoleClient(const std::string& serverHost = "localhost", int serverPort = 4445, bool useXmlFormat = true);
    ~Log2ConsoleClient();

    // Delete copy constructor and copy assignment
    Log2ConsoleClient(const Log2ConsoleClient&) = delete;
    Log2ConsoleClient& operator=(const Log2ConsoleClient&) = delete;

    // Move constructor and move assignment
    Log2ConsoleClient(Log2ConsoleClient&&) noexcept;
    Log2ConsoleClient& operator=(Log2ConsoleClient&&) noexcept;

    bool Connect();
    void Disconnect();
    bool IsConnected() const;

    void Log(LogLevel level, const std::string& category, const std::string& message);
    void SetXmlFormat(bool useXml);
    void SetAutoReconnect(bool autoReconnect);
    void SetReconnectDelay(int seconds);

private:
    class Impl;
    std::unique_ptr<Impl> m_pImpl;
};