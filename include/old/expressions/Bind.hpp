#pragma once

#include "ttl/ScalarIndex.hpp"
#include "ttl/TensorIndex.hpp"
#include "ttl/concepts/tensor.hpp"
#include "ttl/concepts/scalar.hpp"
#include "ttl/concepts/static_extents.hpp"
#include "ttl/concepts/tensor_index.hpp"
#include "ttl/cpos/evaluate.hpp"
#include "ttl/cpos/extent.hpp"
#include "ttl/traits/order.hpp"
#include "ttl/traits/scalar_type.hpp"
#include "ttl/utils/expect.hpp"
#include "ttl/utils/nttp_args.hpp"
#include <utility>

namespace ttl
{
    template <class> struct Bindable;
}

namespace ttl::expressions
{
    template <
        concepts::tensor A,
        concepts::tensor_index auto _index>
    struct Bind : Bindable<Bind<A, _index>>
    {
        using projected_index_t = ScalarIndex<_index.size()>;
        using scalar_type = traits::scalar_type_t<A>;

        static constexpr auto _outer = _index.outer();
        static constexpr auto _contracted = _index.contracted();
        static constexpr auto _inner = join(_outer, _contracted);
        static constexpr auto _order = _outer.order();

        A _a;
        projected_index_t _projected;

        constexpr explicit Bind(concepts::tensor auto&& a, concepts::index auto... is)
                : _a { std::forward<A>(a) }
                , _projected { is... }
        {
        }

        /// Default copy and move will prevent implicit operator= generation, which
        /// means that the `concepts::expression` version will match _all_ instances of
        /// Bind::operator=.
        /// @{
        constexpr Bind(Bind const&) = default;
        constexpr Bind(Bind&&) = default;
        /// @}

        static consteval auto outer() {
            return _outer;
        }

        static consteval auto order() -> int {
            return _order;
        }

        /// If this is an order 0 tensor then it can be converted to the
        /// underlying type.
        ///
        /// This is implicit and may require evaluation of a (perhaps expensive)
        /// contraction, but it is too convenient to ignore or to make explicit.
        constexpr operator traits::scalar_type_t<A>() const
            requires(_order == 0)
        {
            return evaluate(ScalarIndex<0>{});
        }

        /// Get the dimension for one of the indices.
        template <int N>
        constexpr auto extent() const -> int
        {
            static_assert(N < _order);
            map_extents<_inner, _index, A> extents{_a};
            return ttl::extent<N>(extents);
        }

        /// Evaluate the index when the Bind represents a contraction.
        constexpr auto evaluate(ScalarIndex<_order> const& index) const -> traits::scalar_type_t<A>
            requires(_index.contracted().size() != 0)
        {
            map_extents<_inner, _index, A> extents{_a};
            traits::scalar_type_t<A> sum{};
            ScalarIndex<_inner.size()> i = index;
            do {
                sum += ttl::evaluate(_a, ttl::select<_inner, _index>(i, _projected));
            } while (carry_sum_inc<_order>(i, extents));
            return sum;
        }

        /// Pass through the index with projection (if applicable).
        constexpr auto evaluate(ScalarIndex<order()> const& index) const -> decltype(auto)
            requires(_index.contracted().size() == 0)
        {
            return ttl::evaluate(_a, ttl::select<_outer, _index>(index, _projected));
        }

        /// Pass through the index with projection (if applicable).
        constexpr auto evaluate(ScalarIndex<order()> const& index) -> decltype(auto)
            requires(_index.contracted().size() == 0)
        {
            return ttl::evaluate(_a, ttl::select<_outer, _index>(index, _projected));
        }
    };

    template <concepts::tensor A>
    Bind(A&&, concepts::index auto... is)
        -> Bind<A, TensorIndex{utils::types<decltype(is)...>}>;
}

namespace ttl::traits
{
    /// Propagate the static extents through the bind.
    template <int M, concepts::static_extents A, concepts::tensor_index auto i>
    struct extent<M, expressions::Bind<A, i>>
    {
        static constexpr int value = traits::extent_v<M, A>;
    };
}

