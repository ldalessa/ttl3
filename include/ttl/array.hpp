#pragma once

#include <algorithm>
#include <concepts>
#include <ranges>
#include <type_traits>

namespace ttl
{
    template <class T, int _size>
    struct array
    {
        using value_type = T;

        T _data[_size];

        static constexpr auto size() -> int {
            return _size;
        }

        constexpr auto operator==(array const& b) const -> bool {
            return std::equal(begin(), end(), b.begin());
        }

        constexpr auto operator<=>(array const& b) const {
            return std::lexicographical_compare_three_way(begin(), end(), b.begin(), b.end());
        }

        constexpr auto begin() const {
            return std::ranges::begin(_data);
        }

        constexpr auto begin() {
            return std::ranges::begin(_data);
        }

        constexpr auto end() const {
            return std::ranges::end(_data);
        }

        constexpr auto end() {
            return std::ranges::end(_data);
        }

        constexpr auto rbegin() const {
            return std::ranges::rbegin(_data);
        }

        constexpr auto rbegin() {
            return std::ranges::rbegin(_data);
        }

        constexpr auto rend() const {
            return std::ranges::rend(_data);
        }

        constexpr auto rend() {
            return std::ranges::rend(_data);
        }

        constexpr auto operator[](int i) const -> auto& {
            return _data[i];
        }

        constexpr auto operator[](int i) -> auto& {
            return _data[i];
        }
    };

    template <class T>
    struct array<T, 0>
    {
        using value_type = T;

        static constexpr auto size() -> int {
            return 0;
        }

        constexpr auto operator== (array const&) const -> bool = default;
        constexpr auto operator<=>(array const&) const -> auto = default;

        constexpr auto begin() const -> T const* {
            return nullptr;
        }

        constexpr auto begin() -> T* {
            return nullptr;
        }

        constexpr auto end() const -> T const* {
            return nullptr;
        }

        constexpr auto end() -> T* {
            return nullptr;
        }

        constexpr auto rbegin() const -> T const* {
            return nullptr;
        }

        constexpr auto rbegin() -> T* {
            return nullptr;
        }

        constexpr auto rend() const -> T const* {
            return nullptr;
        }

        constexpr auto rend() -> T* {
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

    template <class T>
    array(T, std::convertible_to<T> auto... ts) -> array<T, sizeof...(ts) + 1>;

    template <class T, int A, int B>
    constexpr auto join(array<T, A> const& a, array<T, B> const& b) -> array<T, A + B>
    {
        array<T, A + B> out;
        int i = 0;
        for (auto&& a : a) out[i++] = a;
        for (auto&& b : b) out[i++] = b;
        if (i != A + B) throw "error joining arrays";
        return out;
    }
}
