#pragma once

#include "ttl/FWD.hpp"
#include "ttl/concepts/shape.hpp"
#include "md_base.hpp"
#include <algorithm>                            // reverse
#include <functional>                           // multiplies
#include <numeric>                              // exclusive_scan
#include <type_traits>                          // remove_const_t
#include <utility>                              // index_sequence

namespace ttl::tests
{
    template <class T, ttl::concepts::shape auto _shape>
    struct md_array : md_base<md_array<T, _shape>>
    {
        static constexpr auto _order = _shape.size();

        static constexpr auto _size = []<std::size_t... i>(std::index_sequence<i...>) {
            return (_shape[i] * ... * 1);
        }(std::make_index_sequence<_order>{});

        static constexpr auto _strides = [] {
            using shape_t = std::remove_const_t<decltype(_shape)>;
            shape_t strides{};
            std::exclusive_scan(_shape.begin(), _shape.end(), strides.begin(), 1, std::multiplies{});
            std::reverse(strides.begin(), strides.end());
            return strides;
        }();

        T _data[_size];

        /// Total number of "dimensions" ("N-dimensional array" usage).
        static consteval auto order() -> int {
            return _order;
        }

        /// Total number of elements.
        static consteval auto size() -> int {
            return _size;
        }
    };
}
