#include "environment/EnvironmentProxy.h"
#include "logging/LoggingProxy.h"
#include <stdexcept>
#include <utility>
#include "modules/ResearchModule.h"
EnvironmentProxy::EnvironmentProxy(std::shared_ptr<Environment> realEnvironment):realEnvironment_(std::move(realEnvironment)) { if(!realEnvironment_) throw std::invalid_argument("EnvironmentProxy: реальное окружение не задано"); }
std::string EnvironmentProxy::cacheKey(const Module& m) const {
    std::string key=std::to_string(m.getId())+"|"+realEnvironment_->getName()+"|"+std::to_string(m.getDurability().getValue())+"|"+std::to_string(m.getEnergyConsumption().getValue())+"|"+std::to_string(static_cast<int>(m.getState()));
    if(const auto* r=dynamic_cast<const ResearchModule*>(&m)) key+="|R"+std::to_string(r->isResearchBlocked());
    return key;
}
void EnvironmentProxy::applyEffect(Module& module) const {
    if(module.getDurability().getValue()<=0 || module.getState()==ModuleState::Emergency) { LoggingProxy::globalLog("ENV", std::string("Proxy отказал: модуль разрушен/в аварии: ")+module.getName()); return; }
    const std::string key=cacheKey(module);
    if(appliedEffects_.count(key)) { LoggingProxy::globalLog("ENV", std::string("Proxy cache hit, повторный эффект пропущен: ")+module.getName()); return; }
    if(dynamic_cast<ResearchModule*>(&module)) LoggingProxy::globalLog("ENV", "Proxy распознал исследовательский модуль; специальные ограничения будут делегированы окружению");
    LoggingProxy::globalLog("ENV", std::string("Proxy: проверка пройдена, делегирование в ")+realEnvironment_->getName());
    appliedEffects_.insert(key);
    realEnvironment_->applyEffect(module);
}
const char* EnvironmentProxy::getName() const noexcept{return realEnvironment_->getName();}
double EnvironmentProxy::getEnergyModifier() const noexcept{return realEnvironment_->getEnergyModifier();}
double EnvironmentProxy::getDamageModifier() const noexcept{return realEnvironment_->getDamageModifier();}
