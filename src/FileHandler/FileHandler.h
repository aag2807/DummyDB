#ifndef DUMMYDB_FILEHANDLER_H
#define DUMMYDB_FILEHANDLER_H

#include <string>
#include "../Encryption/Encryption.h"

class FileHandler
{
private:
    std::string filename;
public:
    explicit FileHandler(const std::string &fname) : filename(fname)
    {}

    bool writeEncrypted(const std::string &data);

    std::string readEncrypted();

    bool appendEncrypted(const std::string &data);
}


#endif //DUMMYDB_FILEHANDLER_H
