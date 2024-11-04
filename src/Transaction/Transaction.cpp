#include "Transaction.h"

Transaction::Transaction(FileHandler &fh) : file_handler(fh)
{
    backup_filename = file_handler.filename + ".bak";
    file_handler.createBackup(backup_filename);
}

void Transaction::addOperation(const std::string &operation)
{
    operations.push_back(operation);
}

bool Transaction::commit()
{
    try
    {
        std::string content = file_handler.readEncrypted();
        for (const auto &op: operations)
        {
            content += op;
        }

        if (file_handler.writeEncrypted(content))
        {
            is_committed = true;
            std::remove(backup_filename.c_str());
            return true;
        }
        return false;
    } catch (...)
    {
        rollback();
        file_handler.restoreFromBackup(backup_filename);
        return false;
    }
}

void Transaction::rollback()
{
    operations.clear();
    is_committed = false;
}