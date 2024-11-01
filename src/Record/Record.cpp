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
