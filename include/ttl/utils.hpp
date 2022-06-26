#pragma once

#include <array>
#include <concepts>

#ifndef FWD
#define FWD(x) static_cast<decltype(x)&&>(x)
#endif

namespace ttl
{
    template <auto...>
    struct nttp_args{};

    template <auto... args>
    constexpr nttp_args<args...> nttp{};

    [[deprecated]]
    void debug_print_types(auto...) {}

    template <class T>
    concept has_static_member_function_size = requires {
        { std::remove_cvref_t<T>::size() } -> std::convertible_to<int>;
    };

    template <class T>
    concept has_tuple_size = requires {
        { std::tuple_size<std::remove_cvref_t<T>>::value } -> std::convertible_to<int>;
    };

    template <class T>
    concept has_static_size = has_static_member_function_size<T> or has_tuple_size<T>;

    template <has_static_size T>
    constexpr int size = [] {
        if constexpr (has_static_member_function_size<T>) {
            return std::remove_cvref_t<T>::size();
        }
        else {
            std::tuple_size<std::remove_cvref_t<T>>::value;
        }
    }();
}
