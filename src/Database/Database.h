#ifndef DUMMYDB_DATABASE_H
#define DUMMYDB_DATABASE_H

#include <map>
#include <string>
#include <memory>
#include "../Table/Table.h"
#include "../Encryption/Encryption.h"
#include "../FileHandler/FileHandler.h"

class Database
{
private:
    FileHandler file_handler;
    std::vector<std::string> current_columns;
    std::string current_table;

    bool tableExists(const std::string &tableName);

    std::vector<std::string> splitString(const std::string &str, char delimiter);

public:
    Database(const std::string &filename) : file_handler(filename)
    {}

    bool createTable(const std::string &tableName, const std::vector<std::string> &columns);

    bool insertRecord(const std::string &tableName, const std::vector<std::string> &values);

    void selectAll(const std::string &tableName);

    void deleteRecords(const std::string &tableName, const std::string &condition);

    void updateRecords(const std::string &tableName, const std::string &condition);
};


#endif