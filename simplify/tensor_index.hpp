#pragma once

#include "array.hpp"
#include "index.hpp"
#include <concepts>

namespace ttl
{
    template <class T>
    concept is_tensor_index = requires {
        typename std::remove_cvref_t<T>::is_tensor_index_tag;
    };

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

        constexpr auto count(wchar_t c) const -> int
        {
            int n = 0;
            for (auto [cʹ, _] : *this) {
                n += (c == cʹ);
            }
            return n;
        }

        constexpr auto find(wchar_t c, int t, int n = 1) const -> int
        {
            for (int i = 0; auto [cʹ, tʹ] : *this) {
                if (c == cʹ and t == tʹ and --n == 0) {
                    return i;
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

        constexpr auto is_subset_of(is_tensor_index auto&& b) const -> bool
        {
            if (b.n_projected() < n_projected()) {
                return false;
            }

            for (auto [c, t] : *this) {
                if (find(c, t) == _size) {
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

        constexpr auto gather_from(is_tensor_index auto&& b) const {
            if (not is_subset_of(b)) {
                throw "must gather from a subset";
            }
            array<int, _size> out;
            for (int i = 0, n = 0; auto [c, t] : *this) {
                if (t == PROJECTED) {
                    out[i++] = b.find(c, t, ++n);
                }
                else {
                    out[i++] = b.find(c, t);
                }
            }
            return out;
        }

        constexpr auto validate() const -> void
        {
            for (int i = 0; i < _size; ++i) {
                if (auto [c, t] = (*this)[i]; t != PROJECTED) {
                    for (int n = 0, j = i + 1; j < _size; ++j) {
                        if (auto [cʹ, tʹ] = (*this)[j]; c == cʹ) {
                            if (n++ or t + tʹ != CONTRACTED) {
                                throw "invalid index construction";
                            }
                        }
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
        for (int i = 0; auto [c, t] : a) {
            if (t != PROJECTED and a.count(c) == 2 and out.count(c) == 0) {
                out[i++] = _index(c, CONTRACTED);
            }
        }
        out.validate();
        return out;
    }();

    template <is_tensor_index auto a, is_tensor_index auto b>
    constexpr tensor_index concat(a, b);
}
