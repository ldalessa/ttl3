#pragma once

#include "ttl/concepts/expression.hpp"
#include <utility>

namespace ttl::expressions
{
    template <concepts::expression A, concepts::expression B>
    struct Binary
    {
        A _a;
        B _b;

        constexpr Binary(A a, B b)
                : _a(std::move(a))
                , _b(std::move(b))
        {
        }
    };
}
