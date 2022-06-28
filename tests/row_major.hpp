#pragma once

#include "ttl/array.hpp"

#include <cassert>
#include <concepts>
#include <functional>
#include <numeric>
#include <ranges>

namespace ttl::tests
{
    template <int _order, std::integral _size_t>
    struct row_major
    {
        ttl::array<_size_t, _order> _extents;
        ttl::array<_size_t, _order> _strides;

        constexpr row_major(std::convertible_to<_size_t> auto... extents)
                : _extents{extents...}
        {
            // note the rbegin/rends, this is because the first extent, which
            // should be _extents[0], should be the outermost extent during
            // "linearize".
            std::exclusive_scan(_extents.rbegin(), _extents.rend(), _strides.rbegin(), _size_t{1}, std::multiplies{});
        }

        constexpr auto order() const -> int {
            return _order;
        }

        constexpr auto size() const -> _size_t {
            return std::accumulate(_extents.begin(), _extents.end(), _size_t{1}, std::multiplies{});
        }

        constexpr auto begin() const -> decltype(auto) {
            return _extents.begin();
        }

        constexpr auto begin() -> decltype(auto)  {
            return _extents.begin();
        }

        constexpr auto end() const -> decltype(auto)  {
            return _extents.end();
        }

        constexpr auto end() -> decltype(auto) {
            return _extents.end();
        }

        constexpr auto operator[](_size_t i) const -> decltype(auto) {
            return _extents[i];
        }

        constexpr auto operator[](_size_t i) -> decltype(auto) {
            return _extents[i];
        }

        constexpr auto linearize(std::ranges::random_access_range auto&& index) const -> _size_t {
            _size_t offset = 0;
            for (int i = 0; i < _order; ++i) {
                offset += std::ranges::begin(index)[i] * _strides[i];
            }
            assert(offset < size());
            return offset;
        }

        constexpr auto operator()(std::convertible_to<_size_t> auto... is) const -> _size_t {
            static_assert(sizeof...(is) == _order);
            return linearize(ttl::array<_size_t, sizeof...(is)>{is...});
        }
    };

    row_major() -> row_major<0, int>;

    template <class _size_t>
    row_major(_size_t, std::convertible_to<_size_t> auto... is) ->
        row_major<sizeof...(is) + 1, _size_t>;
}
