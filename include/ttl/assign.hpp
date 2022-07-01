#pragma once

#include "ttl/expression_traits.hpp"
#include "ttl/typed_index.hpp"
#include "ttl/utils.hpp"

namespace ttl
{
    constexpr struct equal_fn
    {
        constexpr auto operator()(auto&& a, auto&& b) const -> decltype(auto)
        {
            static_assert(requires { FWD(a) = FWD(b); });
            return FWD(a) = FWD(b);
        }
    } equal{};

    // This could be an expression node if we wanted it to be lazy.
    template <is_expression A, is_expression B, auto op = equal>
    constexpr auto assign(A&& a, B&& b, nttp_args<op> = {}) -> decltype(auto)
    {
        constexpr tensor_index ai = outer<A>;
        constexpr tensor_index bi = outer<B>;
        static_assert(not ai.has_synthetic(), "left hand side exposes a synthetic index");
        static_assert(not bi.has_synthetic(), "right hand side exposes a synthetic index");
        static_assert(is_permutation_of<ai, bi>, "attempting to assign expressions with incompatible indices");
        using a_index = typed_index<ai>;
        using b_index = typed_index<bi>;
        is_extents auto extents = ttl::extents(a);
        typed_index<ai> i;
        do {
            op(ttl::evaluate(a, a_index(i)), ttl::evaluate(b, b_index(i)));
        } while (carry_sum_add<0>(&i, extents));
        return FWD(a);
    }

    template <is_tensor A, is_expression B, auto op = equal>
    constexpr auto assign(A&& a, B&& b, nttp_args<op> = {}) -> decltype(auto)
        requires (not is_expression<A> and order<A> == order<B>)
    {
        constexpr tensor_index bi = outer<B>;
        static_assert(not bi.has_synthetic(), "right hand side exposes a synthetic index");
        is_extents auto extents = ttl::extents(a);
        typed_index<outer<B>> i;
        do {
            op(ttl::evaluate(a, i), ttl::evaluate(b, i));
        } while (carry_sum_add<0>(&i, extents));
        return FWD(a);
    }
}
