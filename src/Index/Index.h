#ifndef DUMMYDB_INDEX_H
#define DUMMYDB_INDEX_H

#include <string>
#include <map>
#include <vector>

class Index
{
public:
    explicit Index(const std::string &column);

    void addEntry(const std::string &value, size_t position);

    std::vector<size_t> findPositions(const std::string &value);

    void clear();

private:
    std::string column_name;
    std::map<std::string, std::vector<size_t>> value_positions;
};


#endif //DUMMYDB_INDEX_H
