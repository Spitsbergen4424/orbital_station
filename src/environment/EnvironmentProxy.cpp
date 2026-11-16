#include "environment/EnvironmentProxy.h"

#include <iostream>
#include <stdexcept>
#include <utility>

EnvironmentProxy::EnvironmentProxy(std::shared_ptr<Environment> realEnvironment)
    : realEnvironment_(std::move(realEnvironment)) {
    if (!realEnvironment_) {
        throw std::invalid_argument("EnvironmentProxy: реальное окружение не задано");
    }
}

void EnvironmentProxy::applyEffect(Module& module) const {
    // Не воздействуем на уже разрушенный или аварийный модуль.
    if (module.getDurability().getValue() <= 0 ||
        module.getState() == ModuleState::Emergency) {
        std::cout << "[PROXY] Воздействие окружения \"" << realEnvironment_->getName()
                  << "\" пропущено: модуль \"" << module.getName()
                  << "\" находится в аварийном состоянии\n";
        return;
    }

    std::cout << "[PROXY] Проверка пройдена для модуля \"" << module.getName()
              << "\". Делегирование окружению \"" << realEnvironment_->getName()
              << "\"\n";
    realEnvironment_->applyEffect(module);
}

const char* EnvironmentProxy::getName() const noexcept {
    return realEnvironment_->getName();
}

double EnvironmentProxy::getEnergyModifier() const noexcept {
    return realEnvironment_->getEnergyModifier();
}

double EnvironmentProxy::getDamageModifier() const noexcept {
    return realEnvironment_->getDamageModifier();
}
