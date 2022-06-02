#pragma once

#include "ttl/utils/FWD.hpp"
#include <concepts>

namespace ttl
{
    namespace cpos
    {
        template <int N>
        struct dim
        {
            constexpr auto operator()(auto&& obj) const -> int
                requires requires { {FWD(obj).template dim<N>()} -> std::same_as<int>; }
            {
                return FWD(obj).template dim<N>();
            }

            consteval dim(int) {}
        };
    }

    template <int N>
    inline constexpr cpos::dim<N> dim{0};
}
