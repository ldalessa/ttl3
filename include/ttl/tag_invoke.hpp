#pragma once

#include <tag_invoke/tag_invoke.hpp>

namespace ttl::cpos
{
    struct cpo {};
}

namespace ttl
{
    constexpr cpos::cpo cpo_tag{};
}
