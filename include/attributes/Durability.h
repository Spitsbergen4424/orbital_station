#pragma once

// Прочность модуля
class Durability {
public:
    static constexpr int DEFAULT_VALUE = 100;  // прочность по умолчанию

    Durability();            // прочность DEFAULT_VALUE
    Durability(int value);   // начальная прочность value, она же максимальная

    int getValue() const;
    int getMaxValue() const;

    void setValue(int newValue);  // значение от 0 до максимума
    void decrease(int amount);    // уменьшить прочность (урон), не ниже 0
    void repair(int amount);      // восстановить прочность, не выше максимума

private:
    int value_;     // текущая прочность
    int maxValue_;  // максимальная прочность
};
