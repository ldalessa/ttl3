#pragma once

#include "ttl/expression_traits.hpp"
#include "ttl/utils.hpp"
#include <type_traits>

namespace ttl
{
    template <class> struct bindable;

    template <is_expression A, is_expression B, auto _op>
    struct sum : bindable<sum<A, B, _op>>
    {
        using value_type = std::common_type_t<value_type_t<A>, value_type_t<B>>;

        static constexpr tensor_index _outer_a = outer<A>;
        static constexpr tensor_index _outer_b = outer<B>;
        static constexpr tensor_index _outer = merge_synthetic<_outer_a, _outer_b>;
        static constexpr int _order = _outer_a.size();

        static_assert(is_permutation_of<_outer_a, _outer_b>);

        A _a;
        B _b;

        constexpr sum(auto&& a, auto&& b, nttp_args<_op>)
                : _a(std::forward<A>(a))
                , _b(std::forward<B>(b))
        {
        }

        constexpr operator value_type() const requires(_order == 0) {
            return evaluate(typed_index<_outer_a>{});
        }

        static constexpr auto get_outer() -> tensor_index<_order> { /* consteval clang-14 */
            return _outer;
        }

        constexpr auto get_extents() const -> array<int, _order>
        {
            constexpr tensor_index all = _outer_a + _outer_b;
            constexpr array map = _outer.map_extents_from(all);
            array joined = join_extents(ttl::extents(_a), ttl::extents(_b));
            array<int, _order> extents;
            for (int i = 0; i < _order; ++i) {
                extents[i] = joined[map[i]];
            }
            return extents;
        }

        constexpr auto evaluate(typed_index<_outer_a> const& outer) const -> decltype(auto) {
            using a_index = typed_index<_outer_a>;
            using b_index = typed_index<_outer_b>;
            return _op(ttl::evaluate(_a, a_index(outer)), ttl::evaluate(_b, b_index(outer)));
        }
    };

    template <is_expression A, is_expression B, auto op>
    sum(A&&, B&&, nttp_args<op>) -> sum<A, B, op>;
}
