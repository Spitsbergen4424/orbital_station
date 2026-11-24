#pragma once

#include <memory>
#include <string>
#include <sstream>

class StationFacade;

class Handler {
public:
    virtual ~Handler() = default;

    bool handle(const std::string& input);

    Handler* setNext(std::unique_ptr<Handler> next);

protected:
    explicit Handler(std::string keyword);

    // Наследник реализует конкретное действие
    virtual bool execute(std::istringstream& args) = 0;

    const std::string& keyword() const noexcept { return keyword_; }

private:
    std::string keyword_;
    std::unique_ptr<Handler> next_;
};


class ActivateHandler final : public Handler {
public:
    explicit ActivateHandler(StationFacade& facade);
protected:
    bool execute(std::istringstream& args) override;
private:
    StationFacade& facade_;
};

class DeactivateHandler final : public Handler {
public:
    explicit DeactivateHandler(StationFacade& facade);
protected:
    bool execute(std::istringstream& args) override;
private:
    StationFacade& facade_;
};

class MoveHandler final : public Handler {
public:
    explicit MoveHandler(StationFacade& facade);
protected:
    bool execute(std::istringstream& args) override;
private:
    StationFacade& facade_;
};

class RepairHandler final : public Handler {
public:
    explicit RepairHandler(StationFacade& facade);
protected:
    bool execute(std::istringstream& args) override;
private:
    StationFacade& facade_;
};

class DamageHandler final : public Handler {
public:
    explicit DamageHandler(StationFacade& facade);
protected:
    bool execute(std::istringstream& args) override;
private:
    StationFacade& facade_;
};

class EnergyHandler final : public Handler {
public:
    explicit EnergyHandler(StationFacade& facade);
protected:
    bool execute(std::istringstream& args) override;
private:
    StationFacade& facade_;
};

class ResearchHandler final : public Handler {
public:
    explicit ResearchHandler(StationFacade& facade);
protected:
    bool execute(std::istringstream& args) override;
private:
    StationFacade& facade_;
};

class BuildHandler final : public Handler {
public:
    explicit BuildHandler(StationFacade& facade);
protected:
    bool execute(std::istringstream& args) override;
private:
    StationFacade& facade_;
};

class StatusHandler final : public Handler {
public:
    explicit StatusHandler(StationFacade& facade);
protected:
    bool execute(std::istringstream& args) override;
private:
    StationFacade& facade_;
};

class HelpHandler final : public Handler {
public:
    explicit HelpHandler(StationFacade& facade);
protected:
    bool execute(std::istringstream& args) override;
private:
    StationFacade& facade_; 
};

class ExitHandler final : public Handler {
public:
    explicit ExitHandler(StationFacade& facade);
protected:
    bool execute(std::istringstream& args) override;
private:
    StationFacade& facade_;
};

std::unique_ptr<Handler> createHandlerChain(StationFacade& facade);