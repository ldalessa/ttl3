#pragma once

#include "ttl/Index.hpp"
#include "ttl/concepts/index.hpp"
#include "ttl/concepts/tensor_index.hpp"
#include "ttl/cpos/dim.hpp"
#include "ttl/utils/expect.hpp"
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
        std::array<int, _order> _data{};

        constexpr ScalarIndex(concepts::index auto... is)
                : _data { index_to_value(is)... }
        {
            static_assert(sizeof...(is) <= _order);
        }

        template <int B>
        constexpr ScalarIndex(ScalarIndex<B> const& b)
            requires(B <= _order)
        {
            std::copy(b._data.begin(), b._data.end(), _data.begin());
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

    template <
        concepts::tensor_index auto from,
        concepts::tensor_index auto to>
    inline constexpr auto select(
            ScalarIndex<from.size()> const &in,
            ScalarIndex<to.size()> const& projected)
        -> ScalarIndex<to.size()>
    {
        ScalarIndex<to.capacity()> out = projected;

        for (int i = 0; i < out.size(); ++i) {
            auto&& [c, t] = to[i];
            if (t == IndexType::PROJECTED) {
                continue;
            }

            int j = from.index_of(c);
            out[i] = in[j];
        }
        return out;
    }

    template <int prefix, int all>
    inline constexpr auto carry_sum_inc(ScalarIndex<all>& index, auto&& dims) -> bool
        requires(all > prefix)
    {
        return [&]<std::size_t... is>(std::index_sequence<is...>) {
            return ([&] {
                constexpr int i = prefix + is;
                return (++index[i] < dim<i>(dims)) ? true : (index[i] = 0, false);
            }() || ...);
        }(std::make_index_sequence<all - prefix>());
    }
}
