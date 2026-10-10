#include "EnergyConsumption.h"

#include <stdexcept>
#include <string>

namespace {

void requireNonNegative(int value, const char* what) {
    if (value < 0) {
        throw std::invalid_argument(std::string("EnergyConsumption: ") + what +
                                    " не может быть отрицательным (" + std::to_string(value) + ")");
    }
}

}

EnergyConsumption::EnergyConsumption() : value_(0) {}

EnergyConsumption::EnergyConsumption(int value) : value_(value) {
    requireNonNegative(value, "значение");
}

int EnergyConsumption::getValue() const {
    return value_;
}

void EnergyConsumption::setValue(int newValue) {
    requireNonNegative(newValue, "значение");
    value_ = newValue;
}

void EnergyConsumption::increase(int amount) {
    requireNonNegative(amount, "изменение");
    value_ += amount;
}

void EnergyConsumption::decrease(int amount) {
    requireNonNegative(amount, "изменение");
    value_ = amount >= value_ ? 0 : value_ - amount;
}
