#include "metadata/ModuleTypeRegistry.h"

std::shared_ptr<const ModuleTypeInfo> ModuleTypeRegistry::getOrCreate(const std::string& type,
    int maxHealth, int baseEnergyConsumption, int baseEfficiency, int cost) {
    auto it = entries_.find(type);
    if (it != entries_.end()) return it->second;
    auto info = std::make_shared<const ModuleTypeInfo>(ModuleTypeInfo{maxHealth, baseEnergyConsumption, baseEfficiency, cost, type});
    entries_.emplace(type, info);
    return info;
}
