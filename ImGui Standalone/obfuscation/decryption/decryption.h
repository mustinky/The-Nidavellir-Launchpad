
#pragma once

#include "../../pch.h"

#ifndef DECRYPTION_H
#define DECRYPTION_H

namespace decryption
{
    class DecryptionManager
    {
    private:

        std::string string_encryption(const std::string& input, const std::string& key);

    public:

        std::string string_decryption(const std::string& encryptedText, const std::string& key);

        void secure_erase_memory(char* ptr, size_t size);
        void clear_string(std::string& str);
    };
}

#endif
