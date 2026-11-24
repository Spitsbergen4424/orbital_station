#include "command_center/CommandCenter.h"
#include "command_center/Operators.h"
#include "environment/AsteroidField.h"
#include "environment/DeepSpace.h"
#include "environment/EarthOrbit.h"
#include "environment/EnvironmentProxy.h"
#include "factories/StandardModuleFactory.h"
#include "factories/AdvancedModuleFactory.h"
#include "modules/Module.h"
#include "modules/ResearchModule.h"
#include "objects/CargoContainer.h"
#include "objects/MeteorShower.h"
#include "objects/SolarStorm.h"
#include "objects/SpaceAnomaly.h"
#include "objects/Damage.h"
#include <cstdlib>
#include <iostream>
#include <memory>

#define CHECK(condition) do { if (!(condition)) { std::cerr << "CHECK failed: " #condition << " at " << __FILE__ << ":" << __LINE__ << "\n"; std::abort(); } } while (false)

class CountingEnvironment final : public Environment {
public:
    mutable int calls=0;
    void applyEffect(Module&) const override { ++calls; }
    const char* getName() const noexcept override { return "Counting"; }
    double getEnergyModifier() const noexcept override { return 1.0; }
    double getDamageModifier() const noexcept override { return 0.0; }
};

int main(){
    StandardModuleFactory factory;
    CommandCenter center(1000,500,100,8);
    CHECK(center.createModule(factory,ModuleCategory::Energy,"Solar"));
    CHECK(center.createModule(factory,ModuleCategory::Residential,"Crew"));
    CHECK(center.createModule(factory,ModuleCategory::Research,"Lab"));
    CHECK(center.moduleCount()==3);

    // Strategy + Proxy: DeepSpace multiplies energy and damages module.
    Module& energy=*center.modules().begin();
    const int initialEnergy=energy.getEnergyConsumption().getValue();
    center.setEnvironment(std::make_shared<DeepSpace>());
    center.applyCurrentEnvironment();
    CHECK(energy.getEnergyConsumption().getValue()>=initialEnergy);
    CHECK(energy.getDurability().getValue()<energy.getDurability().getMaxValue());

    // AsteroidField blocks research module.
    ResearchModule* lab=nullptr;
    for(auto& m:center.modules()) if((lab=dynamic_cast<ResearchModule*>(&m))) break;
    CHECK(lab);
    center.setEnvironment(std::make_shared<AsteroidField>());
    center.applyCurrentEnvironment();
    CHECK(lab->isResearchBlocked());

    // EarthOrbit clears the temporary research restriction.
    center.setEnvironment(std::make_shared<EarthOrbit>());
    center.applyCurrentEnvironment();
    CHECK(!lab->isResearchBlocked());

    // Proxy cache: identical input snapshot is delegated only once.
    auto counter=std::make_shared<CountingEnvironment>();
    EnvironmentProxy proxy(counter);
    auto cacheModule=factory.createEnergyModule("Cache target");
    proxy.applyEffect(*cacheModule);
    proxy.applyEffect(*cacheModule);
    CHECK(counter->calls==1);
    cacheModule->setState(ModuleState::Emergency);
    proxy.applyEffect(*cacheModule);
    CHECK(counter->calls==1);

    // operator+ adds resources; operator* changes characteristics; operator- damages.
    const double before=center.energy();
    center+ResourcePackage{25,10,2};
    CHECK(center.energy()==before+25);
    const int efficiency=energy.getEfficiency();
    energy*1.5;
    CHECK(energy.getEfficiency()>=efficiency);
    const int durability=energy.getDurability().getValue();
    energy-Damage{3,DamageType::Physical};
    CHECK(energy.getDurability().getValue()<=durability-3);

    // Solar storm strategy selects different algorithms for all four module families.
    auto solarForStorm=factory.createEnergyModule("Solar storm target");
    auto crewForStorm=factory.createResidentialModule("Crew storm target");
    auto labForStorm=factory.createResearchModule("Lab storm target");
    AdvancedModuleFactory advancedFactory;
    auto reactorForStorm=advancedFactory.createEnergyModule("Reactor storm target");
    const int solarHealth=solarForStorm->getDurability().getValue();
    const int solarEfficiency=solarForStorm->getEfficiency();
    const int crewHealth=crewForStorm->getDurability().getValue();
    const int labHealth=labForStorm->getDurability().getValue();
    const int reactorHealth=reactorForStorm->getDurability().getValue();
    SolarStorm familyStorm(9);
    familyStorm.interact(center,*solarForStorm);
    familyStorm.interact(center,*crewForStorm);
    familyStorm.interact(center,*labForStorm);
    familyStorm.interact(center,*reactorForStorm);
    CHECK(solarForStorm->getDurability().getValue()<solarHealth);
    CHECK(solarForStorm->getEfficiency()<solarEfficiency);
    CHECK(crewForStorm->getDurability().getValue()<crewHealth);
    CHECK(labForStorm->getDurability().getValue()<labHealth);
    CHECK(reactorForStorm->getDurability().getValue()<reactorHealth);
    CHECK(labForStorm->getState()==ModuleState::Off);

    // External objects: meteorite physical damage, cargo resources, solar radiation/electronics.
    MeteorShower meteor(2); meteor.interact(center,energy);
    const int radiationBefore=center.radiationLevel();
    SolarStorm storm(2); storm.interact(center,energy);
    CHECK(center.radiationLevel()==radiationBefore+2);
    CargoContainer cargo({10,5,1}); cargo.interact(center,energy);

    // Safety level controls the anomaly response: high security neutralizes it into an energy bonus.
    center.setSafetyLevel(100);
    const double energyBeforeAnomaly=center.energy();
    SpaceAnomaly safeAnomaly(42); safeAnomaly.interact(center,energy);
    CHECK(safeAnomaly.lastEffect()==AnomalyEffect::DoubleEnergy);
    CHECK(center.energy()==energyBeforeAnomaly*2.0);
    // At zero safety both adverse branches remain possible.
    center.setSafetyLevel(0);
    bool sawTeleport=false, sawDisable=false;
    for(unsigned seed=1;seed<100 && !(sawTeleport&&sawDisable);++seed) {
        SpaceAnomaly randomAnomaly(seed); randomAnomaly.interact(center,energy);
        sawTeleport = sawTeleport || randomAnomaly.lastEffect()==AnomalyEffect::Teleport;
        sawDisable = sawDisable || randomAnomaly.lastEffect()==AnomalyEffect::DisableModule;
    }
    CHECK(sawTeleport && sawDisable);

    // Observer: activation and energy-consumption notifications, plus safe deferred destruction.
    auto fragile=factory.createEnergyModule("Fragile");
    const int fragileId=fragile->getId();
    CHECK(center.addModule(std::move(fragile)));
    Module* fragilePtr=center.findModule(fragileId); CHECK(fragilePtr);
    CHECK(fragilePtr->activate());
    const double energyBeforeTick=center.energy();
    center.tick();
    CHECK(center.energy()<=energyBeforeTick);
    fragilePtr=center.findModule(fragileId); CHECK(fragilePtr);
    *fragilePtr-Damage{1000,DamageType::Physical};
    center.flushPendingRemovals();
    CHECK(center.findModule(fragileId)==nullptr);

    // Rule of five: copied center owns deep-cloned modules and moved center rebinds observers.
    CommandCenter copy(center);
    CHECK(copy.moduleCount()==center.moduleCount());
    CommandCenter moved(std::move(copy));
    CHECK(moved.moduleCount()==center.moduleCount());
    Module* movedEnergy=moved.findModule(energy.getId());
    CHECK(movedEnergy);
    moved.setSafetyLevel(100);
    const int movedSafety=moved.safetyLevel();
    movedEnergy->damage(10);
    CHECK(moved.safetyLevel()<movedSafety); // observer перепривязан к новому владельцу


    std::cout<<"All Lab 2 tests passed\n";
}
