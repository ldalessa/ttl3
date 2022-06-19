#pragma once

#include "ttl/traits/tensor.hpp"
#include <concepts>

namespace ttl::traits
{
    template <class> struct value_type;

    template <concepts::_has_tensor_value_type_trait T>
    struct value_type<T>
    {
        using type = typename traits::tensor<std::remove_cvref_t<T>>::value_type;
    };

    template <std::integral T>
    struct value_type<T>
    {
        using type = T;
    };

    template <std::floating_point T>
    struct value_type<T>
    {
        using type = T;
    };
}

namespace ttl::concepts
{
    template <class T>
    concept _has_value_type_trait = requires {
        typename traits::value_type<std::remove_cvref_t<T>>::type;
    };

    template <class T>
    concept has_value_type = _has_value_type_trait<T>;
}

namespace ttl
{
    template <concepts::_has_value_type_trait T>
    using value_type_t = typename traits::value_type<std::remove_cvref_t<T>>::type;
}
