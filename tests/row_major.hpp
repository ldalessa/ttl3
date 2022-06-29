#pragma once

#include <array>
#include <cassert>
#include <concepts>
#include <functional>
#include <initializer_list>
#include <numeric>
#include <ranges>

namespace ttl::tests
{
    template <int _order, std::integral _size_t>
    struct row_major
    {
        std::array<_size_t, _order> _extents;
        std::array<_size_t, _order> _strides;

        constexpr row_major(std::convertible_to<_size_t> auto... extents)
                : _extents{extents...}
        {
            // reverse iterators because extents are stored as big endian
            std::exclusive_scan(
                    _extents.rbegin(),
                    _extents.rend(),
                    _strides.rbegin(),
                    _size_t{1},
                    std::multiplies{});
        }

        static constexpr auto size() -> int {
            return _order;
        }

        constexpr auto count() const -> _size_t {
            return std::accumulate(
                    _extents.begin(),
                    _extents.end(),
                    _size_t{1},
                    std::multiplies{});
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

        constexpr auto linearize(std::ranges::input_range auto&& index) const -> _size_t {
            _size_t offset = 0;
            auto in = std::ranges::begin(index);
            for (int i = 0; i < _order; ++i) {
                offset += *(in++) * _strides[i];
            }
            assert(offset < count());
            return offset;
        }

        constexpr auto operator()(std::convertible_to<_size_t> auto... is) const -> _size_t {
            static_assert(sizeof...(is) == _order);
            return linearize(std::initializer_list<int>{is...});
        }
    };

    row_major() -> row_major<0, int>;

    template <class _size_t>
    row_major(_size_t, std::convertible_to<_size_t> auto... is) ->
        row_major<sizeof...(is) + 1, _size_t>;
}
