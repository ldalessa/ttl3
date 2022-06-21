#pragma once

namespace ttl
{
    constexpr int COVARIANT = 0;
    constexpr int CONTRAVARIANT = 1;

    template <wchar_t c, int type = COVARIANT>
    struct index
    {
        using index_concept_tag = void;
    };

    template <wchar_t c, int type>
    constexpr auto operator~(index<c, type>) -> index<c, type - 1> {
        return {};
    }
}
