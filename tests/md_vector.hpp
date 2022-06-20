#pragma once

#include "ttl/traits/extents.hpp"
#include "md_base.hpp"
#include <array>
#include <numeric>
#include <vector>

namespace ttl::tests
{
    template <class T, int _order>
    struct md_vector : md_base<md_vector<T, _order>>
    {
        std::array<int, _order> _shape;
        std::array<int, _order> _strides{};
        std::vector<T> _data{};

        constexpr explicit md_vector(std::integral auto... extents)
                : _shape { extents... }      // clang doesn't support ()
                , _data((extents * ... * 1)) // {} means the wrong thing
        {
            static_assert(sizeof...(extents) == _order);
            std::exclusive_scan(_shape.begin(), _shape.end(), _strides.begin(), 1, std::multiplies{});
            std::reverse(_strides.begin(), _strides.end());
        }

        static consteval auto order() -> int {
            return _order;
        }

        constexpr auto size() const -> decltype(auto) {
            return _data.size();
        }
    };
}