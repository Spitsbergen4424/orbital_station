#pragma once

#include "modules/EnergyModule.h"

class SolarModule : public EnergyModule {
public:
    SolarModule(const std::string& name, int durability, int energyConsumption,
                int capacity, int efficiency);

    std::unique_ptr<Module> clone() const override;
    std::string getType() const override;
};
