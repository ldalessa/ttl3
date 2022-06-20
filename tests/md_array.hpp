#pragma once

#include "ttl/FWD.hpp"
#include "ttl/concepts/shape.hpp"
#include "md_base.hpp"
#include <array>
#include <concepts>
#include <numeric>
#include <utility>

namespace ttl::tests
{
    template <class T, ttl::concepts::shape auto _shape>
    struct md_array : md_base<md_array<T, _shape>>
    {
        template <std::size_t... i>
        static constexpr auto _unpack(auto&& op, std::index_sequence<i...>) {
            return FWD(op)(i...);
        }

        static constexpr auto _unpack(auto&& op) {
            return _unpack(FWD(op), std::make_index_sequence<_shape.size()>{});
        }

        static constexpr auto _order = _shape.size();

        static constexpr auto _size = _unpack([](auto... i) {
            return (_shape[i] * ... * 1);
        });

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
