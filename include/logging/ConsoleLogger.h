#pragma once
#include "logging/Logger.h"
class ConsoleLogger final : public Logger { public: void write(const std::string& line) override; };
