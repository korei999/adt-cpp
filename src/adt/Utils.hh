#pragma once

namespace adt
{

constexpr auto
max(auto l, auto r)
{
    return l > r ? l : r;
}

constexpr auto
min(auto l, auto r)
{
    return l < r ? l : r;
}

} /* namespace adt */
