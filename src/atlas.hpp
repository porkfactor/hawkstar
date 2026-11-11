#ifndef HAWKSTAR_ATLAS_HPP_
#define HAWKSTAR_ATLAS_HPP_

#include <cstddef>
#include <cstdint>

namespace hawkstar
{
    class Tile
    {
    public:

    private:
    };

    class Atlas
    {
    public:
        Atlas();

        std::size_t width();
        std::size_t height();

        Tile const *get(std::size_t, std::size_t) const noexcept;

    private:
        uint8_t map_[256];
    };
}

#endif
