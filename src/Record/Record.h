#ifndef DUMMYDB_RECORD_H
#define DUMMYDB_RECORD_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>

class Record
{
private:
    std::vector<std::string> values;

public:
    Record(const std::vector<std::string> &data) : values(data)
    {}

    const std::string &getValue(size_t index) const;

    void setValue(size_t index, const std::string &value);


    size_t size() const;
};


#endif //DUMMYDB_RECORD_H
