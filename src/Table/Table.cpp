#include "Table.h"

const std::string &Table::getName()
{
    return this->name;
}

void Table::display() const
{
    for (const auto &col: columns)
    {
        std::cout << col << "\t";
    }
    std::cout << "\n";

    // Display records
    for (const auto &record: records)
    {
        for (size_t i = 0; i < record->size(); ++i)
        {
            std::cout << record->getValue(i) << "\t";
        }
        std::cout << "\n";
    }
}

void Table::insert(const std::vector<std::string> &values)
{
    if (values.size() == columns.size())
    {
        records.push_back(std::make_shared<Record>(values));
        std::cout << "Record inserted successfully" << std::endl;
    }
}

std::string Table::serialize() const
{
    std::stringstream ss;

    // Serialize columns
    for (size_t i = 0; i < columns.size(); ++i)
    {
        if (i > 0) ss << ",";
        ss << columns[i];
    }
    ss << "\n";

    // Serialize records
    for (const auto &record: records)
    {
        ss << record->serialize() << "\n";
    }
    return ss.str();
}

std::shared_ptr<Table> Table::deserialize(const std::string &tableName, const std::string &data)
{
    std::stringstream ss(data);
    std::string line;

    // First line contains columns
    std::getline(ss, line);
    std::vector<std::string> columns;
    std::stringstream colStream(line);
    std::string col;
    while (std::getline(colStream, col, ','))
    {
        columns.push_back(col);
    }

    auto table = std::make_shared<Table>(tableName, columns);

    // Remaining lines are records
    while (std::getline(ss, line))
    {
        if (!line.empty())
        {
            Record record = Record::deserialize(line);
            std::vector<std::string> values;
            for (size_t i = 0; i < record.size(); ++i)
            {
                values.push_back(record.getValue(i));
            }
            table->insert(values);
        }
    }

    return table;
}