#pragma once

#include <type_traits>

namespace ttl::concepts
{
    template <class T>
    concept bind_expression = requires {
        typename std::remove_cvref_t<T>::bind_expression_concept;
    };
}
