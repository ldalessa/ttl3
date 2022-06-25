#pragma once

#include "scalar_index.hpp"
#include "tensor_index.hpp"
#include "tensor_traits.hpp"

namespace ttl
{
    template <class> struct bindable;

    template <has_tensor_traits A, is_tensor_index auto _inner>
    struct bind : bindable<bind<A, _inner>>
    {
        using scalar_type = scalar_type_t<A>;

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

        static consteval auto get_order() -> int {
            return _order;
        }

        constexpr auto get_extents() const -> array<int, _order> {
            return {};
        }

        constexpr operator scalar_type() const requires (_order == 0) {
            return evaluate(typed_index<_outer>{});
        }

        constexpr auto evaluate(typed_index<_outer> const& outer) const -> decltype(auto) {
            typed_index i = outer + _p;
            return ttl::evaluate(_a, typed_index<_inner>(outer));
        }
    };

    template <class A, is_index_or_int... Is>
    bind(A&&, Is...) -> bind<A, tensor_index(Is{}...)>;
}
