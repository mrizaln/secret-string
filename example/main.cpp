#include "secret_string.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <bit>
#include <string_view>
#include <ranges>

struct Obfuscator
{
    static constexpr secret_string::SecretString s_key{ "O76U4CR6u4cov8l7ivto6RVuyo68eiUB6q23r287I65v" };

    static constexpr std::size_t s_seed = []() consteval {
        std::size_t seed = 0;
        for (auto c : __TIME__) {
            seed <<= 8;
            seed  |= (std::size_t)c;
        }
        return seed;
    }() % s_key.size();

    constexpr char encrypt(char c) const { return c ^ s_key.view()[s_seed]; }
    char           decrypt(char c) const { return c ^ s_key.view()[s_seed]; }
};

static_assert(secret_string::ObfuscatorConcept<Obfuscator>);

template <std::size_t N>
using Secret = secret_string::SecretString<N, Obfuscator>;

template <secret_string::FixedString Str>
consteval auto operator""_secret()
{
    return Secret<Str.size()>{ Str };
}

int main()
{
    namespace sv = std::views;

    constexpr std::string_view regular{ "A regular string literal" };    // show up in the built binary as is
    fmt::println("regular      : {:?}", regular);

    constexpr auto secret = "A secret string literal"_secret;    // show up in the built binary but obfuscated
    fmt::println("secret (view): {:?}", secret.view());
    fmt::println("secret (read): {:?}", secret.read());

    const auto bytesView = secret.view() | sv::transform(std::bit_cast<std::byte, char>);
    fmt::println("bytes view   : {:02x}", fmt::join(bytesView, ""));
    fmt::println("               {:02x} (reversed)", fmt::join(bytesView | sv::reverse, ""));
}
