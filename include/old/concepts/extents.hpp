#pragma once

#include "ttl/concepts/non_scalar.hpp"
#include "ttl/cpos/extent.hpp"
#include <concepts>

namespace ttl::concepts
{
    template <class T>
    concept extents = non_scalar<T> and requires (T t) {
        { ttl::extent<0>(t) } -> std::convertible_to<int>;
    };
}
