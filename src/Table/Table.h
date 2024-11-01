#ifndef DUMMYDB_TABLE_H
#define DUMMYDB_TABLE_H


#include <string>
#include <vector>
#include <memory>
#include "../Record/Record.h"

class Table
{
private:
    std::string name;
    std::vector<std::string> columns;
    std::vector<std::shared_ptr<Record>> records;

public:
    Table(const std::string &tableName, const std::vector<std::string> &columnNames) : name(tableName),
                                                                                       columns(columnNames)
    {}

    void insert(const std::vector<std::string> &values);

    void display() const;

    const std::string &getName();

    std::string serialize() const;

    static std::shared_ptr<Table> deserialize(const std::string &tableName, const std::string &data);
};


#endif //DUMMYDB_TABLE_H
