#ifndef HAWKSTAR_TYPES_HPP_
#define HAWKSTAR_TYPES_HPP_

#include "point.hpp"
#include "rectangle.hpp"
#include "vector.hpp"

namespace hawkstar
{
    using world_integral_t = double;
    using world_rectangle = rectangle<world_integral_t>;
    using world_point = point<world_integral_t>;
    using world_vector = vector<world_integral_t>;

    using tile_integral_t = std::intmax_t;
    using tile_rectangle = rectangle<tile_integral_t>;
    using tile_point = point<tile_integral_t>;
}

#endif
