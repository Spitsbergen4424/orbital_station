#pragma once

class Capacity {
public:
    Capacity();
    explicit Capacity(int value);

    int getValue() const;
    void setValue(int newValue);

private:
    int value_;
};
