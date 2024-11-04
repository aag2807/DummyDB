#include "Database.h"

Database::Database(const std::string &filename) : file_handler(filename)
{}

std::vector<std::string> Database::splitString(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<std::string> Database::getTableColumns(const std::string &tableName)
{
    std::string content = file_handler.readEncrypted();
    std::stringstream ss(content);
    std::string line;

    while (std::getline(ss, line))
    {
        if (line == "TABLE:" + tableName)
        {
            std::getline(ss, line);
            if (line.substr(0, 8) == "COLUMNS:")
            {
                return splitString(line.substr(8), ',');
            }
        }
    }
    return std::vector<std::string>();
}

void Database::rebuildIndex(const std::string &tableName, const std::string &columnName)
{
    std::string indexKey = tableName + "." + columnName;
    if (indexes.find(indexKey) != indexes.end())
    {
        indexes[indexKey].clear();
        std::string content = file_handler.readEncrypted();
        std::stringstream ss(content);
        std::string line;
        size_t position = 0;

        auto columns = getTableColumns(tableName);
        size_t colIndex = std::find(columns.begin(), columns.end(), columnName) - columns.begin();

        while (std::getline(ss, line))
        {
            if (line.substr(0, 7) == "RECORD:" &&
                line.substr(7, tableName.length()) == tableName)
            {
                auto values = splitString(line.substr(8 + tableName.length()), ',');
                if (colIndex < values.size())
                {
                    indexes[indexKey].addEntry(values[colIndex], position);
                }
            }
            position = ss.tellg();
        }
    }
}

bool Database::createIndex(const std::string &tableName, const std::string &columnName)
{
    if (!tableExists(tableName))
    {
        std::cout << "Error: Table does not exist.\n";
        return false;
    }

    std::string indexKey = tableName + "." + columnName;
    if (indexes.find(indexKey) == indexes.end())
    {
        auto columns = getTableColumns(tableName);
        if (std::find(columns.begin(), columns.end(), columnName) != columns.end())
        {
            indexes.emplace(indexKey, Index(columnName));
            rebuildIndex(tableName, columnName);
            std::cout << "Index created on " << tableName << "." << columnName << "\n";
            return true;
        } else
        {
            std::cout << "Error: Column does not exist in table.\n";
            return false;
        }
    }
    std::cout << "Error: Index already exists.\n";
    return false;
}

bool Database::createTable(const std::string &tableName, const std::vector<std::string> &columns)
{
    if (tableExists(tableName))
    {
        std::cout << "Error: Table already exists.\n";
        return false;
    }

    Transaction trans(file_handler);
    std::stringstream ss;
    ss << "TABLE:" << tableName << "\n";
    ss << "COLUMNS:";
    for (size_t i = 0; i < columns.size(); ++i)
    {
        if (i > 0) ss << ",";
        ss << columns[i];
    }
    ss << "\n" << TABLE_SEPARATOR;

    trans.addOperation(ss.str());
    if (trans.commit())
    {
        current_table = tableName;
        current_columns = columns;
        std::cout << "Table created successfully.\n";
        return true;
    }

    std::cout << "Error creating table.\n";
    return false;
}

bool Database::tableExists(const std::string &tableName)
{
    std::string content = file_handler.readEncrypted();
    return content.find("TABLE:" + tableName + "\n") != std::string::npos;
}

void Database::showTables()
{
    std::string content = file_handler.readEncrypted();
    std::stringstream ss(content);
    std::string line;
    std::cout << "Tables in database:\n";
    while (std::getline(ss, line))
    {
        if (line.substr(0, 6) == "TABLE:")
        {
            std::cout << "- " << line.substr(6) << "\n";
        }
    }
}

bool Database::insertRecord(const std::string &tableName, const std::vector<std::string> &values)
{
    if (!tableExists(tableName))
    {
        std::cout << "Error: Table does not exist.\n";
        return false;
    }

    auto columns = getTableColumns(tableName);
    if (columns.size() != values.size())
    {
        std::cout << "Error: Number of values doesn't match number of columns.\n";
        return false;
    }

    Transaction trans(file_handler);
    std::stringstream ss;
    ss << "RECORD:" << tableName << ":";
    for (size_t i = 0; i < values.size(); ++i)
    {
        if (i > 0) ss << ",";
        ss << values[i];
    }
    ss << RECORD_SEPARATOR;

    trans.addOperation(ss.str());
    if (trans.commit())
    {
        // Update indexes if they exist
        for (const auto &index: indexes)
        {
            if (index.first.substr(0, tableName.length()) == tableName)
            {
                rebuildIndex(tableName, index.first.substr(tableName.length() + 1));
            }
        }
        std::cout << "Record inserted successfully.\n";
        return true;
    }

    std::cout << "Error inserting record.\n";
    return false;
}

void Database::selectAll(const std::string &tableName, WhereClause *where)
{
    if (!tableExists(tableName))
    {
        std::cout << "Error: Table does not exist.\n";
        return;
    }

    auto columns = getTableColumns(tableName);
    if (columns.empty())
    {
        std::cout << "Error: Could not read table columns.\n";
        return;
    }

    // Print headers
    for (const auto &col: columns)
    {
        std::cout << col << "\t";
    }
    std::cout << "\n";

    // Read and filter records
    std::string content = file_handler.readEncrypted();
    std::stringstream ss(content);
    std::string line;

    while (std::getline(ss, line))
    {
        if (line.substr(0, 7) == "RECORD:" &&
            line.substr(7, tableName.length()) == tableName)
        {

            auto values = splitString(line.substr(8 + tableName.length()), ',');

            if (!where || where->evaluate(columns, values))
            {
                for (const auto &value: values)
                {
                    std::cout << value << "\t";
                }
                std::cout << "\n";
            }
        }
    }
}

bool Database::updateRecords(const std::string &tableName,
                             const std::string &columnName,
                             const std::string &newValue,
                             WhereClause *where)
{
    if (!tableExists(tableName))
    {
        std::cout << "Error: Table does not exist.\n";
        return false;
    }

    auto columns = getTableColumns(tableName);
    size_t updateColumn = std::find(columns.begin(), columns.end(), columnName) - columns.begin();

    if (updateColumn >= columns.size())
    {
        std::cout << "Error: Column not found.\n";
        return false;
    }

    Transaction trans(file_handler);
    std::string content = file_handler.readEncrypted();
    std::stringstream ss(content);
    std::stringstream newContent;
    std::string line;
    bool recordsUpdated = false;

    while (std::getline(ss, line))
    {
        if (line.substr(0, 7) == "RECORD:" &&
            line.substr(7, tableName.length()) == tableName)
        {

            auto values = splitString(line.substr(8 + tableName.length()), ',');

            if (!where || where->evaluate(columns, values))
            {
                values[updateColumn] = newValue;
                recordsUpdated = true;

                newContent << "RECORD:" << tableName << ":";
                for (size_t i = 0; i < values.size(); ++i)
                {
                    if (i > 0) newContent << ",";
                    newContent << values[i];
                }
                newContent << RECORD_SEPARATOR;
            } else
            {
                newContent << line << RECORD_SEPARATOR;
            }
        } else
        {
            newContent << line << "\n";
        }
    }

    if (!recordsUpdated)
    {
        std::cout << "No records matched the update criteria.\n";
        return false;
    }

    trans.addOperation(newContent.str());
    if (trans.commit())
    {
        // Update indexes if they exist
        for (const auto &index: indexes)
        {
            if (index.first.substr(0, tableName.length()) == tableName)
            {
                rebuildIndex(tableName, index.first.substr(tableName.length() + 1));
            }
        }
        std::cout << "Records updated successfully.\n";
        return true;
    }

    std::cout << "Error updating records.\n";
    return false;
}

bool Database::deleteRecords(const std::string &tableName, WhereClause *where)
{
    if (!tableExists(tableName))
    {
        std::cout << "Error: Table does not exist.\n";
        return false;
    }

    auto columns = getTableColumns(tableName);
    Transaction trans(file_handler);
    std::string content = file_handler.readEncrypted();
    std::stringstream ss(content);
    std::stringstream newContent;
    std::string line;
    bool recordsDeleted = false;

    while (std::getline(ss, line))
    {
        if (line.substr(0, 7) == "RECORD:" &&
            line.substr(7, tableName.length()) == tableName)
        {

            auto values = splitString(line.substr(8 + tableName.length()), ',');

            if (!where || where->evaluate(columns, values))
            {
                recordsDeleted = true;
                continue; // Skip this record (delete it)
            }
            newContent << line << RECORD_SEPARATOR;
        } else
        {
            newContent << line << "\n";
        }
    }

    if (!recordsDeleted)
    {
        std::cout << "No records matched the delete criteria.\n";
        return false;
    }

    trans.addOperation(newContent.str());
    if (trans.commit())
    {
        // Update indexes if they exist
        for (const auto &index: indexes)
        {
            if (index.first.substr(0, tableName.length()) == tableName)
            {
                rebuildIndex(tableName, index.first.substr(tableName.length() + 1));
            }
        }
        std::cout << "Records deleted successfully.\n";
        return true;
    }

    std::cout << "Error deleting records.\n";
    return false;
}
