#pragma once

#include "ttl/array.hpp"
#include "ttl/tensor_index.hpp"
#include "ttl/utils.hpp"

namespace ttl
{
    template <class T>
    concept is_scalar_index = is_array_of<T, int>;

    template <int _size>
    struct scalar_index : array<int, _size>
    {
        constexpr scalar_index() = default;

        constexpr scalar_index(is_index_or_int auto... is) requires (0 < sizeof...(is) and sizeof...(is) <= _size)
            : array<int, _size> { ._data { to_int(is)... }}
        {
        }

        constexpr scalar_index(is_array_of<int> auto&& a, is_array_of<int> auto&& b)
        {
            int i = 0;
            if constexpr (0 < _size) {
                for (auto j : a) this->_data[i++] = j;
                for (auto j : b) this->_data[i++] = j;
            }
            if (i != _size) throw "invalid scalar index construction";
        }
    };

    scalar_index(std::convertible_to<int> auto... is) -> scalar_index<sizeof...(is)>;

    template <is_array_of<int> A, is_array_of<int> B>
    scalar_index(A, B) -> scalar_index<A::size() + B::size()>;

    template <int N>
    inline constexpr auto carry_sum_add(is_scalar_index auto* index, is_array auto&& extents) -> bool
    {
        for (int i = N; i < index->size(); ++i) {
            if (++(*index)[i] < extents[i]) {
                return true;
            }
            (*index)[i] = 0;
        }
        return false;
    }
}
