cmake_minimum_required(VERSION 3.29)
project(DummyDB)

set(CMAKE_CXX_STANDARD 20)

add_executable(DummyDB main.cpp
        src/Database/Database.cpp
        src/Database/Database.h
        src/Record/Record.cpp
        src/Record/Record.h
        src/Table/Table.cpp
        src/Table/Table.h
        src/Encryption/Encryption.cpp
        src/Encryption/Encryption.h
        src/FileHandler/FileHandler.cpp
        src/FileHandler/FileHandler.h
        src/constants.h
        src/WhereClause/WhereClause.cpp
        src/WhereClause/WhereClause.h
        src/Index/Index.cpp
        src/Index/Index.h
        src/Transaction/Transaction.cpp
        src/Transaction/Transaction.h
        src/SQLParser/SQLParser.cpp
        src/SQLParser/SQLParser.h)

find_package(refl-cpp)

target_link_libraries(DummyDB refl-cpp::refl-cpp)
