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

        template<typename _Other>
        explicit rectangle(rectangle<_Other> const &other) :
            rectangle(
                static_cast<value_type>(other.x()),
                static_cast<value_type>(other.y()),
                static_cast<value_type>(other.width()),
                static_cast<value_type>(other.height()))
        {}

        rectangle &operator=(rectangle const &other)
        {
            x_ = other.x_;
            y_ = other.y_;
            width_ = other.width_;
            height_ = other.height_;
            return *this;
        }

        template<typename _Other>
        rectangle &operator=(rectangle<_Other> const &other)
        {
            x_ = static_cast<value_type>(other.x());
            y_ = static_cast<value_type>(other.y());
            width_ = static_cast<value_type>(other.width());
            height_ = static_cast<value_type>(other.height());
            return *this;
        }

        bool contains(rectangle const &other) const noexcept
        {
            return
                (left() <= other.left()) &&
                (right() >= other.right()) &&
                (bottom() <= other.bottom()) &&
                (top() >= other.top())
                ;
        }

        template<typename _Other>
        bool contains(rectangle<_Other> const &other) const noexcept
        {
            return
                (left() <= static_cast<value_type>(other.left())) &&
                (right() >= static_cast<value_type>(other.right())) &&
                (top() >= static_cast<value_type>(other.top())) &&
                (bottom() <= static_cast<value_type>(other.bottom()))
                ;
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

        inline value_type left() const noexcept
        {
            return x_;
        }

        inline value_type right() const noexcept
        {
            return x_ + width_;
        }

        inline value_type bottom() const noexcept
        {
            return y_;
        }

        inline value_type top() const noexcept
        {
            return y_ + height_;
        }

        inline point<value_type> origin() const noexcept
        {
            return point<value_type>{x_, y_};
        }

        friend inline bool operator>(rectangle const &l, rectangle const &r)
        {
            return l.contains(r);
        }

        friend inline bool operator>=(rectangle const &l, rectangle const &r)
        {
            return l.contains(r);
        }

        friend inline bool operator<(rectangle const &l, rectangle const &r)
        {
            return r.contains(l);
        }

        friend inline bool operator<=(rectangle const &l, rectangle const &r)
        {
            return r.contains(l);
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
