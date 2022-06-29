#pragma once

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
    concept is_extents = is_array_of<T, int> and has_static_size<T>;

    template <class T, int N>
    concept is_extents_of_order = is_extents<T> and size<T> == N;

    namespace _detail
    {
        template <class T>
        concept _has_tensor_traits_specialization = requires {
            typename tensor_traits_t<T>;
        };

        template <class T>
        concept _has_tensor_traits_scalar_type = _has_tensor_traits_specialization<T> and requires {
            typename tensor_traits_t<T>::scalar_type;
        };

        template <class T>
        concept _has_tensor_traits_get_extents = _has_tensor_traits_specialization<T>
            and requires (T t) {
                { tensor_traits_t<T>::get_extents(t) } -> is_extents;
            };

        template <class T>
        concept _has_tensor_traits_evaluate = _has_tensor_traits_specialization<T>
            and _has_tensor_traits_scalar_type<T>
            and _has_tensor_traits_get_extents<T>
            and requires (T t, scalar_index<size<decltype(tensor_traits_t<T>::get_extents(t))>> const& i) {
                { tensor_traits_t<T>::evaluate(t, i) } -> std::convertible_to<typename tensor_traits_t<T>::scalar_type>;
            };
    };

    template <class T>
    concept is_tensor = _detail::_has_tensor_traits_scalar_type<T>
        and _detail::_has_tensor_traits_get_extents<T>
        and _detail::_has_tensor_traits_evaluate<T>;

    template <_detail::_has_tensor_traits_scalar_type T>
    using scalar_type_t = typename tensor_traits_t<T>::scalar_type;

    template <_detail::_has_tensor_traits_get_extents T>
    constexpr int order = size<decltype(tensor_traits_t<T>::get_extents(std::declval<T>()))>;

    namespace _detail
    {
        struct _get_extents_fn
        {
            template <_detail::_has_tensor_traits_get_extents T>
            constexpr auto operator()(T&& obj) const -> decltype(auto) {
                return tensor_traits_t<T>::get_extents(FWD(obj));
            }
        };
    }
    constexpr _detail::_get_extents_fn extents{};

    namespace _detail
    {
        struct _evaluate_fn
        {
            template <_detail::_has_tensor_traits_evaluate T>
            constexpr auto operator()(T&& obj, is_scalar_index auto&& i) const -> decltype(auto) {
                return tensor_traits_t<T>::evaluate(FWD(obj), FWD(i));
            }
        };
    }
    constexpr _detail::_evaluate_fn evaluate{};

    template <class T, auto N>
    concept is_tensor_of_order = is_tensor<T> and (order<T> == N);

    template <class T>
    concept is_scalar = is_tensor_of_order<T, 0>;

    namespace _detail
    {
        template <class T>
        concept _has_member_scalar_type = requires {
            typename std::remove_cvref_t<T>::scalar_type;
        };

        template <class T>
        concept _has_member_get_extents = requires (T t) {
            { t.get_extents() } -> is_extents;
        };

        template <class T>
        concept _has_member_evaluate = _has_member_scalar_type<T>
            and _has_member_get_extents<T>
            and requires (T t, scalar_index<size<decltype(t.get_extents())>> const& i) {
                { t.evaluate(i) } -> std::convertible_to<typename std::remove_cvref_t<T>::scalar_type>;
            };

        template <class T>
        concept _has_member_tensor_traits = _has_member_scalar_type<T>
            and _has_member_get_extents<T>
            and _has_member_evaluate<T>;
    }

    template <_detail::_has_member_tensor_traits T>
    struct tensor_traits<T>
    {
        using scalar_type = typename std::remove_cvref_t<T>::scalar_type;

        static constexpr auto get_extents(auto&& obj) -> decltype(auto) {
            return FWD(obj).get_extents();
        }

        static constexpr auto evaluate(auto&& obj, is_scalar_index auto&& i) -> decltype(auto) {
            return FWD(obj).evaluate(FWD(i));
        }
    };
}
