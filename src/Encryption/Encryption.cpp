#include "Encryption.h"

std::string Encryption::xorEncrypt(const std::string& data, const std::string& key) {
    std::string result = data;
    for (size_t i = 0; i < data.length(); i++) {
        result[i] = data[i] ^ key[i % key.length()];
    }
    return result;
}

std::string Encryption::encrypt(const std::string &data)
{
    return xorEncrypt(data, ENCRYPTION_KEY);
}

std::string Encryption::decrypt(const std::string &data)
{
    return xorEncrypt(data, ENCRYPTION_KEY);
}
