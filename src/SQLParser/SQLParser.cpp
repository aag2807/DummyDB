#include "SQLParser.h"

#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

std::string SQLParser::toLowerCase(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

std::vector<std::string> SQLParser::tokenize(const std::string &query)
{
    std::vector<std::string> tokens;
    std::string token;
    bool inQuotes = false;

    for (char c: query)
    {
        if (c == '\'')
        {
            inQuotes = !inQuotes;
            token += c;
        } else if (!inQuotes && (c == ' ' || c == ',' || c == '(' || c == ')'))
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
        } else
        {
            token += c;
        }
    }
    if (!token.empty())
    {
        tokens.push_back(token);
    }
    return tokens;
}

std::string SQLParser::trimQuotes(std::string str)
{
    if (str.size() >= 2 && str.front() == '\'' && str.back() == '\'')
    {
        return str.substr(1, str.size() - 2);
    }
    return str;
}

std::string SQLParser::extractFromQuotes(const std::string &str)
{
    std::regex pattern("'(.*?)'");
    std::smatch match;
    if (std::regex_search(str, match, pattern))
    {
        return match[1].str();
    }
    return str;
}

WhereClause::Operator SQLParser::parseOperator(const std::string &op)
{
    std::string lowerOp = toLowerCase(op);
    if (lowerOp == "=" || lowerOp == "==") return WhereClause::Operator::EQUALS;
    if (lowerOp == ">") return WhereClause::Operator::GREATER_THAN;
    if (lowerOp == "<") return WhereClause::Operator::LESS_THAN;
    if (lowerOp == "like") return WhereClause::Operator::LIKE;
    return WhereClause::Operator::EQUALS; // Default
}

SQLParser::SQLQuery SQLParser::parse(const std::string &query)
{
    SQLQuery result;
    std::vector<std::string> tokens = tokenize(query);

    if (tokens.empty()) return result;

    // Convert first token to lowercase for command identification
    std::string command = toLowerCase(tokens[0]);

    if (command == "select")
    {
        result.type = QueryType::SELECT;

        // Parse columns
        size_t i = 1;
        while (i < tokens.size() && toLowerCase(tokens[i]) != "from")
        {
            if (tokens[i] != "*" && tokens[i] != ",")
            {
                result.columns.push_back(tokens[i]);
            }
            i++;
        }

        // If no columns specified or *, select all columns
        if (result.columns.empty())
        {
            result.columns.push_back("*");
        }

        // Find table name
        i++; // Skip "from"
        if (i < tokens.size())
        {
            result.table = tokens[i++];
        }

        // Parse WHERE clause if exists
        while (i < tokens.size())
        {
            if (toLowerCase(tokens[i]) == "where" && i + 3 < tokens.size())
            {
                std::string column = tokens[i + 1];
                WhereClause::Operator op = parseOperator(tokens[i + 2]);
                std::string value = extractFromQuotes(tokens[i + 3]);
                result.whereClause = std::make_unique<WhereClause>(column, op, value);
                break;
            }
            i++;
        }
    } else if (command == "insert")
    {
        result.type = QueryType::INSERT;

        // Parse "INSERT INTO table VALUES (v1, v2, ...)"
        size_t i = 1;
        if (toLowerCase(tokens[i]) == "into") i++;

        // Get table name
        if (i < tokens.size())
        {
            result.table = tokens[i++];
        }

        // Get values
        while (i < tokens.size())
        {
            if (toLowerCase(tokens[i]) == "values")
            {
                i++;
                while (i < tokens.size())
                {
                    std::string value = tokens[i];
                    if (value != "," && value != "(" && value != ")")
                    {
                        result.values.push_back(trimQuotes(value));
                    }
                    i++;
                }
            }
            i++;
        }
    } else if (command == "update")
    {
        result.type = QueryType::UPDATE;

        // Get table name
        if (tokens.size() > 1)
        {
            result.table = tokens[1];
        }

        // Parse SET clause
        size_t i = 2;
        while (i < tokens.size())
        {
            if (toLowerCase(tokens[i]) == "set" && i + 2 < tokens.size())
            {
                result.updateColumn = tokens[i + 1];
                result.updateValue = trimQuotes(tokens[i + 3]); // Skip the = sign
                i += 4;
                break;
            }
            i++;
        }

        // Parse WHERE clause
        while (i < tokens.size())
        {
            if (toLowerCase(tokens[i]) == "where" && i + 3 < tokens.size())
            {
                std::string column = tokens[i + 1];
                WhereClause::Operator op = parseOperator(tokens[i + 2]);
                std::string value = trimQuotes(tokens[i + 3]);
                result.whereClause = std::make_unique<WhereClause>(column, op, value);
                break;
            }
            i++;
        }
    } else if (command == "delete")
    {
        result.type = QueryType::DELETE;

        // Parse "DELETE FROM table WHERE ..."
        size_t i = 1;
        if (toLowerCase(tokens[i]) == "from") i++;

        // Get table name
        if (i < tokens.size())
        {
            result.table = tokens[i++];
        }

        // Parse WHERE clause
        while (i < tokens.size())
        {
            if (toLowerCase(tokens[i]) == "where" && i + 3 < tokens.size())
            {
                std::string column = tokens[i + 1];
                WhereClause::Operator op = parseOperator(tokens[i + 2]);
                std::string value = trimQuotes(tokens[i + 3]);
                result.whereClause = std::make_unique<WhereClause>(column, op, value);
                break;
            }
            i++;
        }
    } else if (command == "drop")
    {
        result.type = QueryType::DROP;
        if (tokens.size() > 2 && toLowerCase(tokens[1]) == "table")
        {
            result.table = tokens[2];
        }
    }

    return result;
}