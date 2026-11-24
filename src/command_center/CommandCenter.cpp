#include "command_center/CommandCenter.h"
#include "environment/EnvironmentProxy.h"
#include "modules/Module.h"

#include "logging/LoggingProxy.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <random>

CommandCenter::CommandCenter(double energy,double oxygen,int safetyLevel,std::size_t maxModules)
 : energy_(std::max(0.0,energy)), oxygen_(std::max(0.0,oxygen)), radiationLevel_(0), safetyLevel_(std::clamp(safetyLevel,0,100)), maxModules_(maxModules), modules_(maxModules) {
    if(maxModules==0) throw std::invalid_argument("CommandCenter: maxModules must be positive");
}
CommandCenter::CommandCenter(const CommandCenter& o)
 : energy_(o.energy_), oxygen_(o.oxygen_), radiationLevel_(o.radiationLevel_), safetyLevel_(o.safetyLevel_), maxModules_(o.maxModules_), modules_(o.modules_), currentEnvironment_(o.currentEnvironment_), typeRegistry_(o.typeRegistry_), logger_(o.logger_) { bindObservers(); }
CommandCenter& CommandCenter::operator=(const CommandCenter& o) {
    if(this!=&o) { unbindObservers(); energy_=o.energy_; oxygen_=o.oxygen_; radiationLevel_=o.radiationLevel_; safetyLevel_=o.safetyLevel_; maxModules_=o.maxModules_; modules_=o.modules_; currentEnvironment_=o.currentEnvironment_; typeRegistry_=o.typeRegistry_; pendingRemoval_.clear(); logger_=o.logger_; bindObservers(); }
    return *this;
}
CommandCenter::CommandCenter(CommandCenter&& o) noexcept
 : energy_(o.energy_), oxygen_(o.oxygen_), radiationLevel_(o.radiationLevel_), safetyLevel_(o.safetyLevel_), maxModules_(o.maxModules_), modules_(std::move(o.modules_)), currentEnvironment_(std::move(o.currentEnvironment_)), typeRegistry_(std::move(o.typeRegistry_)), pendingRemoval_(std::move(o.pendingRemoval_)), logger_(std::move(o.logger_)) {
    for(auto& m:modules_) { m.unsubscribe(&o); m.subscribe(this); }
    o.pendingRemoval_.clear();
}
CommandCenter& CommandCenter::operator=(CommandCenter&& o) noexcept {
    if(this!=&o) { unbindObservers(); energy_=o.energy_; oxygen_=o.oxygen_; radiationLevel_=o.radiationLevel_; safetyLevel_=o.safetyLevel_; maxModules_=o.maxModules_; modules_=std::move(o.modules_); currentEnvironment_=std::move(o.currentEnvironment_); typeRegistry_=std::move(o.typeRegistry_); pendingRemoval_=std::move(o.pendingRemoval_); logger_=std::move(o.logger_); for(auto& m:modules_) {m.unsubscribe(&o);m.subscribe(this);} o.pendingRemoval_.clear(); }
    return *this;
}
CommandCenter::~CommandCenter(){ unbindObservers(); }
void CommandCenter::bindObservers(){ for(auto& m:modules_) m.subscribe(this); }
void CommandCenter::unbindObservers() noexcept { for(auto& m:modules_) m.unsubscribe(this); }
bool CommandCenter::addModule(std::unique_ptr<Module> module,int cost) {
    if(!module || modules_.size()>=maxModules_ || energy_<cost || cost<0) return false;
    auto info=typeRegistry_.getOrCreate(module->getType(),module->getDurability().getMaxValue(),module->getEnergyConsumption().getValue(),module->getEfficiency(),cost);
    module->setTypeInfo(std::move(info));
    module->subscribe(this);
    if(!modules_.addModule(std::move(module))) return false;
    energy_-=cost; log("CENTER","Добавлен модуль; списана стоимость " + std::to_string(cost)); return true;
}
bool CommandCenter::createModule(const ModuleFactory& factory, ModuleCategory category, const std::string& name, int cost) {
    std::unique_ptr<Module> module;
    switch(category) {
      case ModuleCategory::Residential: module=factory.createResidentialModule(name); break;
      case ModuleCategory::Energy: module=factory.createEnergyModule(name); break;
      case ModuleCategory::Research: module=factory.createResearchModule(name); break;
    }
    return addModule(std::move(module),cost);
}
bool CommandCenter::removeModule(int id){ if(auto* m=findModule(id)) m->unsubscribe(this); return modules_.removeById(id); }
Module* CommandCenter::findModule(int id) noexcept { return modules_.findById(id); }
void CommandCenter::setEnvironment(std::shared_ptr<Environment> env) {
    if(!env) throw std::invalid_argument("CommandCenter: environment is null");
    currentEnvironment_=std::make_shared<EnvironmentProxy>(std::move(env));
    log("CENTER",std::string("Установлено окружение: ")+currentEnvironment_->getName());
}
void CommandCenter::applyCurrentEnvironment(){ if(!currentEnvironment_) {log("CENTER","Окружение не задано");return;} std::vector<int> ids; for(const auto& m:modules_) ids.push_back(m.getId()); for(int id:ids) if(auto* m=findModule(id)) currentEnvironment_->applyEffect(*m); flushPendingRemovals(); }
void CommandCenter::tick(){
    for(auto& m:modules_) {
        if(m.getState()!=ModuleState::On) continue;
        const double use=m.getEnergyConsumption().getValue();
        if(energy_>=use) { energy_-=use; m.notify({ModuleEventType::EnergyConsumed,m.getId(),static_cast<int>(use),std::chrono::system_clock::now()}); }
        else { m.deactivate(); log("CENTER","Недостаточно энергии: модуль "+m.getName()+" отключён"); }
        if(m.consumesOxygen()) oxygen_=std::max(0.0,oxygen_-1.0);
    }
    flushPendingRemovals();
}
void CommandCenter::onModuleEvent(const ModuleEvent& e){
    switch(e.type){
        case ModuleEventType::Activated: log("MODULE","Модуль #"+std::to_string(e.moduleId)+" активирован"); break;
        case ModuleEventType::Deactivated: log("MODULE","Модуль #"+std::to_string(e.moduleId)+" отключён"); break;
        case ModuleEventType::Damaged: safetyLevel_=std::max(0,safetyLevel_-std::max(1,e.damage/10)); log("MODULE","Модуль #"+std::to_string(e.moduleId)+" повреждён на "+std::to_string(e.damage)+"; безопасность снижена"); break;
        case ModuleEventType::Destroyed: pendingRemoval_.insert(e.moduleId); log("MODULE","Модуль #"+std::to_string(e.moduleId)+" разрушен; запланировано удаление"); break;
        case ModuleEventType::EnergyConsumed: log("MODULE","Модуль #"+std::to_string(e.moduleId)+" потребил "+std::to_string(e.damage)+" ед. энергии"); break;
        case ModuleEventType::Modified: log("MODULE","Характеристики модуля #"+std::to_string(e.moduleId)+" изменены"); break;
    }
}
void CommandCenter::flushPendingRemovals(){ for(int id:pendingRemoval_) modules_.removeById(id); pendingRemoval_.clear(); }
void CommandCenter::addResources(const ResourcePackage& r){ energy_=std::max(0.0,energy_+r.energy); oxygen_=std::max(0.0,oxygen_+r.oxygen); setSafetyLevel(safetyLevel_+r.safetyLevel); log("CENTER","Получены ресурсы: энергия +"+std::to_string(r.energy)+", кислород +"+std::to_string(r.oxygen)); }
void CommandCenter::addEnergy(double amount){ energy_=std::max(0.0,energy_+amount); }
void CommandCenter::consumeEnergy(double amount){ energy_=std::max(0.0,energy_-std::max(0.0,amount)); }
void CommandCenter::consumeOxygen(double amount){ oxygen_=std::max(0.0,oxygen_-std::max(0.0,amount)); }
void CommandCenter::increaseRadiation(int amount) noexcept { radiationLevel_=std::max(0,radiationLevel_+std::max(0,amount)); }
void CommandCenter::setSafetyLevel(int v) noexcept { safetyLevel_=std::clamp(v,0,100); }
void CommandCenter::teleportModule(Module& module){ int from=modules_.indexOf(module.getId()); if(from<0||modules_.size()<2)return; const auto to=(static_cast<std::size_t>(from)+1)%modules_.size(); modules_.moveModule(static_cast<std::size_t>(from),to); log("OBJECT","Модуль "+module.getName()+" телепортирован в другую секцию станции"); }
void CommandCenter::printStatistics() const { std::cout<<"\n[СТАТИСТИКА КОМАНДНОГО ЦЕНТРА]\nЭнергия: "<<energy_<<"\nКислород: "<<oxygen_<<"\nРадиация: "<<radiationLevel_<<"\nБезопасность: "<<safetyLevel_<<"/100\nМодули: "<<modules_.size()<<"/"<<maxModules_<<"\nОкружение: "<<(currentEnvironment_?currentEnvironment_->getName():"не задано")<<"\nFlyweight-типов: "<<typeRegistry_.size()<<"\n"; }
void CommandCenter::setLogger(std::shared_ptr<LoggingProxy> logger){ logger_=std::move(logger); if(logger_) LoggingProxy::setGlobal(logger_); }
void CommandCenter::log(const std::string& src,const std::string& msg) const { const std::string line="["+src+"] "+msg; if(logger_) logger_->write(line); else LoggingProxy::globalLog(src,msg); }
