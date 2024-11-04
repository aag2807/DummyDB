#include "Database.h"

const std::string RECORD_SEPARATOR = "\n";
const std::string TABLE_SEPARATOR = "---END_TABLE---\n";

bool Database::tableExists(const std::string &tableName)
{
    std::string content = file_handler.readEncrypted();
    return content.find("TABLE:" + tableName + "\n") != std::string::npos;
}

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

bool Database::createTable(const std::string &tableName, const std::vector<std::string> &columns)
{
    if (tableExists(tableName))
    {
        std::cout << "Error: Table already exists.\n";
        return false;
    }

    std::stringstream ss;
    ss << "TABLE:" << tableName << "\n";
    ss << "COLUMNS:";
    for (size_t i = 0; i < columns.size(); ++i)
    {
        if (i > 0) ss << ",";
        ss << columns[i];
    }
    ss << "\n" << TABLE_SEPARATOR;

    if (file_handler.appendEncrypted(ss.str()))
    {
        current_table = tableName;
        current_columns = columns;
        std::cout << "Table created successfully.\n";
        return true;
    }

    std::cout << "Error creating table.\n";
    return false;
}

bool Database::insertRecord(const std::string &tableName, const std::vector<std::string> &values)
{
    if (!tableExists(tableName))
    {
        std::cout << "Error: Table does not exist.\n";
        return false;
    }

    std::stringstream ss;
    ss << "RECORD:" << tableName << ":";
    for (size_t i = 0; i < values.size(); ++i)
    {
        if (i > 0) ss << ",";
        ss << values[i];
    }
    ss << RECORD_SEPARATOR;

    if (file_handler.appendEncrypted(ss.str()))
    {
        std::cout << "Record inserted successfully.\n";
        return true;
    }

    std::cout << "Error inserting record.\n";
    return false;
}

void Database::selectAll(const std::string &tableName)
{
    std::string content = file_handler.readEncrypted();
    std::stringstream ss(content);
    std::string line;
    bool foundTable = false;
    std::vector<std::string> columns;

    while (std::getline(ss, line))
    {
        if (line == "TABLE:" + tableName)
        {
            foundTable = true;
            std::getline(ss, line); // Read columns line
            if (line.substr(0, 8) == "COLUMNS:")
            {
                columns = splitString(line.substr(8), ',');

                // Print column headers
                for (const auto &col: columns)
                {
                    std::cout << col << "\t";
                }
                std::cout << "\n";
                break;
            }
        }
    }

    if (!foundTable)
    {
        std::cout << "Table not found.\n";
        return;
    }

    // Read and print records
    ss.str(content);
    while (std::getline(ss, line))
    {
        if (line.substr(0, 7) == "RECORD:" &&
            line.substr(7, tableName.length()) == tableName)
        {

            std::string record_data = line.substr(8 + tableName.length());
            auto values = splitString(record_data, ',');

            for (const auto &value: values)
            {
                std::cout << value << "\t";
            }
            std::cout << "\n";
        }
    }
}

void Database::deleteRecords(const std::string &tableName, const std::string &condition)
{
    // To be implemented: Delete records based on condition
    std::cout << "Delete functionality to be implemented.\n";
}

void Database::updateRecords(const std::string &tableName, const std::string &condition)
{
    // To be implemented: Update records based on condition
    std::cout << "Update functionality to be implemented.\n";
}
