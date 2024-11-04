#include "Index.h"

Index::Index(const std::string &column) : column_name(column)
{}

void Index::addEntry(const std::string &value, size_t position)
{
    value_positions[value].push_back(position);
}

std::vector<size_t> Index::findPositions(const std::string &value)
{
    auto it = value_positions.find(value);
    if (it != value_positions.end())
    {
        return it->second;
    }

    return std::vector<size_t>();
}

void Index::clear()
{
    value_positions.clear();
}

Index::Index()
{
}
