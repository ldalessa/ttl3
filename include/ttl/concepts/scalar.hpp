#pragma once

#include "ttl/concepts/tensor.hpp"
#include "ttl/traits/order.hpp"

namespace ttl::concepts
{
    template <class T>
    concept scalar = tensor<T> and has_order_zero<T>;
}
