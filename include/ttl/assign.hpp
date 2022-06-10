#pragma once

#include "ttl/TensorIndex.hpp"
#include "ttl/concepts/expression.hpp"
#include "ttl/expressions/Bind.hpp"
#include "ttl/cpos/evaluate.hpp"
#include "ttl/traits/order.hpp"
#include "ttl/traits/outer.hpp"

namespace ttl
{
    template <concepts::expression A, concepts::expression B>
    constexpr auto assign(A&& a, B&& b, auto&& op) -> decltype(auto)
    {
        static_assert(ttl::is_permutation(traits::outer_v<A>, traits::outer_v<B>));

        constexpr TensorIndex l = traits::outer_v<A>;
        constexpr TensorIndex r = traits::outer_v<B>;
        constexpr int order = traits::order_v<A>;

        ScalarIndex<order> i;
        do {
            op(ttl::evaluate(a, i), ttl::evaluate(b, ttl::select<l, r>(i)));
        } while (ttl::carry_sum_inc<0>(i, a));

        return FWD(a);
    }

    template <concepts::tensor A, concepts::expression B>
    constexpr auto assign(A&& a, B&& b, auto&& op) -> decltype(auto)
    {
        static_assert(traits::order_v<A> == traits::order_v<B>);
        return assign(expressions::Bind<A, traits::outer_v<B>>{FWD(a)}, FWD(b), FWD(op));
    }
}
