#pragma once

#include "ttl/array.hpp"
#include "ttl/scalar_index.hpp"
#include "ttl/utils.hpp"
#include <concepts>

namespace ttl
{
    template <class>
    struct tensor_traits;

    template <class T>
    using tensor_traits_t = tensor_traits<std::remove_cvref_t<T>>;

    template <class T>
    concept has_tensor_traits = requires {
        typename tensor_traits_t<T>::scalar_type;
        { tensor_traits_t<T>::get_order() } -> std::convertible_to<int>;
        requires requires (T t) {
            { tensor_traits_t<T>::get_extents(t) } -> is_int_array;
        };
        requires requires (T t, scalar_index<tensor_traits_t<T>::get_order()> const& i) {
            { tensor_traits_t<T>::evaluate(t, i) } -> std::convertible_to<typename tensor_traits_t<T>::scalar_type>;
        };
    };

    template <has_tensor_traits T>
    using scalar_type_t = typename tensor_traits_t<T>::scalar_type;

    template <has_tensor_traits T>
    constexpr int order = tensor_traits_t<T>::get_order();

    namespace _detail {
        struct _get_extents_fn {
            template <has_tensor_traits T>
            constexpr auto operator()(T&& obj) const -> decltype(auto) {
                return tensor_traits_t<T>::get_extents(FWD(obj));
            }
        };
    }
    constexpr _detail::_get_extents_fn extents{};

    namespace _detail {
        struct _evaluate_fn {
            template <has_tensor_traits T>
            constexpr auto operator()(T&& obj, is_scalar_index auto&& i) const -> decltype(auto) {
                return tensor_traits_t<T>::evaluate(FWD(obj), FWD(i));
            }
        };
    }
    constexpr _detail::_evaluate_fn evaluate{};

    template <class T>
    concept has_member_tensor_traits = requires {
        typename T::scalar_type;
        { T::get_order() } -> std::convertible_to<int>;
        requires requires (T t) {
            { t.get_extents() } -> is_int_array;
        };
        requires requires (T t, scalar_index<T::get_order()> const& i) {
            { t.evaluate(i) } -> std::convertible_to<typename T::scalar_type>;
        };
    };

    template <has_member_tensor_traits T>
    struct tensor_traits<T>
    {
        using scalar_type = typename T::scalar_type;

        static consteval auto get_order() -> int {
            return T::get_order();
        }

        static constexpr auto get_extents(auto&& obj) -> decltype(auto) {
            return FWD(obj).get_extents();
        }

        static constexpr auto evaluate(auto&& obj, is_scalar_index auto&& i) -> decltype(auto) {
            return FWD(obj).evaluate(FWD(i));
        }
    };

    template <class T>
    concept is_tensor = has_tensor_traits<T>;

    template <class T>
    concept is_scalar = is_tensor<T> and (order<T> == 0);
}
