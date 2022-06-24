#pragma once

#include "ttl/concepts/shape.hpp"
#include "ttl/concepts/tensor_index.hpp"
#include <type_traits>

namespace ttl
{
    template <
        concepts::tensor_index auto from,
        concepts::tensor_index auto to,
        concepts::shape Shape>
    inline constexpr auto select(Shape&& extents)
        -> auto requires (from.capacity() == std::remove_cvref_t<Shape>::size())
    {
    }
}
