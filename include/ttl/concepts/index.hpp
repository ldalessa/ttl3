#pragma once

#include "ttl/Index.hpp"
#include <concepts>

namespace ttl::concepts
{
    template <class T>
    concept index = requires (T t) {
        { ttl::index_to_value(t) } -> std::same_as<int>;
        { ttl::index_to_type(t) } -> std::same_as<IndexType>;
        { ttl::index_to_wchar(t) } -> std::same_as<wchar_t>;
    };
}
