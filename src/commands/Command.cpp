#include "commands/Command.h"

ActivateModuleCommand::ActivateModuleCommand(
    std::shared_ptr<StationMediator> mediator, int index)
    : mediator_(mediator), index_(index) {}

void ActivateModuleCommand::execute() {
    mediator_->activateModule(index_);
}

DeactivateModuleCommand::DeactivateModuleCommand(
    std::shared_ptr<StationMediator> mediator, int index)
    : mediator_(mediator), index_(index) {}

void DeactivateModuleCommand::execute() {
    mediator_->deactivateModule(index_);
}

MoveModuleCommand::MoveModuleCommand(
    std::shared_ptr<StationMediator> mediator, int from, int to)
    : mediator_(mediator), from_(from), to_(to) {}

void MoveModuleCommand::execute() {
    mediator_->moveModule(from_, to_);
}

RepairModuleCommand::RepairModuleCommand(
    std::shared_ptr<StationMediator> mediator, int index)
    : mediator_(mediator), index_(index) {}

void RepairModuleCommand::execute() {
    mediator_->repairModule(index_);
}

DistributeEnergyCommand::DistributeEnergyCommand(
    std::shared_ptr<StationMediator> mediator, int index, int percent)
    : mediator_(mediator), index_(index), percent_(percent) {}

void DistributeEnergyCommand::execute() {
    mediator_->distributeEnergy(index_, percent_);
}

StartResearchCommand::StartResearchCommand(
    std::shared_ptr<StationMediator> mediator, int index)
    : mediator_(mediator), index_(index) {}

void StartResearchCommand::execute() {
    mediator_->startResearch(index_);
}

BuildModuleCommand::BuildModuleCommand(
    std::shared_ptr<StationMediator> mediator,
    const std::string& type,
    const std::string& name)
    : mediator_(mediator), type_(type), name_(name) {}

void BuildModuleCommand::execute() {
    mediator_->buildModule(type_, name_);
}

StatusCommand::StatusCommand(
    std::shared_ptr<StationMediator> mediator, int index)
    : mediator_(mediator), index_(index) {}

void StatusCommand::execute() {
    mediator_->getStatus(index_);
}