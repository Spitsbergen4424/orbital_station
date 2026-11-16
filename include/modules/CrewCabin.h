#pragma once

#include "modules/ResidentialModule.h"


class CrewCabin : public ResidentialModule {
public:
    CrewCabin(const std::string& name, int durability, int energyConsumption,
              int capacity, int efficiency);

    std::unique_ptr<Module> clone() const override;
    std::string getType() const override;
};
