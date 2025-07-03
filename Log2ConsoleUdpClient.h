#pragma once

#include "Log2ConsoleCommon.h"
#include <string>
#include <memory>

class Log2ConsoleUdpClient {
public:
    Log2ConsoleUdpClient(const std::string& serverHost = "localhost", int serverPort = 4445, bool useXmlFormat = true);
    ~Log2ConsoleUdpClient();

    // Delete copy constructor and copy assignment
    Log2ConsoleUdpClient(const Log2ConsoleUdpClient&) = delete;
    Log2ConsoleUdpClient& operator=(const Log2ConsoleUdpClient&) = delete;

    // Move constructor and move assignment
    Log2ConsoleUdpClient(Log2ConsoleUdpClient&&) noexcept;
    Log2ConsoleUdpClient& operator=(Log2ConsoleUdpClient&&) noexcept;

    bool Initialize();
    void Cleanup();
    bool IsInitialized() const;

    void Log(LogLevel level, const std::string& category, const std::string& message);
    void SetXmlFormat(bool useXml);

private:
    class Impl;
    std::unique_ptr<Impl> m_pImpl;
};