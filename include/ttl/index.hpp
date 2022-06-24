#pragma once

namespace ttl
{
    constexpr int COVARIANT = 1;
    constexpr int CONTRAVARIANT = -1;

    template <wchar_t c, int type = COVARIANT>
    struct index
    {
        using index_concept_tag = void;
    };

    template <wchar_t c>
    constexpr auto operator~(index<c, COVARIANT>) -> index<c, CONTRAVARIANT> {
        return {};
    }

    template <wchar_t c>
    constexpr auto operator~(index<c, CONTRAVARIANT>) -> index<c, COVARIANT> {
        return {};
    }
}
