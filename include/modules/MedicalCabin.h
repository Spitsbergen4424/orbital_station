#pragma once

#include "modules/ResidentialModule.h"

// Медицинский отсек — улучшенный жилой модуль
class MedicalCabin : public ResidentialModule {
public:
    MedicalCabin(const std::string& name, int durability, int energyConsumption,
                 int capacity, int efficiency);

    std::unique_ptr<Module> clone() const override;  // копия медотсека
    std::string getType() const override;            // "MedicalCabin"
};
