#include "logging/ConsoleLogger.h"
#include <iostream>
void ConsoleLogger::write(const std::string& line) { std::cout << line << '\n'; }
