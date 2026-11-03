#pragma once

#include "modules/Module.h"

// Исследовательский модуль: вместимость — число образцов.
// Остаётся абстрактным: clone() и getType() реализуют конкретные лаборатории.
class ResearchModule : public Module {
public:
    ResearchModule(const std::string& name, int durability, int energyConsumption,
                   int capacity, int efficiency);
};
