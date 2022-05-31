#pragma once

#include "ttl/Bindable.hpp"
#include "ttl/concepts/expression.hpp"
#include "ttl/expressions/Product.hpp"
#include "ttl/utils/FWD.hpp"

namespace ttl::expressions
{
    struct _multiply_fn
    {
        constexpr auto operator()(auto&& a, auto&& b) const -> decltype(auto)
        {
            return FWD(a) * FWD(b);
        }

        consteval _multiply_fn(int) {}
    };

    inline constexpr _multiply_fn multiply{0};

    template <concepts::expression A, concepts::expression B>
    struct Multiplication : Product<A, B, multiply>, Bindable<Multiplication<A, B>>
    {
        using Product<A, B, plus>::Product;
    };

    template <concepts::expression A, concepts::expression B>
    Multiplication(A, B) -> Multiplication<A, B>;
}
