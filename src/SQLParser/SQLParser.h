#ifndef DUMMYDB_SQLPARSER_H
#define DUMMYDB_SQLPARSER_H

#include <string>
#include <vector>
#include <memory>
#include "../WhereClause/WhereClause.h"

class SQLParser
{
public:
    enum class QueryType
    {
        SELECT,
        INSERT,
        UPDATE,
        DELETE,
        DROP,
        CREATE,
        UNKNOWN
    };

    struct SQLQuery
    {
        QueryType type = QueryType::UNKNOWN;
        std::string table;
        std::vector<std::string> columns;
        std::vector<std::string> values;
        std::string updateColumn;
        std::string updateValue;
        std::unique_ptr<WhereClause> whereClause;
    };

    static SQLQuery parse(const std::string &query);

private:
    static std::vector<std::string> tokenize(const std::string &query);

    static std::string toLowerCase(std::string str);

    static WhereClause::Operator parseOperator(const std::string &op);

    static std::string trimQuotes(std::string str);

    static std::string extractFromQuotes(const std::string &str);
};

#endif //DUMMYDB_SQLPARSER_H
