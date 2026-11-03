#pragma once

#include "modules/Module.h"

// Энергетический модуль: вместимость — ёмкость накопителя.
// Остаётся абстрактным: clone() и getType() реализуют конкретные генераторы.
class EnergyModule : public Module {
public:
    EnergyModule(const std::string& name, int durability, int energyConsumption,
                 int capacity, int efficiency);
};
