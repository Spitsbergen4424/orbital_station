#pragma once

#include "modules/Module.h"

// Жилой модуль: вместимость — число людей.
// Остаётся абстрактным: clone() и getType() реализуют конкретные каюты.
class ResidentialModule : public Module {
public:
    ResidentialModule(const std::string& name, int durability, int energyConsumption,
                      int capacity, int efficiency);
};
