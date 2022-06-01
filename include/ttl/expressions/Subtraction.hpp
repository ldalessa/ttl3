#pragma once

#include "ttl/Bindable.hpp"
#include "ttl/concepts/expression.hpp"
#include "ttl/expressions/Sum.hpp"
#include "ttl/utils/FWD.hpp"
#include "ttl/utils/ops.hpp"

namespace ttl::expressions
{
    template <concepts::expression A, concepts::expression B>
    struct Subtraction : Sum<A, B, utils::minus>, Bindable<Subtraction<A, B>>
    {
        using Sum<A, B, utils::minus>::Sum;
    };

    template <concepts::expression A, concepts::expression B>
    Subtraction(A, B) -> Subtraction<A, B>;
}
