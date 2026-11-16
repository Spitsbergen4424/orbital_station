#pragma once
#include <memory>
#include <string>
#include "logging/Logger.h"
class LoggingProxy final : public Logger {
public:
    explicit LoggingProxy(std::shared_ptr<Logger> target);
    void write(const std::string& line) override;
    static void setGlobal(std::shared_ptr<LoggingProxy> logger);
    static void globalLog(const std::string& source, const std::string& message);
private: std::shared_ptr<Logger> target_;
};
