#include "FileHandler.h"

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

bool FileHandler::appendEncrypted(const std::string &data)
{
    std::string currentContent = readEncrypted();
    return writeEncrypted(currentContent + data);
}
