#include <iostream>
#include <memory>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#include "command_center/CommandCenter.h"

#include "factories/StandardModuleFactory.h"
#include "factories/AdvancedModuleFactory.h"

#include "logging/ConsoleLogger.h"
#include "logging/LoggingProxy.h"

#include "mediator/StationMediator.h"
#include "facade/StationFacade.h"

int main() {

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // 1. Создаём логгер.
    auto logger = std::make_shared<LoggingProxy>(
        std::make_shared<ConsoleLogger>()
    );

    LoggingProxy::setGlobal(logger);

    // 2. Создаём командный центр.
    CommandCenter center(1200, 600, 90, 10);

    center.setLogger(logger);

    // 3. Создаём фабрики.
    auto standardFactory =
        std::make_shared<StandardModuleFactory>();

    auto advancedFactory =
        std::make_shared<AdvancedModuleFactory>();

    // 4. Создаём посредника.
    auto mediator = std::make_shared<StationMediator>(
        center,
        standardFactory,
        advancedFactory
    );

    // 5. Создаём фасад.
    StationFacade facade(mediator);

    std::cout
        << "\nДобро пожаловать в систему управления станцией!\n"
        << "Введите help для просмотра команд.\n\n";

    // 6. Главный цикл программы.
    std::string input;

    while (facade.isRunning()) {

        std::cout << "> ";

        if (!std::getline(std::cin, input)) {
            break;
        }

        if (input.empty()) {
            continue;
        }

        facade.processInput(input);
    }

    std::cout << "Программа завершена.\n";

    return 0;
}