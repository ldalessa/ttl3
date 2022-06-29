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

    namespace _detail
    {
        template <class T>
        concept _has_static_member_function_size = requires {
            { std::remove_cvref_t<T>::size() } -> std::convertible_to<int>;
        };

        template <class T>
        concept _has_tuple_size = requires {
            { std::tuple_size<std::remove_cvref_t<T>>::value } -> std::convertible_to<int>;
        };
    }

    template <class T>
    concept has_static_size = _detail::_has_static_member_function_size<T>
        or _detail::_has_tuple_size<T>;

    template <has_static_size T>
    constexpr int size = [] {
        if constexpr (_detail::_has_static_member_function_size<T>) {
            return std::remove_cvref_t<T>::size();
        }
        else {
            std::tuple_size<std::remove_cvref_t<T>>::value;
        }
    }();

    template <class T>
    concept is_array = std::ranges::contiguous_range<std::remove_cvref_t<T>>
        and std::ranges::sized_range<std::remove_cvref_t<T>>
        and requires (T t, int i) {
            { t[i] };
        };

    template <class T, class U>
    concept is_array_of = is_array<T>
        and std::same_as<std::ranges::range_value_t<std::remove_cvref_t<T>>, U>
        and requires (T t, int i) {
            { t[i] } -> std::convertible_to<U>;
        };
}
