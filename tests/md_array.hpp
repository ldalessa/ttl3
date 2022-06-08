#pragma once

#include "ttl/utils/expect.hpp"
#include "ttl/utils/FWD.hpp"
#include <array>
#include <concepts>
#include <numeric>

namespace ttl::tests
{
    /// Really basic, row-major, multi-dimensional array storage.
    ///
    /// usage:
    ///   md_array<int>           x; // just a scalar
    ///   md_array<int, 1>        x; // int x[1]
    ///   md_array<int, 3, 3>     x; // int x[3][3]
    ///   md_array<int, 7, 42, 1> x; // int x[7][42][1]
    ///
    /// We're using this as backing tensor storage for testing purposes.
    template <class T, int... extents>
    requires ((extents > 0) && ...)
    struct md_array
    {
        /// Array order.
        static constexpr int _order = sizeof...(extents);

        /// A multidimensional index into the array.
        using _index_t = std::array<int, _order>;

        /// Capture the extents as an array.
        static constexpr _index_t _extents = { extents... };

        /// Precompute the strides for _row_major.
        static constexpr _index_t _strides = [] {
            _index_t strides;
            std::exclusive_scan(_extents.rbegin(), _extents.rend(), strides.rbegin(), 1, std::multiplies{});
            return strides;
        }();

        /// Number of elements.
        static constexpr int _size = (extents * ... * 1);

        /// Allocate storage.
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
        constexpr auto end() {
            return std::end(_data);
        }

        /// Multi-dimensional access.
        constexpr auto operator()(std::integral auto... is) const -> T const& {
            return _at(*this, is...);
        }

        /// Multi-dimensional access.
        constexpr auto operator()(std::integral auto... is) -> T& {
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
            return _at(FWD(self), _index_t{int(is)...});
        }

        /// Multidimensional access (auto& return handles const)
        static constexpr auto _at(auto&& self, auto&& index) -> auto&
            requires requires {{ index[0] } -> std::convertible_to<int>; }
        {
            return FWD(self)._data[_row_major(FWD(index))];
        }

        /// Compute a row-major index offset (msb in is[0]).
        static constexpr auto _row_major(auto&& index) -> int
            requires requires {{ index[0] } -> std::convertible_to<int>; }
        {
            int n = 0;
            for (int i = 0; i < _order; ++i) {
                n += index[i] * _strides[i];
            }
            expect(0 <= n and n < _size);
            return n;
        }
    };
}
