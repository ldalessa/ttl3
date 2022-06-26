#pragma once

#include "ttl/product.hpp"
#include "ttl/sum.hpp"
#include "ttl/utils.hpp"

namespace ttl
{
    inline namespace operators
    {
        template <is_expression A, is_expression B>
        inline constexpr auto operator+(A&& a, B&& b)
        {
            return sum(FWD(a), FWD(b), nttp<plus>);
        }

        template <is_expression A, is_expression B>
        inline constexpr auto operator-(A&& a, B&& b)
        {
            return sum(FWD(a), FWD(b), nttp<minus>);
        }

        template <is_expression A, is_expression B>
        inline constexpr auto operator*(A&& a, B&& b)
        {
            return product {
                ._a = FWD(a),
                ._b = FWD(b)
            };
        }
    }
}
