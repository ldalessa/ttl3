#pragma once

#include "ttl/Bindable.hpp"
#include "ttl/concepts/expression.hpp"
#include "ttl/expressions/Sum.hpp"
#include "ttl/utils/FWD.hpp"

namespace ttl::expressions
{
    struct _plus_fn
    {
        constexpr auto operator()(auto&& a, auto&& b) const -> decltype(auto)
        {
            return FWD(a) + FWD(b);
        }

        consteval _plus_fn(int) {}
    };

    inline constexpr _plus_fn plus{0};

    template <concepts::expression A, concepts::expression B>
    struct Addition : Sum<A, B, plus>, Bindable<Addition<A, B>>
    {
        using Sum<A, B, plus>::Sum;
    };

    template <concepts::expression A, concepts::expression B>
    Addition(A, B) -> Addition<A, B>;
}
