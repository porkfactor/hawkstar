#ifndef HAWKSTAR_STAGE_MAP_HPP_
#define HAWKSTAR_STAGE_MAP_HPP_

#include <cstdint>
#include <array>

#include "types.hpp"

namespace hawkstar
{
    class StageMap
    {
    public:
        static tile_integral_t constexpr tile_width = tile_integral_t{16};
        static tile_integral_t constexpr tile_height = tile_integral_t{16};

        StageMap();

        bool load();
        bool load(uint8_t const *, size_t, size_t);

        void clear();

        uint8_t getId(tile_integral_t x, tile_integral_t y) const noexcept;
        uint8_t getId(tile_point const &) const noexcept;

        world_rectangle tileToWorld(tile_rectangle const &) const noexcept;
        world_point tileToWorld(tile_point const &) const noexcept;
        tile_rectangle worldToTile(world_rectangle const &) const noexcept;
        tile_point worldToTile(world_point const &) const noexcept;

        tile_rectangle getTileBounds() const noexcept;
        world_rectangle getWorldBounds() const noexcept;

        tile_rectangle getTileRegion(world_rectangle const &) const;
        world_rectangle getWorldRegion(tile_rectangle const &) const;
        world_rectangle getWorldRegion(tile_point const &) const;
        world_rectangle getWorldRegion(tile_integral_t, tile_integral_t) const;

        bool has_intersection(point<double>);
        bool raycast(world_point const &, world_vector const &, double = double{}) const noexcept;
        bool raycast(world_rectangle const &, world_vector const &, double = double{}) const noexcept;

        world_point try_move(world_point const &, world_vector const &) const noexcept;

    private:
        std::array<uint8_t, 256 * 256> data_;
        std::size_t width_;
        std::size_t height_;
    };
}

#endif
