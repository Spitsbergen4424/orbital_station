#include "Durability.h"

#include <stdexcept>
#include <string>

namespace {

void requireNonNegativeAmount(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Durability: величина изменения не может быть отрицательной");
    }
}

}

Durability::Durability() : value_(kDefaultMax), max_(kDefaultMax) {}

Durability::Durability(int value, int maxValue) : value_(value), max_(maxValue) {
    if (maxValue <= 0) {
        throw std::invalid_argument("Durability: максимум должен быть положительным");
    }
    if (value < 0 || value > maxValue) {
        throw std::invalid_argument("Durability: значение " + std::to_string(value) +
                                    " вне диапазона [0, " + std::to_string(maxValue) + "]");
    }
}

int Durability::getValue() const {
    return value_;
}

int Durability::getMax() const {
    return max_;
}

void Durability::setValue(int newValue) {
    if (newValue < 0 || newValue > max_) {
        throw std::invalid_argument("Durability: значение " + std::to_string(newValue) +
                                    " вне диапазона [0, " + std::to_string(max_) + "]");
    }
    value_ = newValue;
}

void Durability::decrease(int amount) {
    requireNonNegativeAmount(amount);
    value_ = amount >= value_ ? 0 : value_ - amount;
}

void Durability::repair(int amount) {
    requireNonNegativeAmount(amount);
    value_ = amount >= max_ - value_ ? max_ : value_ + amount;
}

bool Durability::isBroken() const {
    return value_ == 0;
}

bool Durability::isFull() const {
    return value_ == max_;
}
