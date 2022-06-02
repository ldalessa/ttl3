#pragma once

#include "ScalarIndex.hpp"
#include "TensorIndex.hpp"
#include "ttl/concepts/bindable.hpp"
#include "ttl/concepts/tensor_index.hpp"
#include "ttl/cpos/evaluate.hpp"
#include "ttl/traits/order.hpp"
#include "ttl/traits/scalar_type.hpp"
#include "ttl/utils/expect.hpp"
#include "ttl/utils/nttp_args.hpp"
#include <utility>

namespace ttl
{
    template <class T>
    struct Bindable;

    template <
        concepts::bindable A,
        concepts::tensor_index auto _index>
    struct Bind : Bindable<Bind<A, _index>>
    {
        using projected_index_t = ScalarIndex<_index.capacity()>;
        using scalar_type = traits::scalar_type_t<A>;

        static constexpr concepts::tensor_index auto _outer = _index.outer();
        static constexpr int _order = _outer.order();

        A _a;
        projected_index_t _projected;

        constexpr explicit Bind(concepts::bindable auto&& a, concepts::index auto... is)
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

        static consteval auto outer()
        {
            return _outer;
        }

        static consteval auto order() -> int
        {
            return _order;
        }

        constexpr operator traits::scalar_type_t<A>() const
            requires(_order == 0)
        {
            return evaluate(ScalarIndex<0>{});
        }

        template <int N>
        constexpr auto dim() const -> int
        {
            return _a.template dim<N>();
        }

        constexpr auto evaluate(ScalarIndex<_order> const& index) const -> traits::scalar_type_t<A>
            requires(_index.contracted().size() != 0)
        {
            constexpr TensorIndex outer = _index.outer();
            constexpr TensorIndex inner = _index.contracted();
            constexpr TensorIndex   all = join(outer, inner);

            traits::scalar_type_t<A> sum{};

            ScalarIndex<all.size()> i = index;
            do {
                sum += ttl::evaluate(_a, ttl::select<all, _index>(i, _projected));
            } while (carry_sum_inc<_order>(i, _a));
            return sum;
        }

        constexpr auto evaluate(ScalarIndex<order()> index) const -> decltype(auto)
            requires(_index.contracted().size() == 0)
        {
            return 0;
        }

        constexpr auto evaluate(ScalarIndex<order()> index) -> decltype(auto)
            requires(_index.contracted().size() == 0)
        {
            return 0;
        }
    };

    template <concepts::bindable A>
    Bind(A&&, concepts::index auto... is)
        -> Bind<A, TensorIndex{utils::types<decltype(is)...>}>;
}
