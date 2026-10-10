cmake_minimum_required(VERSION 3.15)
project(OrbitalStation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(orbital_station
    main.cpp
    Logger.cpp
    Durability.cpp
    EnergyConsumption.cpp
    Capacity.cpp
    Module.cpp
    ResidentialModules.cpp
    EnergyModules.cpp
    ResearchModules.cpp
    ModuleFactory.cpp
    StationIterator.cpp
    Station.cpp
    Tests.cpp
)

if(MSVC)
    target_compile_options(orbital_station PRIVATE /W4 /utf-8 /permissive-)
else()
    target_compile_options(orbital_station PRIVATE -Wall -Wextra -pedantic)
endif()
