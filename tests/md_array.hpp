#pragma once

#include "ttl/concepts/shape.hpp"
#include <array>
#include <concepts>
#include <numeric>
#include <utility>

namespace ttl::tests
{
    template <class T, ttl::concepts::shape auto _shape>
    struct md_array
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

        /// Linear iteration.
        constexpr auto begin() const {
            return std::begin(_data);
        }

        /// Linear iteration.
        constexpr auto end() const {
            return std::end(_data);
        }

        /// Multi-dimensional access.
        constexpr auto operator()(std::integral auto... is) const -> T const&
            requires(sizeof...(is) == order())
        {
            return _at(*this, is...);
        }

        /// Multi-dimensional access.
        constexpr auto operator()(std::integral auto... is) -> T&
            requires(sizeof...(is) == order())
        {
            return _at(*this, is...);
        }

        // Linear access.
        constexpr auto operator[](std::integral auto i) const -> T const& {
            return _data[i];
        }

        // Linear access.
        constexpr auto operator[](std::integral auto i) -> T& {
            return _data[i];
        }

        /// Multidimensional access (auto& return handles const)
        static constexpr auto _at(auto&& self, std::convertible_to<int> auto... is) -> auto&
            requires (sizeof...(is) == order())
        {
            return FWD(self)._data[_row_major(is...)];
        }

        static constexpr auto _row_major(std::integral auto... is) -> std::integral auto
            requires(sizeof...(is) == order())
        {
            return _unpack([=](auto... js) {
                return ((is * js) + ... + 0);
            });
        }

        // /// Multidimensional access (auto& return handles const)
        // static constexpr auto _at(auto&& self, auto&& index) -> auto&
        //     requires requires {{ index[0] } -> std::convertible_to<int>; }
        // {
        //     return FWD(self)._data[_row_major(FWD(index))];
        // }

        // /// Compute a row-major index offset (msb in is[0]).
        // static constexpr auto _row_major(auto&& index) -> int
        //     requires requires {{ index[0] } -> std::convertible_to<int>; }
        // {
        //     int n = 0;
        //     for (int i = 0; i < _order; ++i) {
        //         n += index[i] * _strides[i];
        //     }
        //     expect(0 <= n and n < _size);
        //     return n;
        // }
    };
}
