#include "modules/BiologyLab.h"

BiologyLab::BiologyLab(const std::string& name, int durability, int energyConsumption,
                        int capacity, int efficiency)
    : ResearchModule (name, durability, energyConsumption, capacity, efficiency) {}

std::unique_ptr<Module> BiologyLab::clone() const {
    return std::make_unique<BiologyLab>(*this);
}

std::string BiologyLab::getType() const {
    return "BiologyLab";
}