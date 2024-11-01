#include "Record.h"

const std::string &Record::getValue(size_t index) const
{
    return values[index];
}

void Record::setValue(size_t index, const std::string &value)
{
    if (index < values.size())
    {
        values[index] = value;
    }
}

size_t Record::size() const
{
    return values.size();
}

std::string Record::serialize() const
{
    std::stringstream ss;
    for (size_t i = 0; i < values.size(); ++i)
    {
        if (i > 0) ss << ",";
        ss << values[i];
    }
    return ss.str();
}

Record Record::deserialize(const std::string &data)
{
    std::vector<std::string> values;
    std::stringstream ss(data);
    std::string item;
    while (std::getline(ss, item, ','))
    {
        values.push_back(item);
    }
    return Record(values);
}
