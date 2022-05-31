#pragma once

#include "ttl/concepts/bindable.hpp"
#include "ttl/traits/order.hpp"

namespace ttl::concepts
{
    template <class T>
    concept scalar = bindable<T> and (traits::order_v<T> == 0);
}
