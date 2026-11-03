#include "attributes/Durability.h"


    Durability::Durability(): value_(DEFAULT_VALUE), maxValue_(DEFAULT_VALUE) {}            // прочность DEFAULT_VALUE
    Durability::Durability(int value): value_(value), maxValue_(value) {}   // начальная прочность value, она же максимальная

    int Durability::getValue() const {
        return value_;
    }

    int Durability::getMaxValue() const {
        return maxValue_;
    }

    void Durability::setValue(int newValue) {
        if (newValue >= 0 && newValue <= maxValue_) {
            value_ = newValue;
        }
    }

    void Durability::decrease(int amount) {
        value_ -= amount;
        if (value_ < 0) {
            value_ = 0;
        }
    }

    void Durability::repair(int amount) {
        value_ += amount;
        if (value_ > maxValue_) {
            value_ = maxValue_;
        }
    }