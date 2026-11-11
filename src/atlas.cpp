#include "atlas.hpp"

namespace hawkstar
{
    Atlas::Atlas()
    {}

    Tile const *Atlas::get(std::size_t x, std::size_t y) const noexcept
    {
        std::size_t index = x * y;
    }
}
