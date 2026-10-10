#pragma once

class Durability {
public:
    static constexpr int kDefaultMax = 100;

    Durability();
    explicit Durability(int value, int maxValue = kDefaultMax);

    int getValue() const;
    int getMax() const;
    void setValue(int newValue);

    void decrease(int amount);
    void repair(int amount);

    bool isBroken() const;
    bool isFull() const;

private:
    int value_;
    int max_;
};
