#pragma once

#include "modules/Module.h"

class ResidentialModule : public Module {
public:
    ResidentialModule(const std::string& name, int durability, int energyConsumption,
                      int capacity, int efficiency);
};
