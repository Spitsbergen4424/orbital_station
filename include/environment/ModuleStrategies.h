#pragma once

#include "environment/InteractionStrategy.h"

// Стратегии принимают Module&, поэтому вызывающий код не зависит
// от конкретной реализации модуля и не копирует объект.
class EnergyModuleStrategy final : public InteractionStrategy {
public:
    explicit EnergyModuleStrategy(StrategyMode mode);
    void apply(Module& module) const override;
private:
    StrategyMode mode_;
};

class CrewModuleStrategy final : public InteractionStrategy {
public:
    explicit CrewModuleStrategy(StrategyMode mode);
    void apply(Module& module) const override;
private:
    StrategyMode mode_;
};

class ResearchModuleStrategy final : public InteractionStrategy {
public:
    explicit ResearchModuleStrategy(StrategyMode mode);
    void apply(Module& module) const override;
private:
    StrategyMode mode_;
};

class ReactorModuleStrategy final : public InteractionStrategy {
public:
    explicit ReactorModuleStrategy(StrategyMode mode);
    void apply(Module& module) const override;
private:
    StrategyMode mode_;
};
