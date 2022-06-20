#pragma once

#include "ttl/concepts/tensor.hpp"
#include "ttl/traits/extents.hpp"
#include "ttl/traits/order.hpp"
#include "ttl/traits/outer.hpp"
#include "ttl/traits/value_type.hpp"

namespace ttl::expressions
{
    template <concepts::tensor>
    struct bind;
}

namespace ttl::traits
{
    template <concepts::tensor A>
    struct order<expressions::bind<A>>
    {
        static constexpr int value = order_v<A>;
    };

    template <concepts::tensor A>
    struct value_type<expressions::bind<A>>
    {
        using type = value_type_t<A>;
    };

    template <concepts::tensor A>
    requires concepts::has_static_extents<A>
    struct extents<expressions::bind<A>>
    {
        static constexpr concepts::shape auto value = extents_v<A>;
    };
}

namespace ttl::expressions
{
    template <concepts::tensor A>
    struct bind
    {
        using index_t = index<order_v<A>>;
        // using projection_t = scalar_index<order_v<A>>;

        A _a;
        index_t _index;
        // projection_t _projected;

        constexpr friend auto tag_invoke(cpos::outer, bind const& b) {
            return b._index.unique();
        }

        constexpr friend auto tag_invoke(cpos::extents, bind const& b) {
            return ttl::extents(b._a);
        }
    };
}