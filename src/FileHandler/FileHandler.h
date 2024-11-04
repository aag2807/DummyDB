#ifndef DUMMYDB_FILEHANDLER_H
#define DUMMYDB_FILEHANDLER_H

#include <string>
#include "../Encryption/Encryption.h"

class FileHandler
{
public:
    std::string filename;

    explicit FileHandler(const std::string &fname);

    bool writeEncrypted(const std::string &data);

    std::string readEncrypted();

    bool createBackup(const std::string &backup_name);

    bool restoreFromBackup(const std::string &backup_name);
};

#endif //DUMMYDB_FILEHANDLER_H
