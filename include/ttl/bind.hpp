#pragma once

#include "ttl/typed_index.hpp"
#include "ttl/tensor_index.hpp"
#include "ttl/tensor_traits.hpp"

namespace ttl
{
    template <class> struct bindable;

    template <is_tensor A, is_tensor_index auto _inner>
    struct bind : bindable<bind<A, _inner>>
    {
        using value_type = value_type_t<A>;

        static constexpr tensor_index _outer = exported<_inner>;
        static constexpr tensor_index _projected = projected<_inner>;
        static constexpr tensor_index _contracted = contracted<_inner>;
        static constexpr tensor_index _passthrough = concat<_outer, _projected>;
        static constexpr tensor_index _all = concat<_passthrough, _contracted>;
        static constexpr int _order = _outer.size();

        A _a;
        typed_index<_projected> _p;

        constexpr bind(auto&& a, is_index_or_int auto... is) requires (sizeof...(is) == _inner.size())
            : _a(std::forward<A>(a))
            , _p(nttp<_inner>, is...)
        {
        }

        constexpr operator value_type() const requires (_order == 0) {
            return evaluate(typed_index<_outer>{});
        }

        static constexpr auto get_outer() -> tensor_index<_order> {
            return _outer;
        }

        constexpr auto get_extents() const -> array<int, _order> {
            return _gather_extents(nttp<_outer>);
        }

        constexpr auto evaluate(typed_index<_outer> const& outer) const -> decltype(auto)
        {
            array extents = _gather_extents(nttp<_all>);
            typed_index<_all> i = outer + _p;
            value_type sum{0};
            do {
                sum += ttl::evaluate(_a, typed_index<_inner>(i));
            } while (carry_sum_add<_passthrough.size()>(&i, extents));
            return sum;
        }

        constexpr auto evaluate(typed_index<_outer> const& outer) const -> decltype(auto)
            requires (_contracted.size() == 0)
        {
            return ttl::evaluate(_a, typed_index<_inner>(outer + _p));
        }

        constexpr auto evaluate(typed_index<_outer> const& outer) -> decltype(auto)
            requires (_contracted.size() == 0)
        {
            return ttl::evaluate(_a, typed_index<_inner>(outer + _p));
        }

        template <is_tensor_index auto to>
        constexpr auto _gather_extents(nttp_args<to>) const -> array<int, to.size()>
        {
            constexpr auto map = to.map_extents_from(_inner);
            auto&& inner = ttl::extents(_a);
            array<int, to.size()> out;
            for (int i = 0; i < to.size(); ++i) {
                out[i] = inner[map[i]];
            }
            return out;
        }
    };

    template <class A, is_index_or_int... Is>
    bind(A&&, Is...) -> bind<A, tensor_index(Is{}...)>;
}
