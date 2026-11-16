#pragma once

#include "modules/Module.h"

// Режим воздействия, в котором работает конкретная стратегия.
enum class StrategyMode {
    DeepSpace,
    AsteroidField
};

// Strategy: алгоритм воздействия выбирается по семейству модуля.
class InteractionStrategy {
public:
    virtual ~InteractionStrategy() = default;
    virtual void apply(Module& module) const = 0;
};
