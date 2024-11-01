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