#pragma once

#include <concepts>

namespace ttl
{
    enum class IndexType : int {
        INVALID       = 0,
        COVARIANT     = 1,
        CONTRAVARIANT = 2,
        PROJECTED     = 4
    };

    consteval auto matches(IndexType a, IndexType b) {
        return
        (a == IndexType::COVARIANT and b == IndexType::CONTRAVARIANT) or
        (a == IndexType::CONTRAVARIANT and b == IndexType::COVARIANT);
    }

    template <wchar_t...>
    struct CovariantIndex
    {
        using _index_tag = void;
    };

    template <wchar_t...>
    struct ContravariantIndex
    {
        using _index_tag = void;
    };

    template <auto is>
    inline constexpr CovariantIndex<is> index;

    template <wchar_t... is>
    static consteval auto operator~(CovariantIndex<is...>) -> ContravariantIndex<is...> {
        return {};
    }

    template <wchar_t... is>
    static consteval auto operator~(ContravariantIndex<is...>) -> CovariantIndex<is...> {
        return {};
    }

    struct _index_to_type_fn
    {
        template <wchar_t... is>
        consteval auto operator()(CovariantIndex<is...>) const -> IndexType
        {
            return IndexType::COVARIANT;
        }

        template <wchar_t... is>
        consteval auto operator()(ContravariantIndex<is...>) const -> IndexType
        {
            return IndexType::CONTRAVARIANT;
        }

        consteval auto operator()(std::integral auto) const -> IndexType
        {
            return IndexType::PROJECTED;
        }

        consteval _index_to_type_fn(int) {}
    };

    inline constexpr _index_to_type_fn index_to_type{0};

    struct _index_to_wchar_fn
    {
        template <wchar_t is>
        consteval auto operator()(CovariantIndex<is>) const -> wchar_t
        {
            return is;
        }

        template <wchar_t is>
        consteval auto operator()(ContravariantIndex<is>) const -> wchar_t
        {
            return is;
        }

        consteval auto operator()(std::integral auto) const -> wchar_t
        {
            return '\0';
        }

        consteval _index_to_wchar_fn(int) {}
    };

    inline constexpr _index_to_wchar_fn index_to_wchar{0};

    struct _index_to_value_fn
    {
        template <wchar_t is>
        consteval auto operator()(CovariantIndex<is>) const -> int
        {
            return -1;
        }

        template <wchar_t is>
        consteval auto operator()(ContravariantIndex<is>) const -> int
        {
            return -1;
        }

        constexpr auto operator()(std::convertible_to<int> auto i) const -> int
        {
            return i;
        }

        consteval _index_to_value_fn(int) {}
    };

    inline constexpr _index_to_value_fn index_to_value{0};
}
