#pragma once

class EnergyConsumption {
public:
    EnergyConsumption();
    explicit EnergyConsumption(int value);

    int getValue() const;
    void setValue(int newValue);

    void increase(int amount);
    void decrease(int amount);

private:
    int value_;
};
