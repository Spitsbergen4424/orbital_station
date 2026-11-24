#pragma once
#include <chrono>
#include <string>

enum class ModuleEventType { Activated, Deactivated, Damaged, Destroyed, EnergyConsumed, Modified };

struct ModuleEvent {
    ModuleEventType type;
    int moduleId;
    int damage; // величина повреждения; для EnergyConsumed здесь хранится расход энергии
    std::chrono::system_clock::time_point timestamp;
};
