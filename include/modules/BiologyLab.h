#pragma once

#include "modules/ResearchModule.h"

// Биологическая лаборатория — стандартный исследовательский модуль
class BiologyLab : public ResearchModule {
public:
    BiologyLab(const std::string& name, int durability, int energyConsumption,
               int capacity, int efficiency);

    std::unique_ptr<Module> clone() const override;  // копия лаборатории
    std::string getType() const override;            // "BiologyLab"
};
