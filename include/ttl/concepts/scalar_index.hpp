#pragma once

#include <concepts>

namespace ttl::concepts
{
    template <class T>
    concept scalar_index = requires (T t, int i) {
        // tag hack while we sort this out
        typename std::remove_cvref_t<T>::scalar_index_concept_tag;

        { std::remove_cvref_t<T>::size() } -> std::convertible_to<int>;
        { t[i] } -> std::convertible_to<int>;
    };
}
