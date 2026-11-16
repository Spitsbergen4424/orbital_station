#pragma once
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>
#include "observer/IObserver.h"
#include "station/Station.h"
#include "environment/Environment.h"
#include "metadata/ModuleTypeRegistry.h"
#include "objects/ResourcePackage.h"
#include "factories/ModuleFactory.h"

class LoggingProxy;
enum class ModuleCategory { Residential, Energy, Research };
class CommandCenter : public IObserver {
public:
    explicit CommandCenter(double energy = 1000.0, double oxygen = 500.0, int safetyLevel = 100, std::size_t maxModules = 10);
    CommandCenter(const CommandCenter& other);
    CommandCenter& operator=(const CommandCenter& other);
    CommandCenter(CommandCenter&& other) noexcept;
    CommandCenter& operator=(CommandCenter&& other) noexcept;
    ~CommandCenter();

    bool addModule(std::unique_ptr<Module> module, int cost = 0);
    bool createModule(const ModuleFactory& factory, ModuleCategory category, const std::string& name, int cost = 0);
    bool removeModule(int id);
    Module* findModule(int id) noexcept;
    Station& modules() noexcept { return modules_; }
    const Station& modules() const noexcept { return modules_; }
    std::size_t moduleCount() const noexcept { return modules_.size(); }
    std::size_t maxModules() const noexcept { return maxModules_; }

    void setEnvironment(std::shared_ptr<Environment> environment);
    void applyCurrentEnvironment();
    const Environment* currentEnvironment() const noexcept { return currentEnvironment_.get(); }
    void tick();
    void onModuleEvent(const ModuleEvent& event) override;
    void flushPendingRemovals();

    void addResources(const ResourcePackage& package);
    void addEnergy(double amount);
    void doubleEnergy() noexcept { energy_ *= 2.0; }
    void increaseRadiation(int amount) noexcept;
    int radiationLevel() const noexcept { return radiationLevel_; }
    void consumeEnergy(double amount);
    void consumeOxygen(double amount);
    void teleportModule(Module& module);
    double energy() const noexcept { return energy_; }
    double oxygen() const noexcept { return oxygen_; }
    int safetyLevel() const noexcept { return safetyLevel_; }
    void setSafetyLevel(int value) noexcept;
    void printStatistics() const;
    void setLogger(std::shared_ptr<LoggingProxy> logger);
    void log(const std::string& source, const std::string& message) const;

private:
    void bindObservers();
    void unbindObservers() noexcept;
    double energy_;
    double oxygen_;
    int radiationLevel_ = 0;
    int safetyLevel_;
    std::size_t maxModules_;
    Station modules_;
    std::shared_ptr<Environment> currentEnvironment_;
    ModuleTypeRegistry typeRegistry_;
    std::unordered_set<int> pendingRemoval_;
    std::shared_ptr<LoggingProxy> logger_;
};
