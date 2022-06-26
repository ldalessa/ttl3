#pragma once

#include <concepts>

namespace ttl
{
    constexpr int PROJECTED = 0;
    constexpr int COVARIANT = 1;
    constexpr int CONTRAVARIANT = 2;
    constexpr int CONTRACTED = 3;

    template <class T>
    concept is_index = requires {
        typename std::remove_cvref_t<T>::is_index_tag;
    };

    template <class T>
    concept is_index_or_int = is_index<T> or std::same_as<std::remove_cvref_t<T>, int>;

    template <wchar_t _c, int _type = COVARIANT>
    struct index
    {
        using is_index_tag = void;

        constexpr auto operator~() const -> index<_c, CONTRAVARIANT> requires (_type == COVARIANT) {
            return {};
        }

        constexpr auto operator~() const -> index<_c, COVARIANT> requires (_type == CONTRAVARIANT) {
            return {};
        }
    };

    namespace _detail {
        struct _to_int_fn {
            constexpr auto operator()(is_index auto) const -> int {
                return -1;
            }

            constexpr auto operator()(std::same_as<int> auto i) const -> int {
                return i;
            }
        };
    }

    constexpr _detail::_to_int_fn to_int{};
}
