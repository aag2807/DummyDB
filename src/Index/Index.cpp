#include "Index.h"

Index::Index(const std::string &column) : column_name(column)
{}

void Index::addEntry(const std::string &value, size_t position)
{
    value_positions[value].push_back(position);
}

std::vector<size_t> Index::findPositions(const std::string &value)
{
    return value_positions[value];
}

void Index::clear()
{
    value_positions.clear();
}