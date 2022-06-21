#pragma once

#include <ranges>

namespace ttl::tests
{
    template <class T>
    struct md_base
    {
        constexpr auto _derived() const & -> T const&
        {
            return *(static_cast<T const*>(this));
        }

        constexpr auto _derived() && -> T&&
        {
            return std::move(*(static_cast<T*>(this)));
        }

        constexpr auto _derived() & -> T&
        {
            return *(static_cast<T*>(this));
        }

        /// Linear iteration.
        constexpr auto begin() const {
            return std::ranges::begin(_derived()._data);
        }

        /// Linear iteration.
        constexpr auto end() const {
            return std::ranges::end(_derived()._data);
        }

        /// Multi-dimensional access.
        constexpr auto operator()(std::integral auto... is) const & -> auto&
        {
            return _at(_derived(), is...);
        }

        /// Multi-dimensional access.
        constexpr auto operator()(std::integral auto... is) && -> auto&
        {
            return _at(_derived(), is...);
        }

        /// Multi-dimensional access.
        constexpr auto operator()(std::integral auto... is) & -> auto&
        {
            return _at(_derived(), is...);
        }

        // Linear access.
        constexpr auto operator[](std::integral auto i) const& -> auto& {
            return _derived()._data[i];
        }

        // Linear access.
        constexpr auto operator[](std::integral auto i) && -> auto& {
            return _derived()._data[i];
        }

        // Linear access.
        constexpr auto operator[](std::integral auto i) & -> auto& {
            return _derived()._data[i];
        }

        /// Multidimensional access (auto& return handles const)
        static constexpr auto _at(auto&& self, std::integral auto... is) -> auto&
        {
            return FWD(self)._data[_row_major(self, is...)];
        }

        /// Compute a row-major offset for a multidimensional index.
        static constexpr auto _row_major(auto&& self, std::integral auto... is) -> std::integral auto
        {
            static_assert(sizeof...(is) == T::order());
            decltype(self.size()) offset = 0;
            int i = 0;
            ((offset += self._strides[i++] * is), ...);
            return offset;
        }
    };
}
