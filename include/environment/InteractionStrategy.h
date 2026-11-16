#pragma once

#include "modules/Module.h"

// Один и тот же тип стратегии может работать в разных режимах окружения.
enum class StrategyMode {
    DeepSpace,
    AsteroidField
};

// Strategy: алгоритм воздействия выбирается по фактическому типу модуля.
class InteractionStrategy {
public:
    virtual ~InteractionStrategy() = default;
    virtual void apply(Module& module) const = 0;
};
