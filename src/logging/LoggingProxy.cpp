#include "logging/LoggingProxy.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <iostream>
namespace { std::shared_ptr<LoggingProxy> globalLogger; }
LoggingProxy::LoggingProxy(std::shared_ptr<Logger> target) : target_(std::move(target)) { if(!target_) throw std::invalid_argument("LoggingProxy: logger is null"); }
void LoggingProxy::write(const std::string& line) {
    const auto now=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm,&now);
#else
    localtime_r(&now,&tm);
#endif
    std::ostringstream out; out << '[' << std::put_time(&tm,"%Y-%m-%d %H:%M:%S") << "] " << line;
    target_->write(out.str());
}

void LoggingProxy::setGlobal(std::shared_ptr<LoggingProxy> logger) { globalLogger = std::move(logger); }
void LoggingProxy::globalLog(const std::string& source,const std::string& message) { const std::string line="["+source+"] "+message; if(globalLogger) globalLogger->write(line); else std::cout<<line<<"\n"; }
