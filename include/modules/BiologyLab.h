#pragma once

#include "modules/ResearchModule.h"

class BiologyLab : public ResearchModule {
public:
    BiologyLab(const std::string& name, int durability, int energyConsumption,
               int capacity, int efficiency);

    std::unique_ptr<Module> clone() const override;
    std::string getType() const override;
};
