#include "handlers/Handler.h"

#include "facade/StationFacade.h"
#include "commands/Command.h"

#include <iomanip>
#include <iostream>
#include <utility>

Handler::Handler(std::string keyword)
    : keyword_(std::move(keyword)) {}

Handler* Handler::setNext(std::unique_ptr<Handler> next) {
    next_ = std::move(next);
    return next_.get();
}

bool Handler::handle(const std::string& input) {
    std::istringstream stream(input);

    std::string command;
    stream >> command;

    if (command == keyword_) {
        return execute(stream);
    }

    if (next_) {
        return next_->handle(input);
    }

    return false;
}


namespace {
bool readInt(std::istringstream& stream, int& value) {
    return static_cast<bool>(stream >> value);
}
}

ActivateHandler::ActivateHandler(StationFacade& facade)
    : Handler("activate"), facade_(facade) {}

bool ActivateHandler::execute(std::istringstream& args) {
    int index;
    if (!readInt(args, index)) {
        std::cout << "Использование: activate <индекс>\n";
        return true;
    }

    facade_.executeCommand(
        std::make_unique<ActivateModuleCommand>(
            facade_.getMediator(), index));
    return true;
}

DeactivateHandler::DeactivateHandler(StationFacade& facade)
    : Handler("deactivate"), facade_(facade) {}

bool DeactivateHandler::execute(std::istringstream& args) {
    int index;
    if (!readInt(args, index)) {
        std::cout << "Использование: deactivate <индекс>\n";
        return true;
    }

    facade_.executeCommand(
        std::make_unique<DeactivateModuleCommand>(
            facade_.getMediator(), index));
    return true;
}

MoveHandler::MoveHandler(StationFacade& facade)
    : Handler("move"), facade_(facade) {}

bool MoveHandler::execute(std::istringstream& args) {
    int from, to;
    if (!readInt(args, from) || !readInt(args, to)) {
        std::cout << "Использование: move <откуда> <куда>\n";
        return true;
    }

    facade_.executeCommand(
        std::make_unique<MoveModuleCommand>(
            facade_.getMediator(), from, to));
    return true;
}

RepairHandler::RepairHandler(StationFacade& facade)
    : Handler("repair"), facade_(facade) {}

bool RepairHandler::execute(std::istringstream& args) {
    int index;
    if (!readInt(args, index)) {
        std::cout << "Использование: repair <индекс>\n";
        return true;
    }

    facade_.executeCommand(
        std::make_unique<RepairModuleCommand>(
            facade_.getMediator(), index));
    return true;
}

EnergyHandler::EnergyHandler(StationFacade& facade)
    : Handler("energy"), facade_(facade) {}

bool EnergyHandler::execute(std::istringstream& args) {
    int index, percent;
    if (!readInt(args, index) || !readInt(args, percent)) {
        std::cout << "Использование: energy <индекс> <процент>\n";
        return true;
    }

    facade_.executeCommand(
        std::make_unique<DistributeEnergyCommand>(
            facade_.getMediator(), index, percent));
    return true;
}

ResearchHandler::ResearchHandler(StationFacade& facade)
    : Handler("research"), facade_(facade) {}

bool ResearchHandler::execute(std::istringstream& args) {
    int index;
    if (!readInt(args, index)) {
        std::cout << "Использование: research <индекс>\n";
        return true;
    }

    facade_.executeCommand(
        std::make_unique<StartResearchCommand>(
            facade_.getMediator(), index));
    return true;
}

BuildHandler::BuildHandler(StationFacade& facade)
    : Handler("build"), facade_(facade) {}

bool BuildHandler::execute(std::istringstream& args) {
    std::string type;
    std::string name;
    if (!(args >> type >> std::quoted(name))) {
        std::cout << "Использование: build <тип> <имя>\n";
        return true;
    }

    facade_.executeCommand(
        std::make_unique<BuildModuleCommand>(
            facade_.getMediator(), type, name));
    return true;
}

StatusHandler::StatusHandler(StationFacade& facade)
    : Handler("status"), facade_(facade) {}

bool StatusHandler::execute(std::istringstream& args) {
    int index = -1;
    args >> index;   // необязательный аргумент: если нет — остаётся -1

    facade_.executeCommand(
        std::make_unique<StatusCommand>(
            facade_.getMediator(), index));
    return true;
}

HelpHandler::HelpHandler(StationFacade& facade)
    : Handler("help"), facade_(facade) {}

bool HelpHandler::execute(std::istringstream&) {
    std::cout
        << "\nДоступные команды:\n"
        << "  activate <индекс>\n"
        << "  deactivate <индекс>\n"
        << "  move <откуда> <куда>\n"
        << "  repair <индекс>\n"
        << "  energy <индекс> <процент>\n"
        << "  research <индекс>\n"
        << "  build <тип> <имя>\n"
        << "  status [индекс]\n"
        << "  help\n"
        << "  exit\n\n";
    return true;
}

ExitHandler::ExitHandler(StationFacade& facade)
    : Handler("exit"), facade_(facade) {}

bool ExitHandler::execute(std::istringstream&) {
    facade_.stop();
    return true;
}

std::unique_ptr<Handler> createHandlerChain(StationFacade& facade) {
    auto head = std::make_unique<ActivateHandler>(facade);

    // setNext возвращает указатель на только что добавленный узел,
    // поэтому цепочку можно собирать в одну строку.
    head->setNext(std::make_unique<DeactivateHandler>(facade))
        ->setNext(std::make_unique<MoveHandler>(facade))
        ->setNext(std::make_unique<RepairHandler>(facade))
        ->setNext(std::make_unique<EnergyHandler>(facade))
        ->setNext(std::make_unique<ResearchHandler>(facade))
        ->setNext(std::make_unique<BuildHandler>(facade))
        ->setNext(std::make_unique<StatusHandler>(facade))
        ->setNext(std::make_unique<HelpHandler>(facade))
        ->setNext(std::make_unique<ExitHandler>(facade));

    return head;
}