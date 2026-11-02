#pragma once

// Вместимость модуля: для жилых — число людей,
// для исследовательских — число образцов, для энергетических — ёмкость накопителя
class Capacity {
public:
    Capacity();            // вместимость 0
    Capacity(int value);   // начальная вместимость value

    int getValue() const;

    void setValue(int newValue);  // значение не меньше 0

private:
    int value_;  // текущая вместимость
};
