#ifndef HAWKSTAR_CAMERA_HPP_
#define HAWKSTAR_CAMERA_HPP_

#include "point.hpp"

namespace hawkstar
{
    class Camera
    {
    public:
        using point_type = point<double>;

        Camera() :
            point_{}
        {}

        inline point_type::value_type x() const noexcept
        {
            return point_.x();
        }

        inline point_type::value_type y() const noexcept
        {
            return point_.y();
        }

        inline point_type const &position() const noexcept
        {
            return point_;
        }

        void move(vector<double> const &v)
        {
            point_ += v;
        }

        void move(point_type const &p)
        {
            point_ = p;
        }

    private:
        point_type point_;
    };
}

#endif
