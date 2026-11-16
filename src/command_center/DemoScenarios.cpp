#include "command_center/DemoScenarios.h"
#include "command_center/CommandCenter.h"
#include "command_center/Operators.h"
#include "environment/EarthOrbit.h"
#include "environment/DeepSpace.h"
#include "factories/StandardModuleFactory.h"
#include "logging/ConsoleLogger.h"
#include "logging/LoggingProxy.h"
#include "objects/SolarStorm.h"
#include "objects/MeteorShower.h"
#include "objects/CargoContainer.h"
#include "objects/SpaceAnomaly.h"
#include "modules/Module.h"
#include <iostream>
#include <memory>

void runLab2Scenarios() {
    auto logger=std::make_shared<LoggingProxy>(std::make_shared<ConsoleLogger>());
    LoggingProxy::setGlobal(logger);
    StandardModuleFactory factory;
    CommandCenter center(1200.0,600.0,90,8);
    center.setLogger(logger);

    std::cout << "\n========== ЛР2 / Сценарий 1: командный центр и Орбита Земли ==========\n";
    center.createModule(factory,ModuleCategory::Residential,"Каюта экипажа",20);
    center.createModule(factory,ModuleCategory::Energy,"Солнечная панель",30);
    center.createModule(factory,ModuleCategory::Research,"Биолаборатория",40);
    center.setEnvironment(std::make_shared<EarthOrbit>());
    center.applyCurrentEnvironment();
    center.printStatistics();

    std::cout << "\n========== ЛР2 / Сценарий 2: переход в глубокий космос ==========\n";
    center.setEnvironment(std::make_shared<DeepSpace>());
    center.applyCurrentEnvironment();
    for(auto& module:center.modules()) module.activate();
    center.tick();
    center.printStatistics();

    std::cout << "\n========== ЛР2 / Сценарий 3: солнечная буря и Observer ==========\n";
    if(center.moduleCount()>0) { Module& target=*center.modules().begin(); SolarStorm storm(18); storm.interact(center,target); }
    center.printStatistics();

    std::cout << "\n========== ЛР2 / Сценарий 4: грузовой контейнер и operator+ ==========\n";
    if(center.moduleCount()>0) { Module& target=*center.modules().begin(); CargoContainer cargo({350.0,180.0,8}); cargo.interact(center,target); }
    center.printStatistics();

    std::cout << "\n========== ЛР2 / Сценарий 5: космическая аномалия ==========\n";
    if(center.moduleCount()>0) { Module& target=*center.modules().begin(); SpaceAnomaly anomaly(7); anomaly.interact(center,target); }
    center.printStatistics();
}
