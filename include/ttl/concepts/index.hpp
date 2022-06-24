#pragma once

#include <concepts>

namespace ttl::concepts
{
    template <class T>
    concept index = requires {
        typename std::remove_cvref_t<T>::index_concept_tag;
    };

    template <class T>
    concept index_or_integral = std::integral<std::remove_cvref_t<T>> or index<T>;
}
