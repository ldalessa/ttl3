#pragma once

#include "ttl/index.hpp"
#include "ttl/concepts/index.hpp"
#include "ttl/concepts/tensor_index.hpp"
#include <algorithm>
#include <array>
#include <concepts>
#include <limits>
#include <optional>

namespace ttl
{

    static constexpr int IGNORE = 0;

    template <int M>
    requires (M >= 0)
    struct tensor_index
    {
        using tensor_index_concept_tag = void;

        wchar_t _data[M]{};
        int _types[M]{};
        int _size{};

        consteval tensor_index() = default;

        template <concepts::index_or_integral... Is>
        constexpr explicit tensor_index(Is&&... is)
        {
            static_assert((concepts::index<Is> || ...), "logic error in ttl");
            (_append(is), ...);
            _validate();
        }

        constexpr explicit tensor_index(
                concepts::tensor_index auto&& a,
                concepts::tensor_index auto&& b)
        {
            _append(a);
            _append(b);
            _validate();
        }

        struct const_iterator {
            tensor_index const* _index{nullptr};
            int _i{0};

            constexpr auto operator*() const {
                struct _value_type {
                    wchar_t c;
                    int t;
                };
                return _value_type {
                    .c = _index->_data[_i],
                    .t = _index->_types[_i]
                };
            }

            constexpr auto operator++() -> const_iterator& {
                ++_i;
                return *this;
            }

            constexpr auto operator++(int) -> const_iterator {
                auto out = *this;
                ++_i;
                return out;
            }

            constexpr auto operator-(const_iterator const& b) const -> int {
                return _i - b._i;
            }

            constexpr friend bool operator==(const_iterator const&, const_iterator const&) = default;
            constexpr friend auto operator<=>(const_iterator const&, const_iterator const&) = default;
        };

        static constexpr auto capacity() -> int {
            return M;
        }

        constexpr auto size() const -> int {
            return _size;
        }

        constexpr auto order() const -> int {
            return unique().size();
        }

        constexpr auto begin() const -> const_iterator {
            return {
                ._index = this,
                ._i = 0
            };
        }

        constexpr auto end() const -> const_iterator {
            return {
                ._index = this,
                ._i = _size
            };
        }

        constexpr auto data(int i) const -> wchar_t {
            return _data[i];
        }

        constexpr auto push_back(wchar_t c, int t = IGNORE) -> tensor_index& {
            _data[_size] = c;
            _types[_size] = t;
            ++_size;
            return *this;
        }

        constexpr auto unique() const -> tensor_index {
            tensor_index out;
            for (auto [c, t] : *this) {
                if (t != IGNORE and count(c) == 1) {
                    out.push_back(c, t);
                }
            }
            return out;
        }

        constexpr auto n_unique() const -> int {
            int n = 0;
            for (auto [c, t] : *this) {
                n += (t != IGNORE and count(c) == 1);
            }
            return n;
        }

        constexpr auto contracted() const -> tensor_index {
            tensor_index out;
            for (auto [c, t] : *this) {
                if (t != IGNORE and count(c) == 2 and out.count(c) == 0) {
                    out.push_back(c, IGNORE);
                }
            }
            return out;
        }

        constexpr auto n_contracted() const -> int {
            int n = 0;
            for (auto [c, t] : *this) {
                n += (t != IGNORE and count(c) == 2);
            }
            return n/2;
        }

        constexpr auto n_projected() const -> int {
            int n = 0;
            for (auto&& [_, t] : *this) {
                n += (t == IGNORE);
            }
            return n;
        }

        constexpr auto count(wchar_t c) const -> int {
            int sum = 0;
            for (auto&& [d, _] : *this) {
                sum += (c == d);
            }
            return sum;
        }

        constexpr auto index_of(wchar_t c, int n = 1) const -> std::optional<int> {
            for (int i = 0; i < _size; ++i) {
                if (_data[i] == c) {
                    if (--n == 0) {
                        return i;
                    }
                }
            }
            return std::nullopt;
        }

        constexpr auto _append(std::integral auto& b) -> tensor_index& {
            push_back(0, IGNORE);
            return *this;
        }

        template <wchar_t c, int t>
        constexpr auto _append(index<c, t>) -> tensor_index& {
            push_back(c, t);
            return *this;
        }

        constexpr auto _append(concepts::tensor_index auto&& b) -> tensor_index& {
            for (auto&& [c, t] : b) {
                push_back(c, t);
            }
            return *this;
        }

        constexpr auto _validate() const {
            for (int i = 0; i < _size; ++i) {
                int k = 0;
                for (int j = i + 1; j < _size; ++j) {
                    if (_data[i] == _data[j]) {
                        if (k++) {
                            throw "index appears more than once";
                        }

                        if (_types[i] + _types[j] == 2) {
                            throw "index appears twice as covariant";
                        }

                        if (_types[i] + _types[j] == -2) {
                            throw "index appears twice as contravariant";
                        }
                    }
                }
            }
        }
    };

    // Single CTAD for inferring the index capacity for all constructors.
    tensor_index(concepts::index_or_integral auto... is)
        -> tensor_index<sizeof...(is)>;

    template <concepts::tensor_index A, concepts::tensor_index B>
    tensor_index(A, B) -> tensor_index<A::capacity() + B::capacity()>;

    inline constexpr auto operator==(
            concepts::tensor_index auto&& a,
            concepts::tensor_index auto&& b)
        -> bool
    {
        return std::equal(a.begin(), a.end(), b.begin(), b.end());
    }

    inline constexpr auto operator<=>(
            concepts::tensor_index auto&& a,
            concepts::tensor_index auto&& b)
    {
        return std::lexicographical_compare_three_way(
            a.begin(),
            a.end(),
            b.begin(),
            b.end());
    }

    inline constexpr auto operator~(concepts::tensor_index auto i)
        -> concepts::tensor_index auto
    {
        for (auto& t : i._types) {
            t = 1 - t;
        }
        return i;
    }

    inline constexpr auto operator+(
            concepts::tensor_index auto&& a,
            concepts::tensor_index auto&& b)
    {
        return tensor_index { FWD(a), FWD(b) };
    }

    inline constexpr auto operator^(
            concepts::tensor_index auto&& a,
            concepts::tensor_index auto&& b)
    {
        return (FWD(a) + FWD(b)).unique();
    }

    template <int A, int B>
    inline constexpr auto operator&(
            tensor_index<A> const& a,
            tensor_index<B> const& b)
    {
        if constexpr (A <= B) {
            tensor_index aa = a.unique(), out;
            tensor_index bb = b.unique();
            for (auto&& [c, _] : aa) {
                if (bb.count(c) == 2) {
                    out.push_back(c);
                }
            }
            return out;
        }
        else {
            return b & a;
        }
    }

    template <concepts::tensor_index auto index>
    constexpr auto unique_v = [] {
        tensor_index<index.n_unique()> out;
        for (auto&& [c, t] : index) {
            if (t != IGNORE and index.count(c) == 1) {
                out.push_back(c, t);
            }
        }
        return out;
    }();

    template <concepts::tensor_index auto index>
    constexpr auto contracted_v = [] {
        tensor_index<index.n_contracted()> out;
        for (auto&& [c, t] : index) {
            if (t != IGNORE and index.count(c) == 2 and out.count(c) == 0) {
                out.push_back(c, IGNORE);
            }
        }
        return out;
    }();

    template <concepts::tensor_index auto a, concepts::tensor_index auto b>
    constexpr auto concat_v = a + b;

    /// Creates a map of the index of each index in from.
    ///
    /// The resulting map contains the target index for each of the source
    /// indices.
    template <concepts::tensor_index auto from, concepts::tensor_index auto to>
    constexpr auto index_to_index_map_v = [] {
        std::array<int, from.size()> out;
        for (int i = 0; auto&& [c, t] : from) {
            out[i++] = *to.index_of(c);
        }
        return out;
    }();
}
