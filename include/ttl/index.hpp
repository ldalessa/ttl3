#pragma once

#include "ttl/concepts/index.hpp"
#include <algorithm>
#include <array>
#include <concepts>
#include <limits>
#include <optional>

namespace ttl
{
    template <int M>
    requires (M >= 0)
    struct index
    {
        using index_concept = void;

        static constexpr int COV = 0;
        static constexpr int CNV = 1;
        static constexpr int IGNORE = std::numeric_limits<int>::max();

        wchar_t _data[M]{};
        int _types[M]{};
        int _size{};

        consteval index() = default;

        /// This constructor is designed for use by the end-user.
        consteval index(std::same_as<wchar_t> auto is)
            : _data { is }
            , _types { COV }
            , _size { 1 }
        {
        }

        /// This constructor is used for all other construction.
        template <concepts::index_or_integral... Is>
        requires (concepts::index<Is> || ...)
        constexpr explicit index(Is&&... is)
        {
            (_append(is), ...);
            _validate();
        }

        struct const_iterator {
            index const* _index{nullptr};
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

        static consteval auto capacity() -> int {
            return M;
        }

        constexpr auto size() const -> int {
            return _size;
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

        constexpr auto push_back(wchar_t c, int t = IGNORE) -> index& {
            _data[_size] = c;
            _types[_size] = t;
            ++_size;
            return *this;
        }

        constexpr auto unique() const -> index {
            index out;
            for (auto [c, t] : *this) {
                if (not index_of(c, 2)) {
                    out.push_back(c, t);
                }
            }
            return out;
        }

        constexpr auto index_of(wchar_t c, int n = 1) const -> std::optional<int> {
            for (int i = 0; i < _size; ++i) {
                if (_data[i] == c and _types[i] != IGNORE) {
                    if (--n == 0) {
                        return i;
                    }
                }
            }
            return std::nullopt;
        }

        constexpr auto _append(std::integral auto& b) -> index& {
            push_back(0, IGNORE);
            return *this;
        }

        constexpr auto _append(concepts::index auto& b) -> index& {
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

                        if (_types[i] + _types[j] == 2 * COV) {
                            throw "index appears twice as covariant";
                        }

                        if (_types[i] + _types[j] == 2 * CNV) {
                            throw "index appears twice as contravariant";
                        }
                    }
                }
            }
        }
    };

    // Single CTAD for inferring the index capacity for all constructors.
    template <concepts::index_or_integral... Is>
    index(Is... is) -> index<(((std::integral<Is>) ?: Is::capacity()) + ... + 0)>;

    inline constexpr auto operator==(concepts::index auto const& a, concepts::index auto const& b)
        -> bool
    {
        return std::equal(a.begin(), a.end(), b.begin(), b.end());
    }

    inline constexpr auto operator<=>(concepts::index auto const& a, concepts::index auto const& b)
    {
        return std::lexicographical_compare_three_way(
            a.begin(),
            a.end(),
            b.begin(),
            b.end());
    }

    inline constexpr auto operator~(concepts::index auto i) -> concepts::index auto
    {
        for (auto& t : i._types) {
            t = 1 - t;
        }
        return i;
    }

    inline constexpr auto operator+(concepts::index auto const& a, concepts::index auto const& b)
    {
        return index { a, b };
    }

    inline constexpr auto operator^(concepts::index auto const& a, concepts::index auto const& b)
    {
        return (a + b).unique();
    }

    template <int A, int B>
    inline constexpr auto operator&(index<A> const& a, index<B> const& b)
    {
        if constexpr (A <= B) {
            index aa = a.unique(), out;
            index bb = b.unique();
            for (auto [c, _] : aa) {
                if (bb.index_of(c, 2)) {
                    out.push_back(c);
                }
            }
            return out;
        }
        else {
            return b & a;
        }
    }
}
