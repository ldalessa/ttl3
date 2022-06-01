#pragma once

#include "ttl/Bindable.hpp"
#include "ttl/concepts/expression.hpp"
#include "ttl/expressions/Product.hpp"
#include "ttl/utils/FWD.hpp"
#include "ttl/utils/ops.hpp"

namespace ttl::expressions
{
    template <concepts::expression A, concepts::expression B>
    struct Multiplication : Product<A, B, utils::multiply>, Bindable<Multiplication<A, B>>
    {
        using Product<A, B, utils::multiply>::Product;
    };

    template <concepts::expression A, concepts::expression B>
    Multiplication(A, B) -> Multiplication<A, B>;
}
