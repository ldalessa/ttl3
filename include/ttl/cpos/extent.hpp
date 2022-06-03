#pragma once

#include "ttl/utils/FWD.hpp"
#include <concepts>

namespace ttl
{
    namespace cpos
    {
        template <int N>
        struct extent
        {
            constexpr auto operator()(auto&& obj) const -> int
                requires requires{{ FWD(obj).template extent<0>() } -> std::same_as<int>;}
            {
                return FWD(obj).template extent<N>();
            }

            consteval extent(int) {}
        };
    }

    template <int N>
    inline constexpr cpos::extent<N> extent{0};
}
