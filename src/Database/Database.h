#ifndef DUMMYDB_DATABASE_H
#define DUMMYDB_DATABASE_H


#include <map>
#include <string>
#include <memory>
#include "../Table/Table.h"

class Database
{
private:
    std::map<std::string, std::shared_ptr<Table>> tables;
public:

    void createTable(const std::string &tableName, const std::vector<std::string> &columns);

    std::shared_ptr<Table> getTable(const std::string &tableName);

    void showTables() const;
};


#endif //DUMMYDB_DATABASE_H
