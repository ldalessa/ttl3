#pragma once

#include <type_traits>

namespace ttl::concepts
{
    template <class T>
    concept scalar_index = requires {
        typename std::remove_cvref_t<T>::scalar_index_concept_tag;
    };
}
