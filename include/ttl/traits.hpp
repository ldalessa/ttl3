#pragma once

#include "ttl/concepts/shape.hpp"
#include <concepts>


namespace ttl::concepts
{
    template <class T>
    concept number = requires (T x, T y) {
        { x + y } -> std::convertible_to<T>;
        { x - y } -> std::convertible_to<T>;
        { x * y } -> std::convertible_to<T>;
        { x / y } -> std::convertible_to<T>;
        { x += y } -> std::same_as<T&>;
        { x -= y } -> std::same_as<T&>;
        { x *= y } -> std::same_as<T&>;
        { x /= y } -> std::same_as<T&>;
    } and traits<std::remove_cvref_t<T>>::order == 0;
}
