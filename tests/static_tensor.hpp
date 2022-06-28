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

        static constexpr auto _order = _shape.order();
        static constexpr auto _size = _shape.size();

        md_array<T, _shape> _data;

        /// Variadic constructor for a set of scalars.
        constexpr static_tensor(std::convertible_to<T> auto&&... vs)
                : _data{ static_cast<T>(vs)... }
        {
        }

        static consteval auto get_order() -> int {
            return _order;
        }

        static constexpr auto get_extents() -> extents_type const& {
            return _shape;
        }

        // In the scalar index, the first
        constexpr auto evaluate(ttl::scalar_index<_order> const& index) const -> auto& {
            return _data[_shape.linearize(index)];
        }

        constexpr auto evaluate(ttl::scalar_index<_order> const& index) -> auto& {
            return _data[_shape.linearize(index)];
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
