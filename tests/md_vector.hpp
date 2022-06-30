#pragma once

#include <cassert>
#include <concepts>
#include <ranges>
#include <utility>
#include <vector>

namespace ttl::tests
{
    template <class T, int _order, class Shape>
    struct md_vector
    {
        Shape _shape{};
        std::vector<T> _data{};

        constexpr md_vector() requires(_order != 0) = default;

        constexpr explicit md_vector(std::convertible_to<int> auto... extents)
                : _shape(extents...)
                , _data(_shape.count())
        {
            static_assert(sizeof...(extents) == _order);
        }

        template < std::size_t... i>
        constexpr explicit md_vector(std::ranges::sized_range auto&& shape, std::index_sequence<i...>)
                : md_vector(shape[i]...)
        {
            assert(std::ranges::size(shape) == _order);
        }

        constexpr explicit md_vector(std::ranges::sized_range auto&& shape)
                : md_vector(FWD(shape), std::make_index_sequence<_order>{})
        {
            assert(std::ranges::size(shape) == _order);
        }

        constexpr auto shape() const -> Shape const& {
            return _shape;
        }

        constexpr auto reshape(std::ranges::sized_range auto&& shape) -> auto& {
            assert(std::ranges::size(shape) == _order);
            [&]<std::size_t... i>(std::index_sequence<i...>) {
                _shape = Shape(std::ranges::begin(shape)[i]...);
            }(std::make_index_sequence<_order>{});
            _data.resize(_shape.count());
            return *this;
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
