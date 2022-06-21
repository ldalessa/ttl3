#pragma once

#include "ttl/scalar_index.hpp"
#include "ttl/tensor_index.hpp"
#include "ttl/concepts/bind_expression.hpp"
#include "ttl/concepts/shape.hpp"
#include "ttl/concepts/tensor.hpp"
#include "ttl/concepts/tensor_index.hpp"
#include "ttl/traits/extents.hpp"
#include "ttl/traits/order.hpp"
#include "ttl/traits/outer.hpp"
#include "ttl/traits/value_type.hpp"

namespace ttl::expressions
{
    template <concepts::tensor A, concepts::tensor_index auto _index>
    struct bind
    {
        using bind_expression_concept = void;

        using outer_index_t = scalar_index<order_v<bind>>;
        using inner_index_t = scalar_index<_index.size()>;

        static constexpr tensor_index _outer = _index.unique();
        static constexpr int _order = order_v<bind>;

        A _a;
        inner_index_t _projected;

        constexpr bind(concepts::tensor auto&& b, concepts::index_or_integral auto... is)
                : _a(std::forward<A>(b))
                , _projected(is...)
        {
        }

        constexpr static auto outer()
        {
            return _outer;
        }

        /// Map extents from the bound tensor.
        ///
        /// Basically, if I have a tensor that has extents like [3][3][10], and
        /// the bind expression contracts the first two ranks, then the bind has
        /// a single extent of 10.
        ///
        /// This is written as a static member because
        constexpr static auto _extents(concepts::shape auto&& a_extents)
            -> std::array<int, _order>
        {
            constexpr std::array<int, _order> map = _outer.index_map(_index);
            std::array<int, _order> out;
            for (int i = 0; i < _order; ++i) {
                out[i] = a_extents[map[i]];
            }
            return out;
        }

        constexpr auto evaluate(outer_index_t const&) const -> decltype(auto)
        {
            return ttl::evaluate(_a, _projected);
        }

        constexpr auto evaluate(outer_index_t const&) -> decltype(auto)
        {
            return ttl::evaluate(_a, _projected);
        }

        constexpr friend auto tag_invoke(cpos::outer, bind const&) {
            return outer();
        }

        constexpr friend auto tag_invoke(cpos::extents, bind const& b)
            -> decltype(auto) requires (not concepts::has_static_extents<A>)
        {
            return _extents(ttl::extents(b._a));
        }

        constexpr friend auto tag_invoke(cpos::evaluate, concepts::bind_expression auto&& b, outer_index_t const& index)
            -> decltype(auto)
        {
            return b.evaluate(index);
        }
    };

    template <concepts::tensor A, concepts::index_or_integral... Is>
    bind(A&&, Is...) -> bind<A, tensor_index{Is{}...}>;
}


namespace ttl::traits
{
    template <concepts::tensor A, concepts::tensor_index auto _index>
    struct order<expressions::bind<A, _index>>
    {
        static constexpr int value = _index.order();
    };

    template <concepts::tensor A, concepts::tensor_index auto _index>
    struct value_type<expressions::bind<A, _index>>
    {
        using type = value_type_t<A>;
    };

    template <concepts::tensor A, concepts::tensor_index auto _index>
    requires concepts::has_static_extents<A>
    struct extents<expressions::bind<A, _index>>
    {
        static constexpr concepts::shape auto value = expressions::bind<A, _index>::_extents(extents_v<A>);
    };
}
