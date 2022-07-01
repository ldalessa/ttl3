#pragma once

#include "ttl/array.hpp"
#include "ttl/index.hpp"
#include "ttl/utils.hpp"
#include <concepts>

namespace ttl
{
    template <class T>
    concept is_tensor_index = has_static_size<T> and requires {
        typename std::remove_cvref_t<T>::is_tensor_index_tag;
    };

    template <class T, int _size>
    concept is_tensor_index_of_size = is_tensor_index<T> and size<T> == _size;

    struct _index {
        wchar_t c{-1};
        int t{PROJECTED};

        consteval _index() = default;

        template <wchar_t cʹ, int tʹ>
        constexpr explicit _index(ttl::index<cʹ, tʹ>) : c(cʹ), t(tʹ) {}

        constexpr explicit _index(std::same_as<int> auto) {}

        constexpr explicit _index(wchar_t cʹ, int tʹ) : c(cʹ), t(tʹ) {}

        constexpr bool operator==(_index const&) const = default;
        constexpr auto operator<=>(_index const&) const = default;
    };

    template <int _size>
    struct tensor_index : array<_index, _size>
    {
        using is_tensor_index_tag = void;

        constexpr tensor_index() requires (0 < _size) = default;

        constexpr explicit tensor_index(is_index_or_int auto... is) requires (sizeof...(is) == _size)
            : array<_index, _size>{ _index(is)... }
        {
            validate();
        }

        constexpr explicit tensor_index(is_tensor_index auto&& a, is_tensor_index auto&& b)
        {
            int i = 0;
            for (auto index : a) (*this)[i++] = index;
            for (auto index : b) (*this)[i++] = index;
            if (i != _size) throw "unexpected tensor index initialization";
            validate();
        }

        constexpr bool operator==(tensor_index const&) const = default;
        constexpr auto operator<=>(tensor_index const&) const = default;

        constexpr auto count(wchar_t c, int e = _size) const -> int
        {
            int n = 0;
            for (int i = 0; i < e; ++i) {
                n += (c == (*this)[i].c);
            }
            return n;
        }

        constexpr auto _find(wchar_t c, int t, int n = 1) const -> int
        {
            for (int i = 0; auto [cʹ, tʹ] : *this) {
                if (c == cʹ and --n == 0) {
                // if (c == cʹ and ((t & tʹ) == t) and --n == 0) {
                    return i;
                }
                if ( n < 0) {
                    return _size;
                }
                i += 1;
            }
            return _size;
        }

        constexpr auto n_exported() const -> int
        {
            int n = 0;
            for (auto [c, t] : *this) {
                n += (t != PROJECTED) and count(c) == 1;
            }
            return n;
        }

        constexpr auto n_contracted() const -> int
        {
            int n = 0;
            for (auto [c, t] : *this) {
                n += (t != PROJECTED) and count(c) == 2;
            }
            return n / 2;
        }

        constexpr auto n_projected() const -> int
        {
            int n = 0;
            for (auto [_, t] : *this) {
                n += (t == PROJECTED);
            }
            return n;
        }

        constexpr auto has_synthetic() const -> bool {
            bool synthetic = false;
            for (auto [c, t] : *this) {
                synthetic |= (t & SYNTHETIC);
            }
            return synthetic;
        }

        constexpr auto is_subset_of(is_tensor_index auto&& b) const -> bool
        {
            for (int n = 1; auto [c, t] : *this) {
                if (t == PROJECTED) {
                    if (b._find(c, t, n++) == b.size()) {
                        return false;
                    }
                }
                else if (b._find(c, t) == b.size()) {
                    return false;
                }
            }

            return true;
        }

        constexpr auto is_permutation_of(is_tensor_index auto&& b) const -> bool {
            return this->is_subset_of(b) and b.is_subset_of(*this);
        }

        constexpr auto is_prefix_of(is_tensor_index auto&& b) const -> bool {
            if (_size > b.size()) return false;
            for (int i = 0; i < _size; ++i) {
                if ((*this)[i] != b[i]) return false;
            }
            return true;
        }

        constexpr auto map_extents_from(is_tensor_index auto b) const -> array<int, _size>
        {
            array<int, _size> map;
            for (int i = 0; i < _size; ++i) {
                auto [c, t] = (*this)[i];
                if (t == PROJECTED) throw "cannot map projected extents";
                int j = 0;
                for (; j < b.size(); ++j) {
                    auto [cʹ, tʹ] = b[j];
                    if (c != cʹ) continue;
                    if (t == CONTRACTED and tʹ & SYNTHETIC) continue;
                    if (t != CONTRACTED and t != tʹ) continue;
                    map[i] = j;
                    break;
                }
                if (j == b.size()) throw "extent not found";
            }
            return map;
        }

        ///
        constexpr auto gather_from(is_tensor_index auto&& b) const {
            // if (not is_subset_of(b)) {
            //     throw "must gather from a subset";
            // }

            array<int, _size> out;
            for (int i = 0, n = 1; auto [c, t] : *this) {
                if (t == PROJECTED) {
                    out[i++] = b._find(c, t, n++);
                }
                // else if (t == CONTRACTED) {
                //     throw "hmmm";
                // }
                else {
                    out[i++] = b._find(c, t);
                }
            }
            return out;
        }

        constexpr auto validate() const -> void
        {
            for (int i = 0; i < _size; ++i) {
                auto [c, t] = (*this)[i];
                if (t == SYNTHETIC) throw "SYNTHETIC indices must also be CO/CONTRAVARIANT";
                if (t == SYNTHETIC + CONTRACTED) throw "SYNTHETIC indices must not be CONTRACTED";
                if (t == PROJECTED) continue;

                for (int n = 0, j = i + 1; j < _size; ++j) {
                    auto [cʹ, tʹ] = (*this)[j];
                    if (c == cʹ) {
                        if (n++) throw "index appeared more than twice";
                        if ((t & tʹ) == SYNTHETIC) throw "contracted index cannot be synthetic";
                        if (((t & CONTRACTED) + (tʹ & CONTRACTED)) != CONTRACTED) throw "contraction must be between CO/CONTRAVARIANT indices";
                    }
                }
            }
        }
    };

    tensor_index() -> tensor_index<0>;
    tensor_index(is_index_or_int auto... is) -> tensor_index<sizeof...(is)>;

    template <is_tensor_index A, is_tensor_index B>
    tensor_index(A, B) -> tensor_index<A::size() + B::size()>;

    template <is_tensor_index auto a>
    constexpr tensor_index exported = [] {
        constexpr int size = a.n_exported();
        tensor_index<size> out;
        for (int i = 0; auto [c, t] : a) {
            if (t != PROJECTED and a.count(c) == 1) {
                out[i++] = _index(c, t);
            }
        }
        out.validate();
        return out;
    }();

    template <is_tensor_index auto a>
    constexpr tensor_index projected = [] {
        constexpr int size = a.n_projected();
        tensor_index<size> out;
        for (int i = 0; auto [c, t] : a) {
            if (t == PROJECTED) {
                out[i++] = _index(c, t);
            }
        }
        out.validate();
        return out;
    }();

    template <is_tensor_index auto a>
    constexpr tensor_index contracted = [] {
        constexpr int size = a.n_contracted();
        tensor_index<size> out;
        int i = 0;
        for (int j = 0; j < a.size(); ++j) {
            auto [c, t] = a[j];
            for (int k = 0; k < j; ++k) {
                auto [cʹ, tʹ] = a[k];
                if (c != cʹ) continue;
                out[i++] = _index(c, CONTRACTED);
                break;
            }
        }
        if (i != size) throw "failed to properly contract an index";
        out.validate();
        return out;
    }();

    template <is_tensor_index auto a, is_tensor_index auto b>
    constexpr bool is_permutation_of = a.is_permutation_of(b);

    template <is_tensor_index auto a, is_tensor_index auto b>
    constexpr tensor_index concat(a, b);

    /// Join two indices.
    ///
    /// Unlike `concat<>`, this does not validate the returned value.
    constexpr auto operator+(is_tensor_index auto a, is_tensor_index auto b) {
        constexpr int size = a.size() + b.size();
        tensor_index<size> out;
        int i = 0;
        for (auto&& c : a) out[i++] = c;
        for (auto&& c : b) out[i++] = c;
        if (i != out.size()) throw "error concatenating indices";
        // do not validate here
        return out;
    }
}
