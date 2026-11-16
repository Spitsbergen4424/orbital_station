#include "attributes/Capacity.h"

    Capacity::Capacity(): value_(0) {}
    Capacity::Capacity(int value): value_(value) {} 

    int Capacity::getValue() const {
        return value_;
    }

    void Capacity::setValue(int newValue) {
        if (newValue > 0) {
            value_ = newValue;
        }
    }
