#pragma once

#include <ranges>

namespace ttl
{
    template <class T>
    concept has_static_size = requires {
        { std::remove_cvref_t<T>::size() } -> std::convertible_to<int>;
    };

    template <class T>
    concept has_index_operator = (
            std::ranges::contiguous_range<std::remove_cvref_t<T>> and
            requires (T t, int i) {
                { t[i] } -> std::convertible_to<std::ranges::range_value_t<std::remove_cvref_t<T>>>;
            });

    template <class T>
    concept is_array = (
            std::ranges::contiguous_range<std::remove_cvref_t<T>> and
            std::ranges::sized_range<std::remove_cvref_t<T>> and
            has_static_size<T> and
            ((std::remove_cvref_t<T>::size() == 0) or has_index_operator<T>));

    template <class T>
    concept is_int_array = (
            is_array<T> and
            std::same_as<std::ranges::range_value_t<std::remove_cvref_t<T>>, int>);

    template <class T, int _size>
    struct array
    {
        T _data[_size];

        static constexpr auto size() -> int {
            return _size;
        }

        constexpr auto begin() const {
            return std::ranges::begin(_data);
        }

        constexpr auto end() const {
            return std::ranges::end(_data);
        }

        constexpr auto operator[](int i) const -> T const& {
            return _data[i];
        }

        constexpr auto operator[](int i) -> T& {
            return _data[i];
        }
    };

    template <class T>
    struct array<T, 0>
    {
        static constexpr auto size() -> int {
            return 0;
        }

        constexpr auto begin() const -> T const* {
            return nullptr;
        }

        constexpr auto end() const -> T const* {
            return nullptr;
        }

        [[noreturn]]
        auto operator[](int) const -> T const& {
            throw "invalid use of operator[] for 0 sized array";
        }

        [[noreturn]]
        auto operator[](int) -> T& {
            throw "invalid use of operator[] for 0 sized array";
        }
    };

    array() -> array<int, 0>;

    template <class T, std::convertible_to<T>... Ts>
    array(T, Ts...) -> array<T, sizeof...(Ts) + 1>;
}
