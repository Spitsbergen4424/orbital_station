#pragma once
#include "objects/ExternalObject.h"
#include "objects/ResourcePackage.h"
class CargoContainer final : public ExternalObject { public: explicit CargoContainer(ResourcePackage package={250.0,100.0,5}); void interact(CommandCenter&,Module&) override; private: ResourcePackage package_; };
