#pragma once

#include <array>
#include <cstdio>
#include <type_traits>
#include <experimental/source_location> // clang

#define TTL_CHECK(expr, ...) ttl::tests::check((expr), #expr, ##__VA_ARGS__)

namespace ttl::tests
{
    static constexpr auto check(
            bool condition,
            const char *expr,
            std::experimental::source_location&& src = std::experimental::source_location::current()) -> bool
    {
        if (condition) {
            return true;
        }

        if (!std::is_constant_evaluated()) {
            std::printf("%s:%d failed unit test %s\n", src.file_name(), src.line(), expr);
        }

        throw src;
    }

    template <class...> struct type_args{};

    template <class... Ts>
    constexpr type_args<Ts...> types{};

    constexpr void unused(...) {}

    template <auto>
    [[gnu::deprecated]]
    void print() {}

    [[gnu::deprecated]]
    void print_t(auto&& u) {
        unused(u);
    }

    constexpr auto to_int_array(std::convertible_to<int> auto... vs)
        -> std::array<int, sizeof...(vs)>
    {
        return std::array<int, sizeof...(vs)> { vs... };
    }
}
