#pragma once

#include "ttl/concepts/tensor.hpp"
#include "ttl/traits/order.hpp"
#include "ttl/traits/scalar_type.hpp"
#include <concepts>

namespace ttl::concepts
{
    template <class T>
    concept scalar = tensor<T> and (traits::order_v<T> == 0) and std::convertible_to<T, traits::scalar_type_t<T>>;
}
