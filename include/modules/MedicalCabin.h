#pragma once

#include "modules/ResidentialModule.h"

class MedicalCabin : public ResidentialModule {
public:
    MedicalCabin(const std::string& name, int durability, int energyConsumption,
                 int capacity, int efficiency);

    std::unique_ptr<Module> clone() const override;
    std::string getType() const override;
};
