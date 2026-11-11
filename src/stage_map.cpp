#include <blessed/span.hpp>

#include "stage_map.hpp"

namespace hawkstar
{
    namespace detail
    {
        /**
         * @brief: test utility class to easily generate a map programmatically
         */
        class TestMap
        {
        public:
            using size_type = std::size_t;

            static size_type constexpr width = 128;
            static size_type constexpr height = 128;

            TestMap() :
                map_{UINT8_C(0)}
            {
                /* walls */
                for (size_type y{}; y < height; ++y)
                {
                    set(0, y, 1);
                    set(width - 1, y, 1);
                }

                /* floor */
                for (size_type x{}; x < width; ++x)
                {
                    set(x, 0, 1);
                }
            }

            uint8_t const *data() const noexcept
            {
                return map_.data();
            }

            size_t size() const noexcept
            {
                return map_.size();
            }

            uint8_t get(size_type x, size_type y) const noexcept
            {
                return get(offset(x, y));
            }

            void set(size_type x, size_type y, uint8_t v) noexcept
            {
                set(offset(x, y), v);
            }

        private:
            static size_type constexpr offset(std::size_t x, std::size_t y)
            {
                return x * y;
            }

            uint8_t get(size_type pos) const noexcept
            {
                return map_[pos];
            }

            void set(size_type pos, uint8_t v)
            {
                map_[pos] = v;
            }

            std::array<uint8_t, width * height> map_;
        };
    }

    /**
     * note: starting with tiles at 16px x 16px and a screen resolution of 
     * 1024px x 768px -> screen grid of 64 x 48. the loaded grid will need to
     * be +2 in each extent to compensate for the player being at a mid-point
     * between tiles in either axis.
     * 
     * stage map management. intent is to abstract loading the world state. 
     * 
     * at any time we should be able to populate a screen grid based on the 
     * camera position (which may often correlate with the player position).
     * 
     * initial implementation will simply store the entire world. we can 
     * optimize to a lower memory footprint later.
     * 
     * open question on how we store enemy data for a level, and how we reset
     * state. ideally we don't want to have to reload the whole thing so this
     * needs some consideration. if it's possible to just have a low-cost 
     * shadow model of perishable entities then this is the ideal.
     */

    StageMap::StageMap() :
        data_{UINT8_C(0)}
    {
        detail::TestMap test;
        load(test.data(), test.width, test.height);
    }

    bool StageMap::load()
    {
        return false;
    }

    bool StageMap::load(uint8_t const *v, std::size_t w, std::size_t h)
    {
        width_ = w;
        height_ = h;
        std::copy(v, v + (width_ * height_), data_.begin());
        return true;
    }

    void StageMap::clear()
    {}

    uint8_t StageMap::getId(std::size_t x, std::size_t y)
    {
        std::size_t offset{x * y};

        if (offset < data_.size())
            return data_[offset];

        return 0;
    }

    rectangle<std::size_t> StageMap::getBounds(void) const
    {
        return rectangle<std::size_t>{
            width_,
            height_
        };
    }

    rectangle<std::size_t> StageMap::getTileRegion(rectangle<std::size_t> const &r) const
    {
        rectangle<std::size_t>::value_type width = r.width() / tile_width;
        rectangle<std::size_t>::value_type height = r.height() / tile_height;
        rectangle<std::size_t>::value_type x = r.x() / tile_width;
        rectangle<std::size_t>::value_type y = r.y() / tile_height;

        /* expand left */
        if ((r.x() % tile_width) != 0)
        {
            --x;
            width += 1;
        }

        /* expand down */
        if ((r.y() % tile_height) != 0)
        {
            --y;
            height += 1;
        }

        /* expand right */
        if (((r.x() + r.width()) % tile_width) != 0)
        {
            ++width;
        }

        /* expand up */
        if (((r.y() + r.height()) % tile_height) != 0)
        {
            ++height;
        }

        return rectangle<std::size_t>{x, y, width, height};
    }

    rectangle<std::size_t> StageMap::getWorldRegion(std::size_t x, std::size_t y) const
    {
        return rectangle<std::size_t>{x * tile_width, y * tile_height, tile_width, tile_height};
    }
}