#pragma once

#include "ttl/Bindable.hpp"
#include "ttl/TensorIndex.hpp"
#include "ttl/concepts/expression.hpp"
#include "ttl/traits/order.hpp"
#include "ttl/traits/outer.hpp"
#include "ttl/traits/scalar_type.hpp"
#include "ttl/utils/FWD.hpp"
#include "ttl/utils/sequence.hpp"

namespace ttl::expressions
{
    template <concepts::expression A, concepts::expression B, auto op>
    struct Product : Bindable<Product<A, B, op>>
    {
        using scalar_type = std::common_type_t<traits::scalar_type_t<A>, traits::scalar_type_t<B>>;

        static constexpr TensorIndex _ai = traits::outer_v<A>;
        static constexpr TensorIndex _bi = traits::outer_v<B>;
        static constexpr TensorIndex _outer = _ai ^ _bi;
        static constexpr TensorIndex _contracted = _ai & _bi;
        static constexpr TensorIndex _inner = join(_outer, _contracted);

        static constexpr int _order = _outer.order();

        A _a;
        B _b;

        static consteval auto order() -> int {
            return _order;
        }

        static consteval auto outer() {
            return _outer;
        }

        constexpr operator scalar_type() const
            requires(_order == 0)
        {
            return evaluate(ScalarIndex<0>{});
        }

        template <int M>
        constexpr auto extent() const -> int
        {
            static_assert(M < _order);
            map_extents<_contracted, _ai, A> extents{_a};
            return ttl::extent<M>(extents);
        }

        constexpr auto evaluate(ScalarIndex<_order> const& index) const -> scalar_type
        {
            map_extents<_inner, _ai, A> extents{_a};
            scalar_type c{};
            ScalarIndex<_inner.size()> i = index;
            do {
                auto&& a = ttl::evaluate(_a, ttl::select<_inner, _ai>(i));
                auto&& b = ttl::evaluate(_b, ttl::select<_inner, _bi>(i));
                c += op(FWD(a), FWD(b));
            } while (ttl::carry_sum_inc<_order>(i, extents));
            return c;
        }

        static constexpr auto _validate_shapes() -> bool
            requires (concepts::static_extents<A> and concepts::static_extents<B>)
        {
            constexpr int n = _contracted.size();
            return []<int... i>(utils::sequence<i...>) {
                constexpr map_extents<_contracted, _ai, A> a{};
                constexpr map_extents<_contracted, _bi, B> b{};
                return ((ttl::extent<i>(a) == ttl::extent<i>(b)) && ...);
            }(utils::sequence_v<n>);
        }

        constexpr auto _validate_shapes() const -> bool
        {
            constexpr int n = _contracted.size();
            return [&]<int... i>(utils::sequence<i...>)
            {
                map_extents<_contracted, _ai, A> a{_a};
                map_extents<_contracted, _bi, B> b{_b};
                return ((ttl::extent<i>(a) == ttl::extent<i>(b)) && ...);
            }(utils::sequence_v<n>);
        }
    };
}

namespace ttl::traits
{
    /// Propagate the static extents through the bind.
    template <int M, concepts::static_extents A, concepts::static_extents B, auto op>
    struct extent<M, expressions::Product<A, B, op>>
    {
        static constexpr int value = traits::extent_v<M, A>;
    };
}
