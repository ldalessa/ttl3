#pragma once

#include <cassert>
#include <concepts>
#include <ranges>

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
    };
}
