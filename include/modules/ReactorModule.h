#pragma once

#include "modules/EnergyModule.h"

class ReactorModule : public EnergyModule {
public:
    ReactorModule(const std::string& name, int durability, int energyConsumption,
                  int capacity, int efficiency);

    std::unique_ptr<Module> clone() const override;
    std::string getType() const override;
};
