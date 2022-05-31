#pragma once

#include "ttl/traits/order.hpp"
#include "ttl/traits/scalar_type.hpp"

namespace ttl::concepts
{
    template <class T>
    concept bindable = requires {
        typename traits::scalar_type_t<T>;
        { traits::order_v<T> } -> std::same_as<int const&>;
    };
}
