#pragma once

#include <type_traits>

namespace ttl::concepts
{
    template <class T>
    concept tensor_index = requires {
        typename std::remove_cvref_t<T>::tensor_index_concept_tag;
    };
}
