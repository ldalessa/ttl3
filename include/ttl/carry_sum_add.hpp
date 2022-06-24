#pragma once

#include "ttl/concepts/scalar_index.hpp"
#include "ttl/concepts/shape.hpp"
#include <concepts>

namespace ttl
{
    template <int N, concepts::scalar_index Index>
    inline constexpr auto carry_sum_add(Index& i, auto&& extents) -> bool
        requires requires (int i) { {extents(i)} -> std::convertible_to<int>; }
    {
        constexpr int M = Index::size();
        for (int n = N; n < M; ++n) {
            if (++i[n] < extents(n)) {
                return true;
            }
            i[n] = 0;
        }
        return false;
    }
}
