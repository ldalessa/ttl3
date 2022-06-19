#pragma once

#include "ttl/concepts/tensor.hpp"
#include "ttl/concepts/tensor_index.hpp"
#include "ttl/cpos/evaluate.hpp"
#include "ttl/traits/outer.hpp"
#include "ttl/traits/scalar_type.hpp"
#include <concepts>

namespace ttl::concepts
{
    template <class T>
    concept expression = tensor<T> and requires (T t, ScalarIndex<traits::order_v<T>> i) {
        { traits::outer_v<T> } -> concepts::tensor_index;
        { ttl::evaluate(t, i) } -> std::convertible_to<traits::scalar_type_t<T>>;
    };
}
