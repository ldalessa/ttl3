#pragma once

#include <concepts>

namespace ttl::traits
{
    template <class T>
    struct scalar_type;

    template <std::integral T>
    struct scalar_type<T>
    {
        using type = T;
    };

    template <std::floating_point T>
    struct scalar_type<T>
    {
        using type = T;
    };

    template <class T>
    requires requires { typename std::remove_cvref_t<T>::scalar_type; }
    struct scalar_type<T>
    {
        using type = typename std::remove_cvref_t<T>::scalar_type;
    };

    template <class T>
    using scalar_type_t = typename scalar_type<std::remove_cvref_t<T>>::type;
}
