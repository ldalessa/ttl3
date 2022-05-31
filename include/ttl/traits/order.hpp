#pragma once

#include <concepts>

namespace ttl::traits
{
    template <class>
    struct order
    {
        static constexpr struct{} value{};
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

    template <class T>
    requires requires { { T::order() } -> std::same_as<int>; }
    struct order<T>
    {
        static constexpr int value = T::order();
    };

    template <class T>
    inline constexpr auto order_v = order<std::remove_cvref_t<T>>::value;
}
