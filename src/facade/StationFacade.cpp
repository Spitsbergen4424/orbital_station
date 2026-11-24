#include "facade/StationFacade.h"
#include "handlers/Handler.h"
#include "logging/LoggingProxy.h"

#include <iostream>

StationFacade::~StationFacade() = default;

StationFacade::StationFacade(
    std::shared_ptr<StationMediator> mediator
)
    : mediator_(mediator) {

    createHandlers();
}

void StationFacade::executeCommand(
    std::unique_ptr<Command> command
) {
    if (command) {
        LoggingProxy::globalLog(
            "COMMAND",
            "Выполнение пользовательской команды"
        );

        command->execute();
    }
}

void StationFacade::activateModule(int index) {
    executeCommand(
        std::make_unique<ActivateModuleCommand>(mediator_, index)
    );
}

void StationFacade::deactivateModule(int index) {
    executeCommand(
        std::make_unique<DeactivateModuleCommand>(mediator_, index)
    );
}

void StationFacade::moveModule(int from, int to) {
    executeCommand(
        std::make_unique<MoveModuleCommand>(mediator_, from, to)
    );
}

void StationFacade::repairModule(int index) {
    executeCommand(
        std::make_unique<RepairModuleCommand>(mediator_, index)
    );
}

void StationFacade::damageModule(int index) {
    executeCommand(
        std::make_unique<DamageModuleCommand>(mediator_, index)
    );
}

void StationFacade::distributeEnergy(int index, int percent) {
    executeCommand(
        std::make_unique<DistributeEnergyCommand>(
            mediator_, index, percent
        )
    );
}

void StationFacade::startResearch(int index) {
    executeCommand(
        std::make_unique<StartResearchCommand>(mediator_, index)
    );
}

void StationFacade::buildModule(
    const std::string& type,
    const std::string& name
) {
    executeCommand(
        std::make_unique<BuildModuleCommand>(
            mediator_, type, name
        )
    );
}

void StationFacade::getStatus(int index) {
    executeCommand(
        std::make_unique<StatusCommand>(mediator_, index)
    );
}

void StationFacade::processInput(const std::string& input) {
    if (!handler_->handle(input)) {
        std::cout << "Неизвестная команда. Введите help.\n";
    }
}

bool StationFacade::isRunning() const {
    return running_;
}

void StationFacade::createHandlers() {
    handler_ = createHandlerChain(*this);
}

void StationFacade::stop() {
    running_ = false;
    std::cout << "Завершение работы станции.\n";
}