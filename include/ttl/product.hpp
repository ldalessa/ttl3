#pragma once

#include "ttl/expression_traits.hpp"
#include <type_traits>

namespace ttl
{
    template <is_expression A, is_expression B>
    struct product
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

        constexpr operator scalar_type() const requires (_order == 0) {
            return evaluate(typed_index<_outer>{});
        }

        static consteval auto get_order() -> int {
            return _order;
        }

        static consteval auto get_outer() -> tensor_index<_order> {
            return _outer;
        }

        constexpr auto get_extents() const -> decltype(auto) {
            return _gather_extents(nttp<_outer>);
        }

        constexpr auto evaluate(typed_index<_outer> const& outer) const -> decltype(auto)
        {
            array extents = _gather_extents(nttp<_inner>);
            typed_index<_inner> i = outer;
            scalar_type sum{0};
            do {
                sum += ttl::evaluate(_a, i) * ttl::evaluate(_b, i);
            } while (carry_sum_add<_outer.size()>(&i, extents));
            return sum;
        }

        template <is_tensor_index auto to>
        constexpr auto _gather_extents(nttp_args<to>) const -> array<int, to.size()>
        {
            constexpr auto map = to.gather_from(_all);
            auto&& all = join(ttl::extents(_a), ttl::extents(_b));
            array<int, to.size()> out;
            for (int i = 0; i < to.size(); ++i) {
                out[i] = all[map[i]];
            }
            return out;
        }
    };
}
