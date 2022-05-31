#pragma once

#include "ttl/Index.hpp"
#include "ttl/concepts/index.hpp"
#include "ttl/utils/error.hpp"
#include "ttl/utils/type_args.hpp"

namespace ttl
{
    template <int _n_max>
    struct TensorIndex
    {
        using tensor_index_tag = void;

        wchar_t   _data[_n_max]{};
        IndexType _types[_n_max]{};
        int _size{};

        consteval TensorIndex() = default;

        template <concepts::index... Is>
        consteval TensorIndex(utils::type_args<Is...>)
        {
            static_assert(sizeof...(Is) <= _n_max, "Invalid index count");

            (_push_back(index_to_wchar(Is{}), index_to_type(Is{})), ...);

            for (int i = 0; i < _size; ++i) {
                if (_count(_data[i]) > 2) {
                    utils::error("index appears more than two times");
                }
            }

            // make sure that this index makes sense
            for (int i = 0; i < _size; ++i) {
                if (_types[i] == IndexType::PROJECTED) {
                    continue;
                }
                for (int j = i + 1; j < _size; ++j) {
                    if (_types[j] == IndexType::PROJECTED) {
                        continue;
                    }
                    if (_data[i] != _data[j]) {
                        continue;
                    }
                    if (not matches(_types[i], _types[j])) {
                        utils::error("index appears as co (or contra) variant twice");
                    }
                }
            }
        }

        static consteval auto capacity() -> int {
            return _n_max;
        }

        /// Return the uncontracted indices.
        constexpr auto outer() const -> TensorIndex
        {
            TensorIndex out{};
            for (int i = 0; i < _size; ++i) {
                if (_count(_data[i]) == 1) {
                    out._push_back(_data[i], _types[i]);
                }
            }
            return out;
        }

        /// An index's order means the number of indices that are uncontracted.
        constexpr auto order() const -> int
        {
            return outer().size();
        }

        /// Return the size of the index (may be smaller than the capacity).
        constexpr auto size() const -> int {
            return _size;
        }

        /// Count the number of occurrences of the character.
        constexpr auto _count(wchar_t c) const -> int
        {
            int n = 0;
            for (int i = 0; i < _size; ++i) {
                n += (_types[i] != IndexType::PROJECTED and _data[i] == c);
            }
            return n;
        }

        constexpr auto _push_back(wchar_t c, IndexType type) -> int
        {
            _data[_size] = c;
            _types[_size] = type;
            return _size++;
        }
    };

    template <concepts::index... Is>
    TensorIndex(utils::type_args<Is...>) -> TensorIndex<sizeof...(Is)>;

    template <int A, int B>
    constexpr auto operator^(TensorIndex<A> const& a, TensorIndex<B> const& b)
    {
        return a;
    }

    template <int A, int B>
    constexpr auto is_permutation(TensorIndex<A> const& a, TensorIndex<B> const& b)
        -> bool
    {
        return true;
    }
}
