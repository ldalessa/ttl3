#pragma once

#include "ttl/expressions.hpp"
#include "ttl/concepts/expression.hpp"
#include "ttl/concepts/scalar.hpp"
#include "ttl/utils/ops.hpp"

namespace ttl
{
    template <concepts::expression A, concepts::expression B>
    constexpr auto operator+(A&& a, B&& b)
    {
        return expressions::Sum<A, B, utils::plus> {
            ._a = FWD(a),
            ._b = FWD(b)
        };
    }

    template <concepts::expression A, concepts::expression B>
    constexpr auto operator-(A&& a, B&& b)
    {
        return expressions::Sum<A, B, utils::minus> {
            ._a = FWD(a),
            ._b = FWD(b)
        };
    }

    template <concepts::expression A, concepts::expression B>
    constexpr auto operator*(A&& a, B&& b)
    {
        return expressions::Product<A, B, utils::multiply> {
            ._a = FWD(a),
            ._b = FWD(b)
        };
    }
}
