#pragma once

#include <memory>
#include <string>
#include <vector>

#include "command_center/CommandCenter.h"
#include "factories/ModuleFactory.h"

class StationMediator {
public:
    StationMediator(
        CommandCenter& center,
        std::shared_ptr<ModuleFactory> standardFactory,
        std::shared_ptr<ModuleFactory> advancedFactory
    );

    void activateModule(int index);
    void deactivateModule(int index);
    void moveModule(int from, int to);
    void repairModule(int index);

    void distributeEnergy(int index, int percent);
    bool requestEnergy(int index, double amount);

    void startResearch(int index);
    void buildModule(const std::string& type,
                     const std::string& name);

    void getStatus(int index = -1) const;

private:
    Module* getModule(int index);

    CommandCenter& center_;

    std::shared_ptr<ModuleFactory> standardFactory_;
    std::shared_ptr<ModuleFactory> advancedFactory_;

    std::vector<double> energyDistribution_;
};