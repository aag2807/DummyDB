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

bool Database::saveToFile(const std::string &filename)
{
    try
    {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open())
        {
            std::cout << "Error: Could not open file for writing.\n";
            return false;
        }

        // Serialize database structure
        std::stringstream ss;
        ss << tables.size() << "\n";

        for (const auto &table: tables)
        {
            ss << table.first << "\n";  // Table name
            ss << table.second->serialize();  // Table content
            ss << "---END_TABLE---\n";  // Table separator
        }

        // Encrypt and write to file
        std::string encrypted = Encryption::encrypt(ss.str());
        file.write(encrypted.c_str(), encrypted.size());
        file.close();

        std::cout << "Database saved successfully to " << filename << "\n";
        return true;
    } catch (const std::exception &e)
    {
        std::cout << "Error saving database: " << e.what() << "\n";
        return false;
    }
}

bool Database::loadFromFile(const std::string &filename)
{
    try
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open())
        {
            std::cout << "Error: Could not open file for reading.\n";
            return false;
        }

        // Read encrypted content
        std::stringstream ss;
        ss << file.rdbuf();
        std::string encrypted = ss.str();
        file.close();

        // Decrypt content
        std::string decrypted = Encryption::decrypt(encrypted);
        std::stringstream contentStream(decrypted);

        // Clear existing tables
        tables.clear();

        // Read number of tables
        std::string line;
        std::getline(contentStream, line);
        int numTables = std::stoi(line);

        // Read each table
        for (int i = 0; i < numTables; ++i)
        {
            std::string tableName;
            std::getline(contentStream, tableName);

            std::stringstream tableContent;
            while (std::getline(contentStream, line))
            {
                if (line == "---END_TABLE---") break;
                tableContent << line << "\n";
            }

            tables[tableName] = Table::deserialize(tableName, tableContent.str());
        }

        std::cout << "Database loaded successfully from " << filename << "\n";
        return true;
    } catch (const std::exception &e)
    {
        std::cout << "Error loading database: " << e.what() << "\n";
        return false;
    }
}
