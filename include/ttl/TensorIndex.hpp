#pragma once

#include "ttl/Index.hpp"
#include "ttl/concepts/index.hpp"
#include "ttl/concepts/tensor_index.hpp"
#include "ttl/utils/error.hpp"
#include "ttl/utils/expect.hpp"
#include "ttl/utils/ops.hpp"
#include "ttl/utils/type_args.hpp"
#include <array>

namespace ttl
{
    template <int M>
    struct TensorIndex
    {
        using tensor_index_tag = void;

        struct _entry_t
        {
            wchar_t   c;
            IndexType t;

            constexpr _entry_t() = default;

            constexpr _entry_t(wchar_t cʹ, IndexType tʹ)
                    : c { cʹ }
                    , t { tʹ }
            {}

            template <concepts::index I>
            constexpr _entry_t(utils::type_args<I>)
                    : c { index_to_wchar(I{}) }
                    , t { index_to_type(I{}) }
            {}

            constexpr friend auto operator==(_entry_t a, _entry_t b) -> bool
            {
                if (a.t == IndexType::PROJECTED) return false;
                if (b.t == IndexType::PROJECTED) return false;
                return (a.c == b.c and a.t == b.t);
            }
        };

        std::array<_entry_t, M> _data{};
        int _size = 0;

        consteval TensorIndex() = default;

        template <concepts::index... Is>
        consteval TensorIndex(utils::type_args<Is...>)
                : _data { _entry_t(utils::types<Is>)... }
                , _size { sizeof...(Is) }
        {
            _verify();
        }

        static consteval auto capacity() -> int {
            return M;
        }

        /// Return the size of the index (may be smaller than the capacity).
        constexpr auto size() const -> int {
            return _size;
        }

        /// Return the uncontracted indices.
        constexpr auto outer() const -> TensorIndex
        {
            TensorIndex out{};
            for (auto&& [c, t] : *this) {
                if (_count(c) == 1) {
                    out._push_back(c, t);
                }
            }
            return out;
        }

        /// Return the contracted indices.
        constexpr auto contracted() const -> TensorIndex
        {
            TensorIndex out{};
            for (auto&& [c, t] : *this) {
                if (_count(c) == 2 and out._count(c) == 0) {
                    out._push_back(c, IndexType::ANY);
                }
            }
            return out;
        }

        /// Return the projected indices.
        constexpr auto projected() const -> TensorIndex
        {
            TensorIndex out{};
            for (auto&& [c, t] : *this) {
                if (t == IndexType::PROJECTED) {
                    out._push_back(c, t);
                }
            }
            return out;
        }

        /// An index's order is the number of indices that are uncontracted.
        constexpr auto order() const -> int {
            return outer().size();
        }

        constexpr auto begin() const {
            return _data.begin();
        }

        constexpr auto end() const {
            return begin() + size();
        };

        /// Count the number of occurrences of the character.
        constexpr auto _count(wchar_t c) const -> int
        {
            int n = 0;
            for (auto&& [cʹ, tʹ] : *this) {
                n += (tʹ != IndexType::PROJECTED and cʹ == c);
            }
            return n;
        }

        /// Count the number of occurrences of the c/t pair.
        constexpr auto _count(wchar_t c, IndexType t) const -> int
        {
            int n = 0;
            for (auto&& [cʹ, tʹ] : *this) {
                n += (cʹ == c and tʹ == t);
            }
            return n;
        }

        /// Insert a c,t pair.
        constexpr auto _push_back(wchar_t c, IndexType t) -> void
        {
            expect(_size < M);
            _data[_size++] = _entry_t { c, t };
        }

        /// Verify that an index makes sense, semantically.
        constexpr auto _verify() const -> void
        {
            // A character shouldn't appear more than twice.
            for (auto&& [c, _] : *this) {
                if (_count(c) > 2) {
                    utils::error("index appears more than two times");
                }
            }

            // If an index appears twice, it needs to match types.
            for (int i = 0; i < _size; ++i) {
                for (int j = i + 1; j < _size; ++j) {
                    if (_data[i] == _data[j]) {
                        utils::error("index appears as co (or contra) variant twice");
                    }
                }
            }
        }
    };

    template <concepts::index... Is>
    TensorIndex(utils::type_args<Is...>) -> TensorIndex<sizeof...(Is)>;

    inline constexpr auto operator==(
            concepts::tensor_index auto const& a,
            concepts::tensor_index auto const& b)
        -> bool
    {
        return a.size() == b.size() and std::equal(a.begin(), a.end(), b.begin());
    }

    /// Compute the disjoint union of the indices.
    inline constexpr auto operator^(
            concepts::tensor_index auto const& a,
            concepts::tensor_index auto const& b)
    {
        constexpr int M = a.capacity() + b.capacity();
        TensorIndex<M> _union;
        for (auto&& [c, t] : a) _union._push_back(c, t);
        for (auto&& [c, t] : b) _union._push_back(c, t);
        _union._verify();
        return _union.outer();
    }

    /// Determine if two indices are permutations.
    inline constexpr auto is_permutation(
            concepts::tensor_index auto const& a,
            concepts::tensor_index auto const& b)
        -> bool
    {
        bool permutation = true;
        for (auto&& [c, t] : a) permutation &= (b._count(c, t) == 1);
        for (auto&& [c, t] : b) permutation &= (a._count(c, t) == 1);
        return permutation;
    }

    // template <int A>
    // inline constexpr auto join(
    //         TensorIndex<A> a,
    //         concepts::tensor_index auto const&... bs)
    // {
    //     ([&] {
    //         for (auto&& [c, t] : bs) {
    //             a._push_back(c, t);
    //         }
    //     }(), ...);
    //     return a;
    // }
}
