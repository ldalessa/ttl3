#pragma once

#include "ttl/traits/tensor.hpp"
#include <concepts>

namespace ttl::traits
{
    template <class> struct order;

    template <concepts::_has_tensor_order_trait T>
    struct order<T>
    {
        static constexpr int value = traits::tensor<std::remove_cvref_t<T>>::order;
    };

    template <std::integral T>
    struct order<T>
    {
        static constexpr int value = 0;
    };

    template <std::floating_point T>
    struct order<T>
    {
        static constexpr int value = 0;
    };
}

namespace ttl::concepts
{
    template <class T>
    concept _has_order_trait = requires {
        { traits::order<std::remove_cvref_t<T>>::value } -> std::convertible_to<int>;
    };

    template <class T>
    concept has_order = _has_order_trait<T>;

    template <class T>
    concept has_order_zero = has_order<T> and traits::order<std::remove_cvref_t<T>>::value == 0;
}

namespace ttl
{
    template <concepts::has_order T>
    constexpr int order_v = traits::order<std::remove_cvref_t<T>>::value;
}
