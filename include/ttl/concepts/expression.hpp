#pragma once

#include "ttl/traits/order.hpp"
#include "ttl/concepts/tensor.hpp"

namespace ttl::concepts
{
    template <class T>
    concept expression = tensor<T> and has_order<T>;
}
