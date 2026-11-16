#pragma once
#include "modules/Module.h"
#include "objects/Damage.h"
class ExternalInteractionStrategy {
public: virtual ~ExternalInteractionStrategy()=default; virtual void apply(Module& module,int strength) const=0;
};
class SolarStormEnergyStrategy final : public ExternalInteractionStrategy { public: void apply(Module&,int) const override; };
class SolarStormCrewStrategy final : public ExternalInteractionStrategy { public: void apply(Module&,int) const override; };
class SolarStormResearchStrategy final : public ExternalInteractionStrategy { public: void apply(Module&,int) const override; };
class SolarStormReactorStrategy final : public ExternalInteractionStrategy { public: void apply(Module&,int) const override; };
