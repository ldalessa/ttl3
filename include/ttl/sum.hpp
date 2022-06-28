#pragma once

#include "ttl/expression_traits.hpp"
#include "ttl/utils.hpp"
#include <type_traits>

namespace ttl
{
    template <class> struct bindable;

    constexpr struct
    {
        constexpr auto operator()(auto&& a, auto&& b) const -> decltype(auto)
            requires requires { FWD(a) + FWD(b); }
        {
            return FWD(a) + FWD(b);
        }
    } plus{};

    constexpr struct
    {
        constexpr auto operator()(auto&& a, auto&& b) const -> decltype(auto)
            requires requires { FWD(a) - FWD(b); }
        {
            return FWD(a) - FWD(b);
        }
    } minus{};

    template <is_expression A, is_expression B, auto _op>
    struct sum : bindable<sum<A, B, _op>>
    {
        using scalar_type = std::common_type_t<scalar_type_t<A>, scalar_type_t<B>>;

        static constexpr tensor_index _outer_a = outer<A>;
        static constexpr tensor_index _outer_b = outer<B>;
        static constexpr int _order = _outer_a.size();

        static_assert(_outer_a.is_permutation_of(_outer_b));

        A _a;
        B _b;

        constexpr sum(auto&& a, auto&& b, nttp_args<_op>)
                : _a(std::forward<A>(a))
                , _b(std::forward<B>(b))
        {
        }

        constexpr operator scalar_type() const requires(_order == 0) {
            return evaluate(typed_index<_outer_a>{});
        }

        static constexpr auto get_order() -> int {
            return _order;
        }

        static constexpr auto get_outer() -> tensor_index<_order> {
            return _outer_a;
        }

        constexpr auto get_extents() const -> decltype(auto) {
            return ttl::extents(_a);
        }

        constexpr auto evaluate(typed_index<_outer_a> const& outer) const -> decltype(auto) {
            return _op(ttl::evaluate(_a, outer), ttl::evaluate(_b, outer));
        }
    };

    template <is_expression A, is_expression B, auto op>
    sum(A&&, B&&, nttp_args<op>) -> sum<A, B, op>;
}
