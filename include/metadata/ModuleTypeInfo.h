#pragma once
#include <string>

// Flyweight: общие неизменяемые характеристики типа модуля разделяются экземплярами.
struct ModuleTypeInfo {
    int maxHealth;
    int baseEnergyConsumption;
    int baseEfficiency;
    int cost;
    std::string type;
};
