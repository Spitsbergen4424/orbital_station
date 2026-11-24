#pragma once

#include <memory>
#include <string>

#include "commands/Command.h"

class Handler;

class StationFacade {
public:
    ~StationFacade();
    
    explicit StationFacade(std::shared_ptr<StationMediator> mediator);

    void executeCommand(std::unique_ptr<Command> command);

    void activateModule(int index);
    void deactivateModule(int index);
    void moveModule(int from, int to);
    void repairModule(int index);
    void damageModule(int index);
    void distributeEnergy(int index, int percent);
    void startResearch(int index);
    void buildModule(const std::string& type,
                     const std::string& name);
    void getStatus(int index = -1);

    void processInput(const std::string& input);

    bool isRunning() const;

    void stop();

    private:
    std::shared_ptr<StationMediator> mediator_;
    std::unique_ptr<Handler> handler_;

    bool running_ = true;

    void createHandlers();
};