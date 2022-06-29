#pragma once

#include "ttl/product.hpp"
#include "ttl/sum.hpp"
#include "ttl/tensor_traits.hpp"
#include "ttl/utils.hpp"
#include <concepts>

namespace ttl
{
    template <class T>
    requires std::integral<T> or std::floating_point<T>
    struct tensor_traits<T>
    {
        using scalar_type = T;

        static consteval auto get_order() -> int {
            return 0;
        }

        static constexpr auto get_extents(T) -> array<int, 0> {
            return {};
        }

        static constexpr auto evaluate(T const& t, scalar_index<0> const&) -> T {
            return t;
        }

        static constexpr auto evaluate(T& t, scalar_index<0> const&) -> T& {
            return t;
        }
    };

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
            return product<A, B>(FWD(a), FWD(b));
        }
    }
}
