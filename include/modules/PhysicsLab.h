#pragma once

#include "modules/ResearchModule.h"

class PhysicsLab : public ResearchModule {
public:
    PhysicsLab(const std::string& name, int durability, int energyConsumption,
               int capacity, int efficiency);

    std::unique_ptr<Module> clone() const override;
    std::string getType() const override;
};
