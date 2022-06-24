#pragma once

#include "ttl/concepts/index.hpp"
#include <concepts>

namespace ttl
{
    template <int M>
    struct scalar_index
    {
        using scalar_index_concept_tag = void;

        int _data[M];

        constexpr scalar_index() = default;

        constexpr explicit scalar_index(concepts::index_or_integral auto... is)
                : _data { _to_int(is)... }
        {
        }

        template <int B>
        constexpr explicit scalar_index(scalar_index<B> const& b) requires (B < M)
        {
            int i = 0;
            for (; i < B; ++i) _data[i] = b[i];
            for (; i < M; ++i) _data[i] = 0;
        }

        static constexpr auto size() -> int {
            return M;
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
