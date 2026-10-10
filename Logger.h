#pragma once

#include <cstddef>
#include <memory>
#include <set>
#include <string>

enum class LogSource { STATION, MODULE, FACTORY };

enum class LogLevel { Info, Warning, Error };

std::string toString(LogSource source);
std::string toString(LogLevel level);

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(LogSource source, LogLevel level, const std::string& message) = 0;
};

class ConsoleLogger : public ILogger {
public:
    void log(LogSource source, LogLevel level, const std::string& message) override;
};

class LoggingProxy : public ILogger {
public:
    static LoggingProxy& instance();

    LoggingProxy(const LoggingProxy&) = delete;
    LoggingProxy& operator=(const LoggingProxy&) = delete;

    void log(LogSource source, LogLevel level, const std::string& message) override;

    void setEnabled(bool enabled);
    bool isEnabled() const;
    void enableSource(LogSource source, bool enabled);
    void setMinLevel(LogLevel level);
    std::size_t recordsWritten() const;

private:
    LoggingProxy() = default;

    std::unique_ptr<ILogger> realLogger_;
    std::set<LogSource> disabledSources_;
    LogLevel minLevel_ = LogLevel::Info;
    bool enabled_ = true;
    std::size_t counter_ = 0;
};

void logInfo(LogSource source, const std::string& message) noexcept;
void logWarning(LogSource source, const std::string& message) noexcept;
void logError(LogSource source, const std::string& message) noexcept;
