#pragma once

#include "ttl/traits/extent.hpp"
#include "ttl/utils/FWD.hpp"
#include <concepts>

namespace ttl
{
    namespace cpos
    {
        // These concepts help us prefer the extent trait when a type also
        // provides a member extent().
        namespace _concepts
        {
            template <int N, class T>
            concept has_trait_extent = requires {
                { traits::extent_v<N, T> } -> std::convertible_to<int>;
            };

            template <int N, class T>
            concept has_member_extent = requires (T t) {
                { t.template extent<N>()} -> std::convertible_to<int>;
            };

            template <int N, class T>
            concept static_extent = has_trait_extent<N, T>;

            template <int N, class T>
            concept dynamic_extent = not static_extent<N, T> and has_member_extent<N, T>;
        }

        template <int N>
        struct extent
        {
            template <class T> requires(_concepts::static_extent<N, T>)
            constexpr auto operator()(T&&) const -> int
            {
                return traits::extent_v<N, T>;
            }

            template <class T> requires(_concepts::dynamic_extent<N, T>)
            constexpr auto operator()(T&& obj) const -> int
            {
                return FWD(obj).template extent<N>();
            }

            consteval extent(int) {}
        };
    }

    template <int N>
    inline constexpr cpos::extent<N> extent{0};
}
