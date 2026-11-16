#pragma once
#include "objects/ExternalObject.h"
class SolarStorm final : public ExternalObject { public: explicit SolarStorm(int strength=12); void interact(CommandCenter&,Module&) override; };
