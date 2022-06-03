#pragma once

#include "ttl/concepts/tensor.hpp"
#include "ttl/cpos/extent.hpp"
#include "ttl/traits/order.hpp"
#include <concepts>

namespace ttl::concepts
{
    template <class T>
    concept non_scalar = tensor<T> and requires (T t) {
        { ttl::extent<traits::order_v<T> - 1>(t) } -> std::same_as<int>;
    };
}
