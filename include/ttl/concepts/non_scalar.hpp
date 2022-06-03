#pragma once

#include "ttl/concepts/bindable.hpp"
#include "ttl/concepts/scalar.hpp"
#include "ttl/cpos/extent.hpp"
#include "ttl/traits/order.hpp"
#include <concepts>

namespace ttl::concepts
{
    template <class T>
    concept non_scalar = bindable<T> and not scalar<T> and requires (T t) {
        { ttl::extent<traits::order_v<T> - 1>(t) } -> std::same_as<int>;
    };
}
