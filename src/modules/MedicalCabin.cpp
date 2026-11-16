#include "modules/MedicalCabin.h"

MedicalCabin::MedicalCabin(const std::string& name, int durability, int energyConsumption,
                        int capacity, int efficiency)
    : ResidentialModule (name, durability, energyConsumption, capacity, efficiency) {}

std::unique_ptr<Module> MedicalCabin::clone() const {
    return std::make_unique<MedicalCabin>(*this);
}

std::string MedicalCabin::getType() const {
    return "MedicalCabin";
}