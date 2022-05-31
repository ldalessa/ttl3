#pragma once

#include "ttl/Index.hpp"
#include "ttl/concepts/index.hpp"
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
        int _data[_order]{};

        constexpr ScalarIndex(concepts::index auto... is)
                : _data { index_to_value(is)... }
        {
            static_assert(sizeof...(is) <= _order);
        }

        static consteval auto size() -> int
        {
            return _order;
        }

        constexpr auto begin() const -> int const*
        {
            return _data;
        }

        constexpr auto end() const -> int const*
        {
            return _data + _order;
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

    // /// Non-friend operator+ because it reduces the number of instances that the
    // /// compiler needs to generate.
    // template <int A, int B>
    // constexpr auto concat(ScalarIndex<A> const& a, ScalarIndex<B> const& b)
    //     -> ScalarIndex<A + B>
    // {
    //     ScalarIndex<A + B> c;
    //     int i = 0;
    //     for (int a : a) c[i++] = a;
    //     for (int b : b) c[i++] = b;
    //     return c;
    // }
}
