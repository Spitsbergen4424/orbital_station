#include "facade/StationFacade.h"

#include "handlers/Handler.h"

#include <iostream>

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
        command->execute();
    }
}

void StationFacade::processInput(const std::string& input) {
    if (!handler_->handle(input)) {
        std::cout << "Неизвестная команда. Введите help.\n";
    }
}

bool StationFacade::isRunning() const {
    return running_;
}

std::shared_ptr<StationMediator> StationFacade::getMediator() const {
    return mediator_;
}

void StationFacade::createHandlers() {
    handler_ = createHandlerChain(*this);
}

void StationFacade::stop() {
    running_ = false;
    std::cout << "Завершение работы станции.\n";
}