#include "ResearchModules.h"

std::string ResearchModule::getCategory() const {
    return "Исследовательский";
}

std::string ResearchModule::getCapacityUnit() const {
    return "образцов";
}

BiologyLab::BiologyLab(const std::string& name, int samples)
    : ResearchModule(name, 75, Durability(90, 90), EnergyConsumption(15), Capacity(samples)) {}

std::unique_ptr<Module> BiologyLab::clone() const {
    return std::make_unique<BiologyLab>(*this);
}

std::string BiologyLab::getType() const {
    return "BiologyLab";
}

PhysicsLab::PhysicsLab(const std::string& name, int samples)
    : ResearchModule(name, 88, Durability(110, 110), EnergyConsumption(40), Capacity(samples)) {}

std::unique_ptr<Module> PhysicsLab::clone() const {
    return std::make_unique<PhysicsLab>(*this);
}

std::string PhysicsLab::getType() const {
    return "PhysicsLab";
}
