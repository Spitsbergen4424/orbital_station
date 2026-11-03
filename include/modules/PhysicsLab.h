#pragma once

#include "modules/ResearchModule.h"

// Физическая лаборатория — улучшенный исследовательский модуль
class PhysicsLab : public ResearchModule {
public:
    PhysicsLab(const std::string& name, int durability, int energyConsumption,
               int capacity, int efficiency);

    std::unique_ptr<Module> clone() const override;  // копия лаборатории
    std::string getType() const override;            // "PhysicsLab"
};
