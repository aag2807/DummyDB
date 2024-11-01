#include "Database.h"

void Database::createTable(const std::string &tableName, const std::vector<std::string> &columns)
{
    if (tables.find(tableName) == tables.end())
    {
        tables[tableName] = std::make_shared<Table>(tableName, columns);
        std::cout << "Table '" << tableName << "' created successfully.\n";
    } else
    {
        std::cout << "Error: Table '" << tableName << "' already exists.\n";
    }
}

std::shared_ptr<Table> Database::getTable(const std::string &tableName)
{
    auto it = tables.find(tableName);
    if (it != tables.end())
    {
        return it->second;
    }
    return nullptr;
}

void Database::showTables() const
{
    std::cout << "Tables in database:\n";
    for (const auto &table: tables)
    {
        std::cout << "- " << table.first << "\n";
    }
}
