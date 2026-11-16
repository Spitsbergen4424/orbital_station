#include "objects/ExternalInteractionStrategy.h"
#include "command_center/Operators.h"
#include <algorithm>
#include "modules/EnergyModule.h"
#include "modules/ReactorModule.h"
#include "modules/ResidentialModule.h"
#include "modules/ResearchModule.h"
void SolarStormEnergyStrategy::apply(Module& m,int s) const { if(dynamic_cast<EnergyModule*>(&m)&&!dynamic_cast<ReactorModule*>(&m)) { m-Damage{s,DamageType::Electrical}; m*0.8; } }
void SolarStormCrewStrategy::apply(Module& m,int s) const { if(dynamic_cast<ResidentialModule*>(&m)) m-Damage{std::max(1,s/2),DamageType::Radiation}; }
void SolarStormResearchStrategy::apply(Module& m,int s) const { if(dynamic_cast<ResearchModule*>(&m)) { m.deactivate(); m-Damage{s,DamageType::Electrical}; } }
void SolarStormReactorStrategy::apply(Module& m,int s) const { if(dynamic_cast<ReactorModule*>(&m)) { m-Damage{std::max(1,s/3),DamageType::Electrical}; m*0.9; } }
