#pragma once

#include "row_major.hpp"
#include <cassert>
#include <concepts>
#include <ranges>
#include <vector>

namespace ttl::tests
{
    template <class T, int _order, class Shape = row_major<_order, int>>
    struct md_vector
    {
        Shape _shape{};
        std::vector<T> _data{};

        constexpr md_vector() requires(_order != 0) = default;

        constexpr explicit md_vector(std::convertible_to<int> auto... extents)
                : _shape(extents...)
                , _data(_shape.count())
        {
        }

        static consteval auto order() -> int {
            return _order;
        }

        constexpr auto shape() const -> Shape const& {
            return _shape;
        }

        constexpr auto size() const {
            return _data.size();
        }

        constexpr auto begin() const {
            return std::ranges::begin(_data);
        }

        constexpr auto begin() {
            return std::ranges::begin(_data);
        }

        constexpr auto end() const {
            return std::ranges::begin(_data);
        }

        constexpr auto end() {
            return std::ranges::end(_data);
        }

        constexpr auto operator[](int i) const -> auto& {
            assert(0 <= i and i < size());
            return _data[i];
        }

        constexpr auto operator[](int i) -> auto& {
            assert(0 <= i and i < size());
            return _data[i];
        }

        constexpr auto operator()(std::convertible_to<int> auto... is) const -> auto& {
            static_assert(sizeof...(is) == _order);
            return (*this)[_shape(is...)];
        }

        constexpr auto operator()(std::convertible_to<int> auto... is) -> auto& {
            static_assert(sizeof...(is) == _order);
            return (*this)[_shape(is...)];
        }
    };
}
