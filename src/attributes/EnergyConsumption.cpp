#include "attributes/EnergyConsumption.h"

EnergyConsumption::EnergyConsumption(): value_(0) {}            // потребление 0
EnergyConsumption::EnergyConsumption(int value): value_(value) {}   // начальное потребление value

int EnergyConsumption::getValue() const {
    return value_;
}

void EnergyConsumption::setValue(int newValue) {
    if (newValue >= 0) {
        value_ = newValue;
    }
}

void EnergyConsumption::increase(int amount) {
    value_ += amount;
}

void EnergyConsumption::decrease(int amount) {
    value_ -= amount;
    if (value_ < 0) {
        value_ = 0;
    }
}
