#pragma once

#include "ttl/expression_traits.hpp"
#include <type_traits>

namespace ttl
{
    template <is_expression A, is_expression B>
    struct product : ttl::bindable<product<A, B>>
    {
        using scalar_type = std::common_type_t<scalar_type_t<A>, scalar_type_t<B>>;

        static constexpr tensor_index _outer_a = outer<A>;
        static constexpr tensor_index _outer_b = outer<B>;
        static constexpr tensor_index _all = concat<_outer_a, _outer_b>;
        static constexpr tensor_index _outer = exported<_all>;
        static constexpr tensor_index _contracted = contracted<_all>;
        static constexpr tensor_index _inner = concat<_outer, _contracted>;
        static constexpr int _order = _outer.size();

        A _a;
        B _b;

        constexpr product(auto&& a, auto&& b)
            : _a(std::forward<A>(a))
            , _b(std::forward<B>(b))
        {
        }

        constexpr operator scalar_type() const requires (_order == 0) {
            return evaluate(typed_index<_outer>{});
        }

        static consteval auto get_outer() -> tensor_index<_order> {
            return _outer;
        }

        constexpr auto get_extents() const -> decltype(auto) {
            return _gather_extents(nttp<_outer>);
        }

        constexpr auto evaluate(typed_index<_outer> const& outer) const -> decltype(auto)
        {
            using a_index = typed_index<_outer_a>;
            using b_index = typed_index<_outer_b>;
            array extents = _gather_extents(nttp<_inner>);
            typed_index<_inner> i = outer;
            scalar_type sum{0};
            do {
                sum += ttl::evaluate(_a, a_index(i)) * ttl::evaluate(_b, b_index(i));
            } while (carry_sum_add<_outer.size()>(&i, extents));
            return sum;
        }

        template <is_tensor_index auto to>
        constexpr auto _gather_extents(nttp_args<to>) const -> array<int, to.size()>
        {
            constexpr auto map = to.gather_from(_all);
            array all = _join(ttl::extents(_a), ttl::extents(_b));
            array<int, to.size()> out;
            for (int i = 0; i < to.size(); ++i) {
                out[i] = all[map[i]];
            }
            return out;
        }

        template <is_extents T, is_extents U>
        static constexpr auto _join(T&& a, U&& b) -> array<int, size<T> + size<U>>
        {
            array<int, size<T> + size<U>> out;
            int i = 0;
            for (auto&& a : a) out[i++] = a;
            for (auto&& b : b) out[i++] = b;
            if (i != size<T> + size<U>) throw "error joining arrays";
            return out;
        }
    };

    // clang doesn't have CTAD for aggregates (P1816R0, P2082R1)
    template <is_expression A, is_expression B>
    product(A&&, B&&) -> product<A, B>;
}
