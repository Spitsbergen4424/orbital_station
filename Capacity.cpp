#include "Capacity.h"

#include <stdexcept>
#include <string>

namespace {

void requireNonNegative(int value) {
    if (value < 0) {
        throw std::invalid_argument("Capacity: значение не может быть отрицательным (" +
                                    std::to_string(value) + ")");
    }
}

}

Capacity::Capacity() : value_(0) {}

Capacity::Capacity(int value) : value_(value) {
    requireNonNegative(value);
}

int Capacity::getValue() const {
    return value_;
}

void Capacity::setValue(int newValue) {
    requireNonNegative(newValue);
    value_ = newValue;
}
