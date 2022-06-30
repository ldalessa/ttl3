#pragma once

#include "ttl/assign.hpp"
#include "ttl/product.hpp"
#include "ttl/sum.hpp"
#include "ttl/tensor_traits.hpp"
#include "ttl/utils.hpp"
#include <concepts>
#include <functional>

namespace ttl
{
    template <class T>
    requires std::integral<T> or std::floating_point<T>
    struct tensor_traits<T>
    {
        using scalar_type = T;

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

    namespace _detail
    {
        constexpr struct plus_eq_fn
        {
            constexpr auto operator()(auto&& a, auto&& b) const -> decltype(auto)
            {
                static_assert(requires { FWD(a) += FWD(b); });
                return FWD(a) += FWD(b);
            }
        } plus_eq{};

        constexpr struct minus_eq_fn
        {
            constexpr auto operator()(auto&& a, auto&& b) const -> decltype(auto)
            {
                static_assert(requires { FWD(a) -= FWD(b); });
                return FWD(a) -= FWD(b);
            }
        } minus_eq{};
    }

    inline namespace operators
    {
        template <is_expression A, is_expression B>
        inline constexpr auto operator+(A&& a, B&& b)
        {
            static_assert(is_permutation_of<outer<A>, outer<B>>);
            return sum(FWD(a), FWD(b), nttp<std::plus{}>);
        }

        template <is_expression A, is_expression B>
        inline constexpr auto operator-(A&& a, B&& b)
        {
            static_assert(is_permutation_of<outer<A>, outer<B>>);
            return sum(FWD(a), FWD(b), nttp<std::minus{}>);
        }

        template <is_expression A, is_expression B>
        inline constexpr auto operator*(A&& a, B&& b)
        {
            return product<A, B>(FWD(a), FWD(b));
        }

        template <is_expression A, is_expression B>
        inline constexpr auto operator+=(A&& a, B&& b) -> decltype(auto)
        {
            static_assert(is_permutation_of<outer<A>, outer<B>>);
            return assign(FWD(a), FWD(b), nttp<_detail::plus_eq>);
        }

        template <is_expression A, is_expression B>
        inline constexpr auto operator-=(A&& a, B&& b) -> decltype(auto)
        {
            static_assert(is_permutation_of<outer<A>, outer<B>>);
            return assign(FWD(a), FWD(b), nttp<_detail::minus_eq>);
        }
    }
}
