#pragma once

#include "ttl/Bind.hpp"
#include "ttl/expressions.hpp"
#include "ttl/concepts/expression.hpp"
#include "ttl/concepts/scalar.hpp"
#include "ttl/utils/FWD.hpp"

namespace ttl
{
    // namespace concepts
    // {
    //     template <class T>
    //     concept tree_expr = concepts::expression<T> || concepts::scalar<T>;
    // }

    // template <concepts::expression A>
    // constexpr auto _bind(A&& a) {
    //     return FWD(a);
    // }

    // template <concepts::scalar A>
    // constexpr auto _bind(A&& a) {
    //     return Bind { FWD(a) };
    // }

    template <concepts::expression A, concepts::expression B>
    constexpr auto operator+(A&& a, B&& b)
    {
        return expressions::Addition { FWD(a), FWD(b) };
        // return expressions::Addition { _bind(FWD(a)), _bind(FWD(b)) };
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
