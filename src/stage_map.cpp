#include <blessed/span.hpp>
#include <SDL3/SDL.h>

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
                return x + (y * width);
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

    uint8_t StageMap::getId(tile_point const &p) const noexcept
    {
        return getId(p.x(), p.y());
    }

    uint8_t StageMap::getId(tile_integral_t x, tile_integral_t y) const noexcept
    {
        /**
         * this may be unnecessary in practice since twos compliment will
         * put negative indices (in one or both cases) will yield bounds
         * far in excess of practical space, but opting to explicitly discard
         * invalid indices in this case
         */
        if ((x < 0) || (y < 0))
            return uint8_t{};

        std::size_t offset{static_cast<std::size_t>(x) + (static_cast<std::size_t>(y) * width_)};

        if (offset < data_.size())
            return data_[offset];

        return 0;
    }

    tile_rectangle StageMap::getTileBounds(void) const noexcept
    {
        return tile_rectangle{
            static_cast<tile_integral_t>(width_),
            static_cast<tile_integral_t>(height_)
        };
    }

    rectangle<double> StageMap::getWorldBounds(void) const noexcept
    {
        return rectangle<double>(getTileBounds());
    }

    tile_rectangle StageMap::getTileRegion(world_rectangle const &r) const
    {
        tile_rectangle pr{r};

        tile_rectangle::value_type width = pr.width() / tile_width;
        tile_rectangle::value_type height = pr.height() / tile_height;
        tile_rectangle::value_type x = pr.x() / tile_width;
        tile_rectangle::value_type y = pr.y() / tile_height;

        /* expand left */
        if ((pr.x() % tile_width) != 0)
        {
            --x;
            width += 1;
        }

        /* expand down */
        if ((pr.y() % tile_height) != 0)
        {
            --y;
            height += 1;
        }

        /* expand right */
        if (((pr.x() + pr.width()) % tile_width) != 0)
        {
            ++width;
        }

        /* expand up */
        if (((pr.y() + pr.height()) % tile_height) != 0)
        {
            ++height;
        }

        return tile_rectangle{x, y, width, height};
    }

    world_rectangle StageMap::getWorldRegion(tile_rectangle const &r) const
    {
        return world_rectangle(
            r.x() * tile_width,
            r.y() * tile_height,
            r.width() * tile_width,
            r.height() * tile_height
        );
    }

    world_rectangle StageMap::getWorldRegion(tile_integral_t x, tile_integral_t y) const
    {
        return world_rectangle{
            static_cast<world_integral_t>(x * tile_width),
            static_cast<world_integral_t>(y * tile_height),
            static_cast<world_integral_t>(tile_width),
            static_cast<world_integral_t>(tile_height)
        };
    }

    #define RTE_ALIGN_FLOOR(val, align)     (typeof(val))((val) & (~((typeof(val))((align) - 1))))

    template<typename _Type>
    static inline _Type constexpr align_floor(_Type value, _Type align)
    {
        return value & ~(align - 1);
    }

    template<typename _Type>
    static inline _Type constexpr align_ceil(_Type value, _Type align)
    {
        return align_floor(value +  align - 1, align);
    }

    /**
     * problem - this won't tell the caller whether they need to change state e.g.
     * if we land during freefall.
     */
    world_point StageMap::try_move(world_point const &p, world_vector const &v) const noexcept
    {
        world_point pos{p};

        /* horizontal motion */
        if (v.x())
        {
            auto ty = static_cast<tile_integral_t>(pos.y());
            auto wx = pos.x() + v.x();

            /* rightward motion */
            if (v.x() > world_integral_t{})
            {
                pos += world_vector{v.x(), 0.0};
            }
            /* leftward motion */
            else
            {
                auto start = align_floor(static_cast<tile_integral_t>(std::floor(pos.x())), tile_width) / tile_width;
                auto end = align_ceil(static_cast<tile_integral_t>(std::floor(wx)), tile_width) / tile_width;

                for (auto tx = start; tx >= end; --tx)
                {
                    if (getId(tx - 1, ty) != 0)
                    {
                        wx = static_cast<world_integral_t>(tx * tile_width);
                        break;
                    }
                }

                pos = world_point{wx, pos.y()};
            }
        }

        /* vertical motion */
        if (v.y())
        {
            auto tx = static_cast<tile_integral_t>(pos.x());
            auto wy = pos.y() + v.y();

            /* upward motion */
            if (v.y() > world_integral_t{})
            {
                pos += world_vector{0.0, v.y()};
            }
            /* downward motion */
            else
            {
                auto start = align_floor(static_cast<tile_integral_t>(std::floor(pos.y())), tile_height) / tile_height;
                auto end = align_ceil(static_cast<tile_integral_t>(std::floor(wy)), tile_height) / tile_height;

                for (auto ty = start; ty >= end; --ty)
                {
                    if (getId((tx / tile_height), ty - 1) != 0)
                    {
                        wy = static_cast<world_integral_t>(ty * tile_height);
                        break;
                    }
                }

                pos = world_point(pos.x(), wy);
            }
        }

        return pos;
    }

    bool StageMap::raycast(point<double> const &p, vector<double> const &direction SDL_UNUSED, double distance SDL_UNUSED) const noexcept
    {
        /**
         * cast to the nearest item *at least* distance meters away, then measure the true distance
         */
        point<std::size_t> w(p);

        /**
         * how do we snap this to the tile grid - it depends on the direction. we're aiming for
         * containment but... if we're casting up then we need to consider the bottom edge, if down
         * then the top edge, etc.
         * 
         * 
         */

        return false;
    }

    bool StageMap::raycast(rectangle<double> const &r, vector<double> const &direction, double distance) const noexcept
    {
        // TODO: we could maybe optimize with edge hints
        return raycast(point<double>{r.left(), r.bottom()}, direction, distance) ||
            raycast(point<double>(r.right(), r.bottom()), direction, distance);
    }
}