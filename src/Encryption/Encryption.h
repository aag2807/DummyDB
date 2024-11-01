#ifndef DUMMYDB_ENCRYPTION_H
#define DUMMYDB_ENCRYPTION_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <fstream>
#include <sstream>

class Encryption
{
private:
    static std::string xorEncrypt(const std::string &data, const std::string &key);

public:
    static std::string encrypt(const std::string &data);

    static std::string decrypt(const std::string &data);
};

#endif //DUMMYDB_ENCRYPTION_H
