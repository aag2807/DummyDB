cmake_minimum_required(VERSION 3.29)
project(DummyDB)

set(CMAKE_CXX_STANDARD 20)

add_executable(DummyDB main.cpp
        src/Database/Database.cpp
        src/Database/Database.h
        src/Record/Record.cpp
        src/Record/Record.h
        src/Table/Table.cpp
        src/Table/Table.h)

find_package(refl-cpp)

target_link_libraries(DummyDB refl-cpp::refl-cpp)
