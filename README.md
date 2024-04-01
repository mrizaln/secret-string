# secret-string

A simple C++20 string literal obfuscation library.

## Dependencies

- C++20

## Usage

The library can be used as is or with modification (see below).

> `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.14)
project(main)

# for example you clone this repository to ./lib/secret-string
add_subdirectory(./lib/secret-string)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE secret-string)
```

> `main.cpp`

```cpp
#include <secret_string.hpp>

#include <iostream>

using secret_string::operator""_secret;
using secret_string::SecretString;

int main() {
    constexpr auto regular = "This is a regular string literal";

    constexpr auto secret = "This is a secret string literal"_secret;   // returns a secret_string::SecretString
    std::cout << secret.view() << '\n';   // return the obfuscated string as is
    std::cout << secret.read() << '\n';   // de-obfuscate the string

    constexpr SecretString anotherSecret{ "This is another secret string literal" };
}
```

> If you examine the built binary using `strings` you won't find the secret string literals. Possible output:
>
> ```sh
> $ strings main | grep "literal"
> This is a regular string literal
> $
> ```

## Customization

The default obfuscation method is just an invert (`operator~(char)`) operation per character in the string. Replace the defaulted template parameter of `SecretString` to customize the obfuscation method:

```cpp
#include <secret_string.hpp>

struct Obfuscator
{
    constexpr char encrypt(char c) const { return c ^ 1234; }
    constexpr char decrypt(char c) const { return c ^ 1234; }
};

// you can use this concept to make sure the struct is a correct template for SecretString
// (though it will be checked anyway at the "template parameter call site" using constrained template)
static_assert(secret_string::ObfuscatorConcept<Obfuscator>);

template <std::size_t N>
using Secret = secret_string::SecretString<N, Obfuscator>;

// ...
```

See the [example](./example/main.cpp) for a more sophisticated example.

## Limitation

The use of type alias like the above example prevent CTAD from happening thus we need to explicitly specify the string size. A possible workaround is to make a factory function or an-UDL:

```cpp
// ...

template <std::size_t N>
using Secret = secret_string::SecretString<N, Obfuscator>;

template <secret_string::FixedString Str>
consteval auto operator""_secret()
{
    return Secret<Str.size()>{ Str };
}

// ...
```

This makes creating a `SecretString` much easier and less error prone.
