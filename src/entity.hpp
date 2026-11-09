#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include "point.hpp"
#include "vector.hpp"
#include "texture.hpp"

class Entity
{
public:
    Entity();

private:
    point2 location_;
    vector2 direction_;
    unique_sdl_texture texture_;
};

#endif
