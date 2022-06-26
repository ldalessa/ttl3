#pragma once

#include "md_array.hpp"
#include "ttl/ttl.hpp"
#include <algorithm>
#include <array>
#include <concepts>
#include <utility>

namespace ttl::tests
{
    template <class T, auto _shape>
    struct static_tensor : md_array<T, _shape>, ttl::bindable<static_tensor<T, _shape>>
    {
        using scalar_type = T;

        using md_array<T, _shape>::_data;;
        using md_array<T, _shape>::order;
        using md_array<T, _shape>::size;
        using ttl::bindable<static_tensor<T, _shape>>::operator();

        /// Variadic constructor for a set of scalars.
        constexpr static_tensor(std::convertible_to<T> auto&&... vs)
            requires (0 <= sizeof...(vs) and sizeof...(vs) <= size())
            : md_array<T, _shape>{ ._data{ static_cast<T>(vs)... } }
        {
        }

        static consteval auto get_order() -> int {
            return order();
        }

        static consteval auto get_extents() -> decltype(auto) {
            return _shape;
        }

        constexpr auto evaluate(ttl::scalar_index<order()> const& index) const -> decltype(auto)
        {
            return [index]<std::size_t... is>(md_array<T, _shape> const& self, std::index_sequence<is...>) -> decltype(auto) {
                return self(index[is]...);
            }(*this, std::make_index_sequence<order()>{});
        }

        constexpr auto evaluate(ttl::scalar_index<order()> const& index) -> decltype(auto)
        {
            return [index]<std::size_t... is>(md_array<T, _shape>& self, std::index_sequence<is...>) -> decltype(auto) {
                return self(index[is]...);
            }(*this, std::make_index_sequence<order()>{});
        }
    };
}
