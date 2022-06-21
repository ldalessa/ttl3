#pragma once

#include "ttl/scalar_index.hpp"
#include "ttl/concepts/bind_expression.hpp"
#include "ttl/concepts/tensor.hpp"
#include "ttl/concepts/tensor_index.hpp"
#include "ttl/traits/extents.hpp"
#include "ttl/traits/order.hpp"
#include "ttl/traits/outer.hpp"
#include "ttl/traits/value_type.hpp"

namespace ttl::expressions
{
    template <concepts::tensor, concepts::tensor_index auto>
    struct bind;
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
        static constexpr concepts::shape auto value = extents_v<A>;
    };
}

namespace ttl::expressions
{
    template <concepts::tensor A, concepts::tensor_index auto _index>
    struct bind
    {
        using bind_expression_concept = void;

        using outer_index_t = scalar_index<order_v<bind>>;
        using inner_index_t = scalar_index<_index.size()>;

        A _a;
        inner_index_t _projected;

        constexpr bind(concepts::tensor auto&& b, concepts::index_or_integral auto... is)
                : _a(std::forward<A>(b))
                , _projected(is...)
        {
        }

        constexpr static auto outer()
        {
            return _index.unique();
        }

        constexpr auto extents() const -> decltype(auto)
        {
            return ttl::extents(_a);
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
            return b.extents();
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
