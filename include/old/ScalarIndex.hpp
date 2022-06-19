#pragma once

#include "ttl/Index.hpp"
#include "ttl/concepts/index.hpp"
#include "ttl/concepts/tensor_index.hpp"
#include "ttl/cpos/extent.hpp"
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
    inline constexpr auto select(ScalarIndex<from.size()> const &in)
        -> ScalarIndex<to.size()>
    {
        ScalarIndex<to.size()> out{};

        for (int i = 0; i < out.size(); ++i) {
            auto&& [c, t] = to[i];
            expect(t != IndexType::PROJECTED);
            int j = from.nth_index_of(c, 1);
            out[i] = in[j];
        }
        return out;
    }

    template <
        concepts::tensor_index auto from,
        concepts::tensor_index auto to>
    inline constexpr auto select(
            ScalarIndex<from.size()> const &in,
            ScalarIndex<to.size()> const& projected)
        -> ScalarIndex<to.size()>
    {
        ScalarIndex<to.size()> out = projected;

        for (int i = 0; i < out.size(); ++i) {
            auto&& [c, t] = to[i];
            if (t == IndexType::PROJECTED) {
                continue;
            }

            int j = from.nth_index_of(c, 1);
            out[i] = in[j];
        }
        return out;
    }

    template <int prefix>
    struct _carry_sum_inc_fn
    {
        template <int all>
        constexpr auto operator()(ScalarIndex<all>& index, auto&& extents) const -> bool
            requires(all > prefix)
        {
            return [&]<std::size_t... is>(std::index_sequence<is...>) {
                return ([&] {
                    constexpr int i = prefix + is;
                    return (++index[i] < ttl::extent<i>(extents)) ? true : (index[i] = 0, false);
                }() || ...);
            }(std::make_index_sequence<all - prefix>());
        }

        consteval _carry_sum_inc_fn(int) {}
    };

    template <int prefix>
    inline constexpr _carry_sum_inc_fn<prefix> carry_sum_inc{0};
}
