#pragma once

#include "ttl/concepts/tensor.hpp"
#include "ttl/traits/extents.hpp"
#include "ttl/traits/order.hpp"
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
