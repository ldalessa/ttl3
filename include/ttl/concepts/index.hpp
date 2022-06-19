#pragma once

#include <type_traits>

namespace ttl::concepts
{
    template <class T>
    concept index = requires {
        typename std::remove_cvref_t<T>::index_concept;
    };
}
