#pragma once

#include "ttl/Bindable.hpp"
#include "ttl/concepts/expression.hpp"
#include "ttl/expressions/Sum.hpp"
#include "ttl/utils/FWD.hpp"
#include "ttl/utils/ops.hpp"

namespace ttl::expressions
{
    template <concepts::expression A, concepts::expression B>
    struct Addition : Sum<A, B, utils::plus>, Bindable<Addition<A, B>>
    {
        using Sum<A, B, utils::plus>::Sum;
    };

    template <concepts::expression A, concepts::expression B>
    Addition(A, B) -> Addition<A, B>;
}
