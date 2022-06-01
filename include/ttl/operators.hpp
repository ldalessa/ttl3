#pragma once

#include "ttl/Bind.hpp"
#include "ttl/expressions.hpp"
#include "ttl/concepts/expression.hpp"
#include "ttl/concepts/scalar.hpp"
#include "ttl/utils/FWD.hpp"

namespace ttl
{
    template <concepts::expression A, concepts::expression B>
    constexpr auto operator+(A&& a, B&& b)
    {
        return expressions::Addition { FWD(a), FWD(b) };
    }

    template <concepts::expression A, concepts::expression B>
    constexpr auto operator-(A&& a, B&& b)
    {
        return expressions::Subtraction { FWD(a), FWD(b) };
    }

    template <concepts::expression A, concepts::expression B>
    constexpr auto operator*(A&& a, B&& b)
    {
        return expressions::Multiplication { FWD(a), FWD(b) };
    }
}
