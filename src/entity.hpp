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
    point<double> position_;
    vector<double> movement_;
    vector<double> acceleration_;
};

#endif
