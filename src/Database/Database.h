#ifndef DUMMYDB_DATABASE_H
#define DUMMYDB_DATABASE_H

#include <map>
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "../Table/Table.h"
#include "../Encryption/Encryption.h"
#include "../FileHandler/FileHandler.h"
#include "../Index/Index.h"
#include "../Transaction/Transaction.h"
#include "../constants.h"
#include "../WhereClause/WhereClause.h"

class Database {
public:
    explicit Database(const std::string& filename);

    // Table operations
    bool createTable(const std::string& tableName, const std::vector<std::string>& columns);
    bool tableExists(const std::string& tableName);
    void showTables();

    // Index operations
    bool createIndex(const std::string& tableName, const std::string& columnName);

    // CRUD operations
    bool insertRecord(const std::string& tableName, const std::vector<std::string>& values);
    void selectAll(const std::string& tableName, WhereClause* where = nullptr);
    bool updateRecords(const std::string& tableName,
                       const std::string& columnName,
                       const std::string& newValue,
                       WhereClause* where = nullptr);
    bool deleteRecords(const std::string& tableName, WhereClause* where = nullptr);

    bool dropTable(const std::string& tableName);

    void executeSQL(const std::string& query);
private:
    FileHandler file_handler;
    std::map<std::string, Index> indexes;
    std::vector<std::string> current_columns;
    std::string current_table;

    // Helper methods
    std::vector<std::string> splitString(const std::string& str, char delimiter);
    void rebuildIndex(const std::string& tableName, const std::string& columnName);
    std::vector<std::string> getTableColumns(const std::string& tableName);
};

#endif