#pragma once

#include "modules/Module.h"

class EnergyModule : public Module {
public:
    EnergyModule(const std::string& name, int durability, int energyConsumption,
                 int capacity, int efficiency);
};
