#include "FileHandler.h"

FileHandler::FileHandler(const std::string &fname) : filename(fname)
{}

bool FileHandler::writeEncrypted(const std::string &data)
{
    try
    {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) return false;
        std::string encrypted = Encryption::encrypt(data);
        file.write(encrypted.c_str(), encrypted.size());
        return true;
    } catch (...)
    {
        return false;
    }
}

std::string FileHandler::readEncrypted()
{
    try
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) return "";
        std::stringstream ss;
        ss << file.rdbuf();
        return Encryption::decrypt(ss.str());
    } catch (...)
    {
        return "";
    }
}

bool FileHandler::createBackup(const std::string &backup_name)
{
    try
    {
        std::string content = readEncrypted();
        std::ofstream backup(backup_name, std::ios::binary);
        if (!backup.is_open()) return false;
        backup.write(content.c_str(), content.size());
        return true;
    } catch (...)
    {
        return false;
    }
}

bool FileHandler::restoreFromBackup(const std::string &backup_name)
{
    try
    {
        std::ifstream backup(backup_name, std::ios::binary);
        if (!backup.is_open()) return false;
        std::stringstream ss;
        ss << backup.rdbuf();
        return writeEncrypted(ss.str());
    } catch (...)
    {
        return false;
    }
}