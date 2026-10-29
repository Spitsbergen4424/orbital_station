#pragma once

// Энергопотребление модуля, кВт
class EnergyConsumption {
public:
    EnergyConsumption();            // потребление 0
    EnergyConsumption(int value);   // начальное потребление value

    int getValue() const;

    void setValue(int newValue);  // значение не меньше 0
    void increase(int amount);    // увеличить потребление
    void decrease(int amount);    // уменьшить потребление, не ниже 0

private:
    int value_;  // текущее потребление
};
