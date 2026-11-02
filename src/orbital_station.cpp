#include <iostream>
#include <string>
#include <windows.h>


class Module {
public:
    static constexpr int MAX_EFFICIENCY = 100;
    static constexpr int MIN_EFFICIENCY = 0;

    Module(const std::string& name, int efficiency)
        : name_(name), id_(++curId_),efficiency_(0) {
            setEfficiency(efficiency);
        }

    std::string getName() const { return name_; }
    int getEfficiency() const { return efficiency_; }
    size_t getId() const { return id_; }

    void print() const {
        std::cout << "ID: " << id_ << ", " << name_ << ", эффективность " << efficiency_ << "%\n";
    }

    void setEfficiency(int efficiency) {
        efficiency_ = (efficiency > MAX_EFFICIENCY) ? MAX_EFFICIENCY : (efficiency < MIN_EFFICIENCY) ? MIN_EFFICIENCY : efficiency;
    }

private:
    std::string name_;
    int efficiency_;
    size_t id_;
    static size_t curId_; 
};

size_t Module::curId_ = 0;

int main() {
    SetConsoleOutputCP(CP_UTF8);

    Module cabin("Каюта", 85);
    Module solar("Солнечная панель", 70);

    cabin.print();
    solar.print();
}
