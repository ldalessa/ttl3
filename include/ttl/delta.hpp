#pragma once

#include "ttl/array.hpp"
#include "ttl/index.hpp"
#include "ttl/tensor_index.hpp"
#include "ttl/typed_index.hpp"
#include "ttl/utils.hpp"

namespace ttl
{
    template <class> struct bindable;

    template <ttl::is_tensor_index_of_size<2> auto _outer>
    struct delta : bindable<delta<_outer>>
    {
        using scalar_type = int;

        consteval delta(ttl::is_index auto, ttl::is_index auto) {}

        static constexpr auto get_extents() -> array<int, 2> {
            return {0,0};
        }

        static constexpr auto get_outer() -> ttl::tensor_index<2> {
            return _outer;
        }

        static constexpr auto evaluate(ttl::typed_index<_outer> const& i) -> int {
            return (i[0] == i[1]);
        }
    };

    template <wchar_t i, wchar_t j, int _u, int _v>
    delta(ttl::index<i, _u>, ttl::index<j, _v>) -> delta<ttl::tensor_index<2> {
        ttl::index<i, _u + SYNTHETIC>{},
        ttl::index<j, _v + SYNTHETIC>{}
    }>;
}
