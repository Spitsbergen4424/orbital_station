#pragma once

// Энергопотребление модуля, кВт
class EnergyConsumption {
public:
    EnergyConsumption();
    EnergyConsumption(int value);

    int getValue() const;

    void setValue(int newValue);
    void increase(int amount);
    void decrease(int amount);

private:
    int value_;  // текущее потребление
};
