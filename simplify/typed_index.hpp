#pragma once

#include "scalar_index.hpp"
#include "utils.hpp"

namespace ttl
{
    template <is_tensor_index auto _type>
    struct typed_index : scalar_index<_type.size()>
    {
        static constexpr int _size = _type.size();

        using scalar_index<_type.size()>::scalar_index;

        template <is_tensor_index auto b>
        constexpr typed_index(nttp_args<b>, is_index_or_int auto... all)
            requires (_type.is_subset_of(b) and sizeof...(all) == b.size())
        {
            constexpr array map = _type.gather_from(b);
            array is{ttl::to_int(all)...};
            for (int i = 0; i < _size; ++i) {
                _at(i) = is[map[i]];
            }
        }

        template <is_tensor_index auto other>
        constexpr typed_index(typed_index<other> const& b) requires (_type.is_subset_of(other))
        {
            constexpr array map = _type.gather_from(b);
            for (int i = 0; i < _size; ++i) {
                _at(i) = b[map[i]];
            }
        }

        template <is_tensor_index auto other>
        constexpr auto operator+(typed_index<other> const& b) const -> typed_index<concat<_type, other>>
        {
            return typed_index<concat<_type, other>>(*this, b);
        }

        constexpr auto _at(int i) const -> decltype(auto) {
            return (*this)[i];
        }

        constexpr auto _at(int i) -> decltype(auto) {
            return (*this)[i];
        }

        [[noreturn]]
        typed_index(scalar_index<_size> const& b) : scalar_index<_size>(b)
        {
            // Sort of fake-out the has_tensor_traits concept. This allows
            // expression types, which implement evaluate for typed indices, to
            // pretend to have the tensor_trait that just uses a scalar index.
            throw "shouldn't ever use this";
        }
    };
}
