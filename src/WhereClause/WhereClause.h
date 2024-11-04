#ifndef DUMMYDB_WHERECLAUSE_H
#define DUMMYDB_WHERECLAUSE_H

#include <string>
#include <vector>
#include <algorithm>

class WhereClause
{
public:
    enum class Operator
    {
        EQUALS,
        GREATER_THAN,
        LESS_THAN,
        LIKE
    };

    WhereClause(const std::string &col, Operator oper, const std::string &val);

    bool evaluate(const std::vector<std::string> &columnNames, const std::vector<std::string> &values);

private:
    std::string column;
    Operator op;
    std::string value;
};


#endif //DUMMYDB_WHERECLAUSE_H
