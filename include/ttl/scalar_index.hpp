#pragma once

#include <concepts>

namespace ttl
{
    template <int M>
    struct scalar_index
    {
        int _data[M];

        constexpr auto operator[](int i) const -> int {
            return _data[i];
        }

        constexpr auto operator[](int i) -> int& {
            return _data[i];
        }
    };

    scalar_index(std::convertible_to<int> auto... is) -> scalar_index<sizeof...(is)>;
}
