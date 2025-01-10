
#include "decryption.h"

namespace decryption
{
    std::string DecryptionManager::string_encryption(const std::string& input, const std::string& key)
    {
        std::string output = input;

        size_t key_index = 0;

        for (size_t i = 0; i < input.size(); ++i)
        {
            output[i] ^= key[key_index++];

            if (key_index >= key.size())
            {
                key_index = 0;
            }
        }
        return output;
    }

    std::string DecryptionManager::string_decryption(const std::string& encryptedText, const std::string& key)
    {
        return string_encryption(encryptedText, key);
    }  

    void DecryptionManager::secure_erase_memory(char* ptr, size_t size)
    {
      //  VMProtectBeginUltra("nidavellir::decryption::secure_erase_memory");

        volatile char* vptr = ptr;

        for (size_t i = 0; i < size; ++i)
        {
            vptr[i] = 0;
        }

       // VMProtectEnd();
    }

    void DecryptionManager::clear_string(std::string& str)
    {
        for (char& c : str)
        {
            c ^= 898932389328938892;
        }

        str = xorstr("");
    }
}