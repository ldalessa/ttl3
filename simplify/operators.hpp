#pragma once

#include "sum.hpp"

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
    }
}
