
#pragma once

#include "../pch.h"

namespace xorstr_impl {

#ifdef _MSC_VER
#define XORSTR_FORCEINLINE __forceinline
#else
#define XORSTR_FORCEINLINE inline
#endif

    // Compile-time random seed derived from __TIME__ and __DATE__
    constexpr auto compile_time_seed = []() constexpr {
        constexpr auto date = __DATE__;
        constexpr auto time = __TIME__;
        return static_cast<int>(time[7] - '0') +
            static_cast<int>(time[6] - '0') * 10 +
            static_cast<int>(time[4] - '0') * 60 +
            static_cast<int>(time[3] - '0') * 600 +
            static_cast<int>(time[1] - '0') * 3600 +
            static_cast<int>(time[0] - '0') * 36000 +
            static_cast<int>(date[4] - '0') * 86400; // Add date-based variation
        }();

    // Random number generator (compile-time)
    template <int N>
    struct random_generator {
        static constexpr unsigned a = 48271; // Improved constant
        static constexpr unsigned m = 0x7FFFFFFF;
        static constexpr unsigned s = random_generator<N - 1>::value;
        static constexpr unsigned value = (a * s) % m;
    };

    template <>
    struct random_generator<0> {
        static constexpr unsigned value = compile_time_seed;
    };

    template <int N, int M>
    struct random_int {
        static constexpr unsigned value = random_generator<N + 1>::value % M;
    };

    template <int N>
    struct random_char {
        static constexpr char value = static_cast<char>(1 + random_int<N, 0x7F - 1>::value);
    };

    // Encrypted string container
    template <size_t N, int K>
    class encrypted_string {
    private:
        const char key_;
        std::array<char, N + 1> encrypted_;

        constexpr char encrypt_char(char c) const noexcept {
            return c ^ key_;
        }

        char decrypt_char(char c) const noexcept {
            return c ^ key_;
        }

    public:
        // Compile-time encryption
        template <size_t... Is>
        constexpr XORSTR_FORCEINLINE encrypted_string(const char* str, std::index_sequence<Is...>)
            : key_(random_char<K>::value), encrypted_{ { encrypt_char(str[Is])... } } {
            encrypted_[N] = '\0';
        }

        // Runtime decryption
        XORSTR_FORCEINLINE const char* decrypt() const noexcept {
            thread_local std::array<char, N + 1> decrypted;
            for (size_t i = 0; i < N; ++i) {
                decrypted[i] = decrypt_char(encrypted_[i]);
            }
            decrypted[N] = '\0';
            return decrypted.data();
        }

        // Access key for debugging (protected by obfuscation)
        constexpr char get_key() const noexcept {
            return key_;
        }
    };

#undef XORSTR_FORCEINLINE

} // namespace xorstr_impl

// XOR macro for obfuscation
#define xorstr(s) (xorstr_impl::encrypted_string<sizeof(s) - 1, __COUNTER__>(s, std::make_index_sequence<sizeof(s) - 1>()).decrypt())

