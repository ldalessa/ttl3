#pragma once

#include "ttl/TensorIndex.hpp"
#include "ttl/concepts/scalar.hpp"
#include "ttl/concepts/tensor_index.hpp"
#include <concepts>

namespace ttl::traits
{
    template <class>
    struct outer
    {
        static constexpr struct{} value{};
    };

    template <class T>
    requires requires {{ T::outer() } -> concepts::tensor_index; }
    struct outer<T>
    {
        static constexpr concepts::tensor_index auto value = T::outer();
    };

    template <concepts::scalar T>
    struct outer<T>
    {
        static constexpr TensorIndex<0> value = {};
    };

    template <class T>
    inline constexpr auto outer_v = outer<std::remove_cvref_t<T>>::value;
}
