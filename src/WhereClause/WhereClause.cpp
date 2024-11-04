#include "WhereClause.h"

WhereClause::WhereClause(const std::string &col, Operator oper, const std::string &val)
        : column(col), op(oper), value(val)
{}

bool WhereClause::evaluate(const std::vector<std::string> &columnNames, const std::vector<std::string> &values)
{
    size_t colIndex = std::find(columnNames.begin(), columnNames.end(), column) - columnNames.begin();
    if (colIndex >= values.size()) return false;

    const std::string &fieldValue = values[colIndex];

    switch (op)
    {
        case Operator::EQUALS:
            return fieldValue == value;
        case Operator::GREATER_THAN:
            return std::stod(fieldValue) > std::stod(value);
        case Operator::LESS_THAN:
            return std::stod(fieldValue) < std::stod(value);
        case Operator::LIKE:
            return fieldValue.find(value) != std::string::npos;
        default:
            return false;
    }
}