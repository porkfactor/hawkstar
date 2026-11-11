#ifndef HAWKSTAR_RECTANGLE_HPP_
#define HAWKSTAR_RECTANGLE_HPP_

#include "point.hpp"

namespace hawkstar
{
    template<typename _Type>
    class rectangle
    {
    public:
        using value_type = _Type;

        rectangle() :
            x_{},
            y_{},
            width_{},
            height_{}
        {}

        rectangle(value_type px, value_type py, value_type pw, value_type ph) :
            x_(px),
            y_(py),
            width_(pw),
            height_(ph)
        {}

        rectangle(value_type pw, value_type ph) :
            rectangle(0, 0, pw, ph)
        {}

        rectangle(point<value_type> const &p, value_type pw, value_type ph) :
            rectangle(p.x(), p.y(), pw, ph)
        {}

        rectangle(rectangle const &other) :
            rectangle(other.x_, other.y_, other.width_, other.height_)
        {}

        rectangle const &operator=(rectangle const &other)
        {
            x_ = other.x_;
            return *this;
        }

        inline value_type x() const noexcept
        {
            return x_;
        }

        inline value_type y() const noexcept
        {
            return y_;
        }

        inline value_type width() const noexcept
        {
            return width_;
        }

        inline value_type height() const noexcept
        {
            return height_;
        }

        friend inline bool operator==(rectangle const &l, rectangle const &r)
        {
            return (l.x_ == r.x_) && (l.y_ == r.y_) && (l.width_ == r.width_) && (l.height_ == r.height_);
        }

        friend inline bool operator!=(rectangle const &l, rectangle const &r)
        {
            return (l.x_ != r.x_) || (l.y_ != r.y_) || (l.width_ != r.width_) || (l.height_ != r.height_);
        }

    private:
        value_type x_;
        value_type y_;
        value_type width_;
        value_type height_;
    };

    using RectI = rectangle<int>;
    using RectF = rectangle<float>;
    using RectD = rectangle<double>;
}

#endif
