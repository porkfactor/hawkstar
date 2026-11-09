#ifndef HAWKSTAR_MAP_HPP_
#define HAWKSTAR_MAP_HPP_

#include <cstddef>
#include <cstdint>
#include <string_view>

#include "texture.hpp"
#include <hawkstar/intrusive_ptr.hpp>

class Texture
{
public:
    Texture(hawkstar::string_view sv);

private:
    unique_sdl_texture texture_;
};

class Tile
{
public:

private:
    hawkstar::intrusive_ptr<Texture> texture_;
};

class Map
{
public:
    Map();

private:
    std::size_t width_;
    std::size_t height_;
    struct Tile **tiles_;
};

#endif
