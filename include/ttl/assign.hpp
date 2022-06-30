#pragma once

#include "expression_traits.hpp"
#include "utils.hpp"

namespace ttl
{
    // This could be an expression node if we wanted it to be lazy.
    template <is_expression A, is_expression B, auto op>
    constexpr auto assign(A&& a, B&& b, nttp_args<op>) ->
        decltype(auto)
    {
        constexpr tensor_index ai = outer<A>;
        constexpr tensor_index bi = outer<B>;
        static_assert(is_permutation_of<ai, bi>);
        using a_index = typed_index<ai>;
        using b_index = typed_index<bi>;
        array extents = ttl::extents(a);
        typed_index<ai> i;
        do {
            op(ttl::evaluate(a, a_index(i)), ttl::evaluate(b, b_index(i)));
        } while (carry_sum_add<0>(&i, extents));
        return FWD(a);
    }
}
