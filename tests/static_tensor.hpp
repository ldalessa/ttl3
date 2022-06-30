#pragma once

#include "md_array.hpp"
#include "ttl/ttl.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <utility>

namespace ttl::tests
{
    template <class T, auto _shape>
    struct static_tensor : ttl::bindable<static_tensor<T, _shape>>
    {
        using scalar_type = T;
        using extents_type = decltype(_shape);

        static constexpr auto _order = _shape.size();
        static constexpr auto _size = _shape.count();

        md_array<T, _shape> _data;

        /// Variadic constructor for a set of scalars.
        constexpr static_tensor(std::convertible_to<T> auto&&... vs)
                : _data{ static_cast<T>(vs)... }
        {
        }

        constexpr static_tensor(ttl::is_expression_of_order<_order> auto&& rhs) {
            ttl::assign(*this, FWD(rhs));
        }

        static constexpr auto get_extents() -> extents_type const& {
            return _shape;
        }

        constexpr auto evaluate(ttl::scalar_index<_order> const& index) const -> decltype(auto) {
            return _evaluate(*this, index);
        }

        constexpr auto evaluate(ttl::scalar_index<_order> const& index) -> decltype(auto) {
            return _evaluate(*this, index);
        }

        static constexpr auto _evaluate(auto&& self, ttl::scalar_index<_order> const& index) -> decltype(auto) {
            return _evaluate(FWD(self), index, std::make_index_sequence<_order>{});
        }

        template <std::size_t... is>
        static constexpr auto _evaluate(auto&& self, ttl::scalar_index<_order> const& index, std::index_sequence<is...>)
            -> decltype(auto)
        {
            return FWD(self)._data(index[is]...);
        }

        constexpr auto operator[](std::integral auto i) const -> auto& {
            assert(0 <= i and i < _size);
            return _data[i];
        }

        constexpr auto operator[](std::integral auto i) -> auto& {
            assert(0 <= i and i < _size);
            return _data[i];
        }
    };
}
