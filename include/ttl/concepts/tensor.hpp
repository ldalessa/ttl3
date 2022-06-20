#pragma once

#include "ttl/evaluate.hpp"
#include "ttl/traits/extents.hpp"
#include "ttl/traits/order.hpp"
#include "ttl/traits/value_type.hpp"

namespace ttl::concepts
{
    template <class T>
    concept tensor = has_order<T> and has_value_type<T> and has_extents<T> and has_evaluate<T>;
}
