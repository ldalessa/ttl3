#pragma once

#include "ttl/traits/extent.hpp"
#include "ttl/concepts/non_scalar.hpp"
#include <concepts>

namespace ttl::concepts
{
    template <class T>
    concept static_extents = non_scalar<T> and requires {
        { traits::extent_v<0, T> } -> std::convertible_to<int>;
    };
}
