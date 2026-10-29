#pragma once

#include "ResidentialModule.h"

// Каюта экипажа — стандартный жилой модуль
class CrewCabin : public ResidentialModule {
public:
    CrewCabin(const std::string& name, int durability, int energyConsumption,
              int capacity, int efficiency);

    std::unique_ptr<Module> clone() const override;  // копия каюты
    std::string getType() const override;            // "CrewCabin"
};
