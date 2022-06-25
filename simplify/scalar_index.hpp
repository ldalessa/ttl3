#pragma once

#include "array.hpp"
#include "tensor_index.hpp"
#include "utils.hpp"

namespace ttl
{
    template <class T>
    concept is_scalar_index = is_int_array<T>;

    template <int _size>
    struct scalar_index : array<int, _size>
    {
        constexpr scalar_index() = default;

        constexpr scalar_index(is_index_or_int auto... is) requires (0 < sizeof...(is) and sizeof...(is) <= _size )
            : array<int, _size> { ._data { to_int(is)... }}
        {
        }

        constexpr scalar_index(is_int_array auto&& a, is_int_array auto&& b)
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

    template <is_int_array A, is_int_array B>
    scalar_index(A, B) -> scalar_index<A::size() + B::size()>;

    inline constexpr auto operator+(is_scalar_index auto&& a, is_scalar_index auto&& b) {
        return scalar_index(FWD(a), FWD(b));
    }

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
