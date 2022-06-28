#pragma once

#include "ttl/ttl.hpp"
#include "md_vector.hpp"
#include <concepts>

namespace ttl::tests
{
    template <class T, int _order>
    struct dynamic_tensor : ttl::bindable<dynamic_tensor<T, _order>>
    {
        using scalar_type = T;

        md_vector<T, _order> _data{};

        constexpr dynamic_tensor() requires(_order != 0) = default;

        constexpr explicit dynamic_tensor(std::integral auto... extents)
            requires (sizeof...(extents) == _order)
                : _data { extents... }
        {
        }

        static consteval auto get_order() -> int {
            return _order;
        }

        constexpr auto get_extents() const -> decltype(auto) {
            return _data.shape();
        }

        constexpr auto size() const {
            return _data.size();
        }

        constexpr auto begin() const {
            return std::ranges::begin(_data);
        }

        constexpr auto begin() {
            return std::ranges::begin(_data);
        }

        constexpr auto end() const {
            return std::ranges::begin(_data);
        }

        constexpr auto end() {
            return std::ranges::end(_data);
        }

        constexpr auto operator[](int i) const -> auto& {
            assert(0 <= i and i < size());
            return _data[i];
        }

        constexpr auto operator[](int i) -> auto& {
            assert(0 <= i and i < size());
            return _data[i];
        }

        constexpr auto evaluate(ttl::scalar_index<_order> const& index) const -> decltype(auto) {
            return _evaluate(*this, index);
        }

        constexpr auto evaluate(ttl::scalar_index<_order> const& index) -> decltype(auto) {
            return _evaluate(*this, index);
        }

        static constexpr auto _evaluate(auto&& self, ttl::scalar_index<_order> const& index) -> decltype(auto)
        {
            return _evaluate(FWD(self), index, std::make_index_sequence<_order>{});
        }

        template <std::size_t... is>
        static constexpr auto _evaluate(auto&& self, ttl::scalar_index<_order> const& index, std::index_sequence<is...>)
            -> decltype(auto)
        {
            return FWD(self)._data(index[is]...);
        }
    };
}
