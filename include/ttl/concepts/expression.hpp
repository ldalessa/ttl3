#pragma once

#include "ttl/concepts/bindable.hpp"
#include "ttl/concepts/tensor_index.hpp"
#include "ttl/traits/outer.hpp"

namespace ttl::concepts
{
    template <class T>
    concept expression = bindable<T> and requires {
        { traits::outer_v<T> } -> concepts::tensor_index;
    };
}
