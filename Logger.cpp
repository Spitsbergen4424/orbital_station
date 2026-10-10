#include "Logger.h"

#include <iomanip>
#include <iostream>
#include <sstream>

std::string toString(LogSource source) {
    switch (source) {
    case LogSource::STATION: return "STATION";
    case LogSource::MODULE:  return "MODULE";
    case LogSource::FACTORY: return "FACTORY";
    }
    return "UNKNOWN";
}

std::string toString(LogLevel level) {
    switch (level) {
    case LogLevel::Info:    return "INFO";
    case LogLevel::Warning: return "WARN";
    case LogLevel::Error:   return "ERROR";
    }
    return "UNKNOWN";
}

void ConsoleLogger::log(LogSource source, LogLevel level, const std::string& message) {
    std::cout << "    [" << toString(source) << '|' << toString(level) << "] " << message << '\n';
}

LoggingProxy& LoggingProxy::instance() {
    static LoggingProxy proxy;
    return proxy;
}

void LoggingProxy::log(LogSource source, LogLevel level, const std::string& message) {
    if (!enabled_ || level < minLevel_ || disabledSources_.count(source) != 0) {
        return;
    }
    if (!realLogger_) {
        realLogger_ = std::make_unique<ConsoleLogger>();
    }
    ++counter_;
    std::ostringstream record;
    record << '#' << std::setw(3) << std::setfill('0') << counter_ << ' ' << message;
    realLogger_->log(source, level, record.str());
}

void LoggingProxy::setEnabled(bool enabled) {
    enabled_ = enabled;
}

bool LoggingProxy::isEnabled() const {
    return enabled_;
}

void LoggingProxy::enableSource(LogSource source, bool enabled) {
    if (enabled) {
        disabledSources_.erase(source);
    } else {
        disabledSources_.insert(source);
    }
}

void LoggingProxy::setMinLevel(LogLevel level) {
    minLevel_ = level;
}

std::size_t LoggingProxy::recordsWritten() const {
    return counter_;
}

namespace {

void write(LogSource source, LogLevel level, const std::string& message) noexcept {
    try {
        LoggingProxy::instance().log(source, level, message);
    } catch (...) {
    }
}

}

void logInfo(LogSource source, const std::string& message) noexcept {
    write(source, LogLevel::Info, message);
}

void logWarning(LogSource source, const std::string& message) noexcept {
    write(source, LogLevel::Warning, message);
}

void logError(LogSource source, const std::string& message) noexcept {
    write(source, LogLevel::Error, message);
}
