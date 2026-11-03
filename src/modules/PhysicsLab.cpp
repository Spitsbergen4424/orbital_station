#include "modules/PhysicsLab.h"

PhysicsLab::PhysicsLab(const std::string& name, int durability, int energyConsumption,
                        int capacity, int efficiency)
    : ResearchModule (name, durability, energyConsumption, capacity, efficiency) {}

std::unique_ptr<Module> PhysicsLab::clone() const {
    return std::make_unique<PhysicsLab>(*this);
}

std::string PhysicsLab::getType() const {
    return "PhysicsLab";
}