#pragma once

#include "ttl/Bindable.hpp"
#include "ttl/concepts/expression.hpp"
#include "ttl/expressions/Sum.hpp"
#include "ttl/utils/FWD.hpp"

namespace ttl::expressions
{
    struct _minus_fn
    {
        constexpr auto operator()(auto&& a, auto&& b) const -> decltype(auto)
        {
            return FWD(a) - FWD(b);
        }

        consteval _minus_fn(int) {}
    };

    inline constexpr _minus_fn minus{0};

    template <concepts::expression A, concepts::expression B>
    struct Subtraction : Sum<A, B, minus>, Bindable<Subtraction<A, B>>
    {
        using Sum<A, B, minus>::Sum;
    };

    template <concepts::expression A, concepts::expression B>
    Subtraction(A, B) -> Subtraction<A, B>;
}
