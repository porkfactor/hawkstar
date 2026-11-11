#ifndef HAWKSTAR_STAGE_MAP_HPP_
#define HAWKSTAR_STAGE_MAP_HPP_

#include <cstdint>
#include <array>

#include "point.hpp"
#include "rectangle.hpp"

namespace hawkstar
{
    class StageMap
    {
    public:
        static std::size_t constexpr tile_width = 16;
        static std::size_t constexpr tile_height = 16;

        StageMap();

        bool load();
        bool load(uint8_t const *, size_t, size_t);

        void clear();

        uint8_t getId(size_t x, size_t y);
        uint8_t getId(point<std::size_t> const &);

        rectangle<std::size_t> getBounds() const;

        rectangle<std::size_t> getTileRegion(rectangle<std::size_t> const &) const;
        rectangle<std::size_t> getWorldRegion(rectangle<std::size_t> const &) const;
        rectangle<std::size_t> getWorldRegion(point<std::size_t> const &) const;
        rectangle<std::size_t> getWorldRegion(std::size_t x, std::size_t y) const;

    private:
        std::array<uint8_t, 256 * 256> data_;
        std::size_t width_;
        std::size_t height_;
    };
}

#endif
