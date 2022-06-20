#pragma once

#include <concepts>

namespace ttl::concepts
{
    template <class T>
    concept index = requires {
        typename std::remove_cvref_t<T>::index_concept;
    };

    template <class T>
    concept index_or_integral = index<T> or std::integral<T>;
}
