#pragma once

#include "ttl/concepts/extents.hpp"
#include "ttl/traits/extent.hpp"
#include <concepts>

namespace ttl::concepts
{
    template <class T>
    concept static_extents = extents<T> and requires (T t) {
        { traits::extent_v<0, T> } -> std::convertible_to<int>;
    };
}
