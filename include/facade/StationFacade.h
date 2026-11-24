#pragma once

#include <memory>
#include <string>

#include "commands/Command.h"

class Handler;

class StationFacade {
public:
    explicit StationFacade(std::shared_ptr<StationMediator> mediator);

    void executeCommand(std::unique_ptr<Command> command);

    void processInput(const std::string& input);

    bool isRunning() const;

    void stop();

    std::shared_ptr<StationMediator> getMediator() const;

private:
    std::shared_ptr<StationMediator> mediator_;
    std::unique_ptr<Handler> handler_;

    bool running_ = true;

    void createHandlers();
};