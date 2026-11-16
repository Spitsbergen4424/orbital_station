#include "command_center/Operators.h"
#include "command_center/CommandCenter.h"
#include "modules/Module.h"
#include "objects/ResourcePackage.h"
#include "objects/Damage.h"
CommandCenter& operator+(CommandCenter& center, const ResourcePackage& package) { center.addResources(package); return center; }
Module& operator-(Module& module, const Damage& damage) { module.damage(damage.amount); return module; }
Module& operator*(Module& module, double modifier) { module.multiplyCharacteristics(modifier); return module; }
