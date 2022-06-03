#pragma once

#include "ttl/Index.hpp"
#include "ttl/concepts/index.hpp"
#include "ttl/concepts/non_scalar.hpp"
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

        static constexpr auto capacity() -> int {
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
        }

        constexpr auto operator[](int i) const -> _entry_t const& {
            return _data[i];
        }

        constexpr auto nth_index_of(wchar_t c, int n) const -> int
        {
            expect(_count(c) >= n);
            for (int i = 0; i < _size; ++i) {
                if (_data[i].c == c and --n == 0) {
                    return i;
                }
            }
            __builtin_unreachable();
        }

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

    /// Concatenate two indices.
    inline constexpr auto operator+(
            concepts::tensor_index auto const& a,
            concepts::tensor_index auto const& b)
    {
        constexpr int M = a.capacity() + b.capacity();
        TensorIndex<M> out{};
        for (auto&& [c, t] : a) out._push_back(c, t);
        for (auto&& [c, t] : b) out._push_back(c, t);
        out._verify();
        return out;
    }

    /// Compute the disjoint union of the indices.
    inline constexpr auto operator^(
            concepts::tensor_index auto const& a,
            concepts::tensor_index auto const& b)
    {
        return (a + b).outer();
    }

    inline constexpr auto operator&(
            concepts::tensor_index auto const& a,
            concepts::tensor_index auto const& b)
    {
        return (a + b).contracted();
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

    /// Join a sequence of indices.
    ///
    /// This is distinct from concatenate (operator+) because join requires that
    /// the joined index fit into an index with the capacity of `a`. This occurs
    /// in some expressions where we have contractions.
    inline constexpr auto join(
            concepts::tensor_index auto a,
            concepts::tensor_index auto&&... b)
        -> concepts::tensor_index auto
    {
        expect((a.size() + ... + b.size()) <= a.capacity());
        ([&] {
            for (auto&& [c, t] : b) {
            a._push_back(c, t);
            }
        }(), ...);
        return a;
    }

    /// Use two tensor indices to map between extents.
    ///
    /// It is the case that our tensors have extents for each of their indices,
    /// but during various operations (like contractions and/or shuffles) these
    /// indices can become shuffled.
    ///
    /// For instance, when evaluating the following example:
    ///
    ///     Matrix A, B, C;
    ///     Index i,j,k;
    ///     C(i,j) = A(i,~k) * B(k,j)
    ///
    /// The Bind resulting from the right-hand-side has an outer index of `i,j`
    /// and a contracted index of k. When evaluating the contraction we need to
    /// know the bounds for `k`.
    ///
    ///     auto contract(int i, int j) {
    ///         scalar d = 0
    ///         for (int k = 0; k < WHAT_GOES_HERE; ++k) {
    ///             d += A(i,k) + B(k,j);
    ///         }
    ///     }
    ///
    /// Ultimately, to figure out "WHAT_GOES_HERE" we need to take the
    /// contracted index (i,j,k) and ask A what the extents for k are (and
    /// possibly also ask B for debugging purposed, since they need to be
    /// equivalent).
    ///
    /// This template allows that.
    ///
    ///     Matrix A(8, 16), B(16, 4);
    ///     assert(ttl::extent<0>(A) == 8);
    ///     assert(ttl::extent<1>(A) == 16);
    ///     map_extents<{i,j,k}, {i,k}, decltype(A)> mapper(A);
    ///     assert(ttl::extent<0>(mapper) == 8);
    ///     assert(ttl::extent<2>(mapper) == 16);
    ///
    ///     ttl::extent<1>(mapper); // NO MAPPING FOR `j`, FAILS TO COMPILE
    template <
        concepts::tensor_index auto a,
        concepts::tensor_index auto b,
        concepts::non_scalar T>
    struct map_extents
    {
        T&& t;

        template <int i>
        constexpr auto extent() const -> int {
            constexpr int j = b.nth_index_of(a[i].c, 1);
            return ttl::extent<j>(t);
        }
    };

    template <
        concepts::tensor_index auto a,
        concepts::tensor_index auto b,
        concepts::non_scalar T>
    inline constexpr auto make_map_extents(T&& t) -> map_extents<a, b, T>
    {
        return map_extents<a, b, T> {
            .t = t
        };
    }
}
