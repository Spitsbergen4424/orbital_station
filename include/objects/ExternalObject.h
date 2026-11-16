#pragma once
#include <string>
#include <utility>
class CommandCenter; class Module;
class ExternalObject {
public:
    ExternalObject(std::string type,int strength):type_(std::move(type)),strength_(strength<0?0:strength){}
    virtual ~ExternalObject()=default;
    const std::string& type() const noexcept{return type_;}
    int strength() const noexcept{return strength_;}
    virtual void interact(CommandCenter& center,Module& module)=0;
protected: std::string type_; int strength_;
};
