#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "metadata/ModuleTypeInfo.h"

class ModuleTypeRegistry {
public:
    std::shared_ptr<const ModuleTypeInfo> getOrCreate(const std::string& type,
        int maxHealth, int baseEnergyConsumption, int baseEfficiency, int cost);
    std::size_t size() const noexcept { return entries_.size(); }
private:
    std::unordered_map<std::string, std::shared_ptr<const ModuleTypeInfo>> entries_;
};
