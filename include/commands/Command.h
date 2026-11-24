#pragma once

#include <memory>
#include <string>

#include "mediator/StationMediator.h"

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

class ActivateModuleCommand : public Command {
public:
    ActivateModuleCommand(std::shared_ptr<StationMediator> mediator,
                           int index);

    void execute() override;

private:
    std::shared_ptr<StationMediator> mediator_;
    int index_;
};

class DeactivateModuleCommand : public Command {
public:
    DeactivateModuleCommand(std::shared_ptr<StationMediator> mediator,
                            int index);

    void execute() override;

private:
    std::shared_ptr<StationMediator> mediator_;
    int index_;
};

class MoveModuleCommand : public Command {
public:
    MoveModuleCommand(std::shared_ptr<StationMediator> mediator,
                      int from, int to);

    void execute() override;

private:
    std::shared_ptr<StationMediator> mediator_;
    int from_;
    int to_;
};

class RepairModuleCommand : public Command {
public:
    RepairModuleCommand(std::shared_ptr<StationMediator> mediator,
                        int index);

    void execute() override;

private:
    std::shared_ptr<StationMediator> mediator_;
    int index_;
};

class DistributeEnergyCommand : public Command {
public:
    DistributeEnergyCommand(std::shared_ptr<StationMediator> mediator,
                            int index, int percent);

    void execute() override;

private:
    std::shared_ptr<StationMediator> mediator_;
    int index_;
    int percent_;
};

class StartResearchCommand : public Command {
public:
    StartResearchCommand(std::shared_ptr<StationMediator> mediator,
                         int index);

    void execute() override;

private:
    std::shared_ptr<StationMediator> mediator_;
    int index_;
};

class BuildModuleCommand : public Command {
public:
    BuildModuleCommand(std::shared_ptr<StationMediator> mediator,
                       const std::string& type,
                       const std::string& name);

    void execute() override;

private:
    std::shared_ptr<StationMediator> mediator_;
    std::string type_;
    std::string name_;
};

class StatusCommand : public Command {
public:
    StatusCommand(std::shared_ptr<StationMediator> mediator,
                  int index = -1);

    void execute() override;

private:
    std::shared_ptr<StationMediator> mediator_;
    int index_;
};