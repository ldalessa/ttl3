#pragma once

#include "ttl/ttl.hpp"
#include "md_vector.hpp"
#include <concepts>

namespace ttl::tests
{
    template <class T, int _order>
    struct dynamic_tensor : md_vector<T, _order>
    {
        constexpr explicit dynamic_tensor(std::integral auto... extents)
                : md_vector<T, _order> { extents... }
        {
        }

        constexpr friend auto tag_invoke(ttl::cpos::extents, dynamic_tensor const& t) {
            return t._shape;
        }
    };
}

namespace ttl::traits
{
    template <class T, int _order>
    struct tensor<ttl::tests::dynamic_tensor<T, _order>>
    {
        using value_type = T;
        static constexpr int order = _order;
    };
}
