
#ifndef DUMMYDB_TRANSACTION_H
#define DUMMYDB_TRANSACTION_H

#include <vector>
#include <string>
#include "../FileHandler/FileHandler.h"

class Transaction
{
public:
    explicit Transaction(FileHandler &file_handler);

    void addOperation(const std::string &operation);

    bool commit();

    void rollback();

private:
    std::vector<std::string> operations;
    bool is_committed = false;
    std::string backup_filename;
    FileHandler &file_handler;
};

#endif //DUMMYDB_TRANSACTION_H
