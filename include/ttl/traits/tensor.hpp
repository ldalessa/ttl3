#pragma once

#include "ttl/FWD.hpp"
#include "ttl/concepts/shape.hpp"
#include <concepts>

namespace ttl::traits
{
    /// Users can define these traits so that we can use their types as
    /// tensors. Traits can also be specialized individually (sometimes it makes
    /// more sense to do so).
    template <class>
    struct tensor;
}

namespace ttl::concepts
{
    template <class T>
    concept _has_tensor_value_type_trait = requires {
        typename traits::tensor<std::remove_cvref_t<T>>::value_type;
    };

    template <class T>
    concept _has_tensor_order_trait = requires {
        { traits::tensor<std::remove_cvref_t<T>>::order } -> std::convertible_to<int>;
    };

    template <class T>
    concept _has_tensor_extents_trait = requires {
        { traits::tensor<std::remove_cvref_t<T>>::extents } -> concepts::shape;
    };

    template <class T>
    concept _has_tensor_extents_call = requires (T&& t) {
        { traits::tensor<std::remove_cvref_t<T>>::extents(FWD(t)) } -> concepts::shape;
    };
}
