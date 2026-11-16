#pragma once
class CommandCenter;
class Module;
struct ResourcePackage;
struct Damage;
CommandCenter& operator+(CommandCenter& center, const ResourcePackage& package);
Module& operator-(Module& module, const Damage& damage);
Module& operator*(Module& module, double modifier);
