#pragma once

#include "ttl/concepts/tensor.hpp"
#include "ttl/concepts/tensor_index.hpp"
#include "ttl/traits/outer.hpp"

namespace ttl::concepts
{
    template <class T>
    concept expression = tensor<T> and requires {
        { traits::outer_v<T> } -> concepts::tensor_index;
    };
}
