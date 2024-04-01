#pragma once

#include <algorithm>
#include <concepts>
#include <string>
#include <string_view>
#include <ranges>
#include <type_traits>

namespace secret_string
{
    template <std::size_t N>
    struct FixedString
    {
        char m_data[N]{};

        constexpr FixedString() = default;
        constexpr FixedString(const char (&str)[N]) { std::ranges::copy_n(str, N, m_data); }
        static constexpr std::size_t size() { return N; }
    };

    // clang-format off
    template <typename T>
    concept ObfuscatorConcept = requires(const T t, char c) {
        // can be defaulted
        requires std::default_initializable<T>;

        // required member functions
        { t.encrypt(c) } -> std::same_as<char>;
        { t.decrypt(c) } -> std::same_as<char>;

        // constexpr requirement for encrypt()
        { std::bool_constant<(T{}.encrypt(char{}), true)>() } -> std::same_as<std::true_type>;
    };
    // clang-format on

    struct DefaultObfuscator
    {
        constexpr char encrypt(char c) const { return ~c; }
        constexpr char decrypt(char c) const { return ~c; }
    };

    template <std::size_t N, ObfuscatorConcept Obf = DefaultObfuscator>
    class SecretString : private FixedString<N>
    {
    public:
        using Base = FixedString<N>;

        consteval SecretString(const Base& str)
        {
            std::ranges::transform(str.m_data, Base::m_data, [this](char c) { return m_obfuscator.encrypt(c); });
        }

        static constexpr std::size_t size() { return Base::size(); }

        constexpr std::string_view view() const { return { Base::m_data, Base::size() }; }

        std::string read() const
        {
            auto decrypted = Base::m_data | std::views::transform([this](char c) { return m_obfuscator.decrypt(c); });
            return { decrypted.begin(), decrypted.end() };
        }

    private:
        // no unique address if Obf is an empty class (like a stateless lambda)
        [[no_unique_address]] Obf m_obfuscator;
    };

    template <std::size_t N>
    SecretString(const char (&)[N]) -> SecretString<N>;

    template <FixedString Str>
    consteval auto operator""_secret()
    {
        return SecretString{ Str };
    }
}
