#pragma once

#include "ttl/concepts/index.hpp"
#include <concepts>

namespace ttl
{
    template <int M>
    struct scalar_index
    {
        int _data[M];

        constexpr scalar_index() = default;

        constexpr explicit scalar_index(concepts::index_or_integral auto... is)
                : _data { _to_int(is)... }
        {
        }

        constexpr auto operator[](int i) const -> int {
            return _data[i];
        }

        constexpr auto operator[](int i) -> int& {
            return _data[i];
        }

        static constexpr auto _to_int(concepts::index auto) -> int {
            return -1;
        }

        static constexpr auto _to_int(std::convertible_to<int> auto i) -> int {
            return i;
        }
    };

    scalar_index(concepts::index_or_integral auto... is) -> scalar_index<sizeof...(is)>;
}
