#include "secret_string.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <bit>
#include <string_view>
#include <ranges>

namespace sr = std::ranges;
namespace sv = std::views;

consteval std::size_t time_as_usize()
{
    return sr::fold_left(__TIME__, 0uz, [](auto a, auto v) { return a << 8 | static_cast<std::size_t>(v); });
}

struct Obfuscator
{
    static constexpr auto key  = secret_string::SecretString{ "O76U4CR6u4cov8l7ivto6RVuyo68eiUB6q23r287I65v" };
    static constexpr auto seed = time_as_usize() % key.size();

    constexpr char encrypt(char c) const { return c ^ key.view()[seed]; }
    constexpr char decrypt(char c) const { return c ^ key.view()[seed]; }
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
    constexpr auto regular = std::string_view{ "A regular string literal" };    // show up in the built binary as is
    fmt::println("regular      : {:?}", regular);

    constexpr auto secret = "A secret string literal"_secret;    // show up in the built binary but obfuscated
    fmt::println("secret (view): {:?}", secret.view());
    fmt::println("secret (read): {:?}", secret.read());

    const auto bytesView = secret.view() | sv::transform(std::bit_cast<std::byte, char>);
    fmt::println("bytes view   : {:02x}", fmt::join(bytesView, ""));
    fmt::println("               {:02x} (reversed)", fmt::join(bytesView | sv::reverse, ""));
}
