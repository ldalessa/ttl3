#pragma once

#include <cassert>
#include <concepts>
#include <ranges>

#ifndef FWD
#define FWD(x) static_cast<decltype(x)&&>(x)
#endif

namespace ttl::tests
{
    template <class T, auto _shape>
    struct md_array
    {
        static constexpr int _order = _shape.order();
        static constexpr int _size = _shape.size();

        T _data[_size];

        static constexpr auto order() -> int {
            return _order;
        }

        static constexpr auto size() -> int {
            return _size;
        }

        constexpr auto begin() const -> decltype(auto) {
            return std::ranges::begin(_data);
        }

        constexpr auto end() -> decltype(auto) {
            return std::ranges::end(_data);
        }

        constexpr auto operator[](std::integral auto i) const -> auto& {
            assert(0 <= i and i < _size);
            return _data[i];
        }

        constexpr auto operator[](std::integral auto i) -> auto& {
            assert(0 <= i and i < _size);
            return _data[i];
        }

        constexpr auto operator()(std::ranges::random_access_range auto&& index) const -> auto& {
            return _data[_shape.linearize(FWD(index))];
        }

        constexpr auto operator()(std::ranges::random_access_range auto&& index) -> auto& {
            return _data[_shape.linearize(FWD(index))];
        }

        constexpr auto operator()(std::convertible_to<int> auto... is) const -> auto& {
            static_assert(sizeof...(is) == _order);
            return _data[_shape(is...)];
        }

        constexpr auto operator()(std::convertible_to<int> auto... is) -> auto& {
            static_assert(sizeof...(is) == _order);
            return _data[_shape(is...)];
        }
    };
}
