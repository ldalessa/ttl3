#pragma once

#include "ttl/Index.hpp"
#include "ttl/concepts/index.hpp"
#include <array>
#include <concepts>

namespace ttl
{
    /// A scalar index represents a runtime tensor index.
    ///
    /// Scalar indices are stored "big endian" in the sense that the outermost
    /// index is at offset 0.
    template <int _order>
    struct ScalarIndex
    {
        std::array<int, _order> _data;

        constexpr ScalarIndex(concepts::index auto... is)
                : _data { index_to_value(is)... }
        {
            static_assert(sizeof...(is) <= _order);
        }

        static consteval auto size() -> int
        {
            return _order;
        }

        constexpr auto operator[](std::integral auto i) const -> int const&
        {
            return _data[i];
        }

        constexpr auto operator[](std::integral auto i) -> int&
        {
            return _data[i];
        }
    };

    /// Infer the ScalarIndex order for this constructor.
    ScalarIndex(concepts::index auto... is) -> ScalarIndex<sizeof...(is)>;
}
