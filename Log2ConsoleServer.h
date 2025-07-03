#pragma once

#include "Log2ConsoleCommon.h"
#include <string>
#include <functional>
#include <memory>

class Log2ConsoleServer {
public:
    Log2ConsoleServer(int port = 4445, bool useXmlFormat = true);
    ~Log2ConsoleServer();

    // Delete copy constructor and copy assignment
    Log2ConsoleServer(const Log2ConsoleServer&) = delete;
    Log2ConsoleServer& operator=(const Log2ConsoleServer&) = delete;

    // Move constructor and move assignment
    Log2ConsoleServer(Log2ConsoleServer&&) noexcept;
    Log2ConsoleServer& operator=(Log2ConsoleServer&&) noexcept;

    bool Start();
    void Stop();
    bool IsRunning() const;

    void Log(LogLevel level, const std::string& category, const std::string& message);
    void SetClientConnectedCallback(std::function<void(bool)> callback);
    void SetXmlFormat(bool useXml);

private:
    class Impl;
    std::unique_ptr<Impl> m_pImpl;
};