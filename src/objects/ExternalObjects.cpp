#include "objects/SolarStorm.h"
#include "objects/MeteorShower.h"
#include "objects/CargoContainer.h"
#include "objects/SpaceAnomaly.h"
#include "objects/ExternalInteractionStrategy.h"
#include "command_center/CommandCenter.h"
#include "command_center/Operators.h"
#include "modules/Module.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include "modules/EnergyModule.h"
#include "modules/ReactorModule.h"
#include "modules/ResidentialModule.h"
#include "modules/ResearchModule.h"

SolarStorm::SolarStorm(int s):ExternalObject("Солнечная буря",s){}
void SolarStorm::interact(CommandCenter& c,Module& m){
    c.log("OBJECT","Солнечная буря: радиация и сбой электроники");
    c.increaseRadiation(strength_);
    std::vector<std::unique_ptr<ExternalInteractionStrategy>> strategies;
    // Подбирается стратегия по семейству модуля; реактор — до EnergyModule.
    if(dynamic_cast<ReactorModule*>(&m)) strategies.push_back(std::make_unique<SolarStormReactorStrategy>());
    else if(dynamic_cast<ResearchModule*>(&m)) strategies.push_back(std::make_unique<SolarStormResearchStrategy>());
    else if(dynamic_cast<ResidentialModule*>(&m)) strategies.push_back(std::make_unique<SolarStormCrewStrategy>());
    else if(dynamic_cast<EnergyModule*>(&m)) strategies.push_back(std::make_unique<SolarStormEnergyStrategy>());
    for(const auto& strategy:strategies) strategy->apply(m,strength_);
    c.flushPendingRemovals();
}
MeteorShower::MeteorShower(int s):ExternalObject("Метеоритный поток",s){}
void MeteorShower::interact(CommandCenter& c,Module& m){ c.log("OBJECT","Метеоритный поток: физический удар по модулю"); m-Damage{strength_,DamageType::Physical}; c.flushPendingRemovals(); }
CargoContainer::CargoContainer(ResourcePackage package):ExternalObject("Грузовой контейнер",1),package_(package){}
void CargoContainer::interact(CommandCenter& c,Module&){ c.log("OBJECT","Грузовой контейнер: получен пакет ресурсов"); c+package_; }
SpaceAnomaly::SpaceAnomaly(unsigned seed):ExternalObject("Космическая аномалия",1),rng_(seed){}
void SpaceAnomaly::interact(CommandCenter& c,Module& m){
    std::uniform_int_distribution<int> safetyRoll(0,199);
    if(safetyRoll(rng_) < c.safetyLevel()*2) lastEffect_=AnomalyEffect::DoubleEnergy;
    else { std::uniform_int_distribution<int> adverse(0,1); lastEffect_=adverse(rng_)==0?AnomalyEffect::Teleport:AnomalyEffect::DisableModule; }
    switch(lastEffect_){
      case AnomalyEffect::Teleport: c.log("OBJECT","Аномалия: телепортация модуля"); m*1.0; c.teleportModule(m); break;
      case AnomalyEffect::DoubleEnergy: c.log("OBJECT","Аномалия: удвоение запаса энергии и усиление модуля"); m*2.0; c.doubleEnergy(); break;
      case AnomalyEffect::DisableModule: c.log("OBJECT","Аномалия: модуль отключён"); m*1.0; m.deactivate(); break;
    }
}
