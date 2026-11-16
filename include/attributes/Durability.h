#pragma once

class Durability {
public:
    static constexpr int DEFAULT_VALUE = 100;

    Durability();
    Durability(int value);

    int getValue() const;
    int getMaxValue() const;

    void setValue(int newValue);
    void decrease(int amount);
    void repair(int amount);

private:
    int value_;
    int maxValue_;
};
