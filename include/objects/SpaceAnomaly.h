#pragma once
#include "objects/ExternalObject.h"
#include <random>
enum class AnomalyEffect { Teleport, DoubleEnergy, DisableModule };
class SpaceAnomaly final : public ExternalObject {
public: explicit SpaceAnomaly(unsigned seed=std::random_device{}()); void interact(CommandCenter&,Module&) override; AnomalyEffect lastEffect() const noexcept{return lastEffect_;}
private: std::mt19937 rng_; AnomalyEffect lastEffect_=AnomalyEffect::Teleport;
};
