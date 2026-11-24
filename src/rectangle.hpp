#ifndef HAWKSTAR_RECTANGLE_HPP_
#define HAWKSTAR_RECTANGLE_HPP_

#include <algorithm>

#include "point.hpp"

namespace hawkstar
{
    template<typename _Type>
    class rectangle
    {
    public:
        using value_type = _Type;
        using point_type = point<value_type>;

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

        rectangle(point_type const &p, value_type pw, value_type ph) :
            rectangle(p.x(), p.y(), pw, ph)
        {}

        rectangle(point_type const &p1, point_type const &p2)
        {
            value_type minx{std::min(p1.x(), p2.x())};
            value_type miny{std::min(p1.y(), p2.y())};
            value_type maxx(std::max(p1.x(), p2.x()));
            value_type maxy{std::max(p1.y(), p2.y())};

            x_ = minx;
            y_ = miny;
            width_ = maxx - minx;
            height_ = maxy - miny;
        }

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

        bool contains(point_type const &p) const noexcept
        {
            return
                (left() <= p.x()) && 
                (right() >= p.x()) &&
                (bottom() <= p.y()) &&
                (top() >= p.y());
        }

        template<typename _Other>
        bool contains(point<_Other> const &p) const noexcept
        {
            return contains(point_type{p});
        }

        inline bool empty() const noexcept
        {
            return (width_ == value_type{}) && (height_ == value_type{});
        }

        value_type perimiter() const noexcept
        {
            return (height_ * value_type{2}) + (width_ * value_type{2});
        }

        value_type area() const noexcept
        {
            return height_ * width_;
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

        inline point_type position() const noexcept
        {
            return point_type{x_, y_};
        }

        /**
         * @brief get the centre point of the rectangle
         * 
         */
        inline point_type centre() const noexcept
        {
            return point_type(
                x_ + (width_ / value_type{2}),
                y_ + (height_ / value_type{2})
            );
        }

        /**
         * @brief reposition to use @p pos as centre point
         * 
         * @param pos                   the point on which to centre the rectangle
         */
        void centre(point_type const &pos) noexcept
        {
            centre(pos.x(), pos.y());
        }

        void centre(value_type px, value_type py) noexcept
        {
            move(px - (width_ / value_type{2}), py - (height_ / value_type{2}));
        }

        void move(point_type const &pos) noexcept
        {
            move(pos.x(), pos.y());
        }

        void move(value_type px, value_type py) noexcept
        {
            x_ = px;
            y_ = py;
        }

        rectangle &merge(rectangle const &other)
        {
            auto minx = std::min(x_, other.x_);
            auto miny = std::min(y_, other.y_);
            auto maxx = std::max(right(), other.right());
            auto maxy = std::max(top(), other.top());

            *this = rectangle{minx, miny, maxx - minx, maxy - miny};

            return *this;
        }

        rectangle &merge(point_type const &p)
        {
            return merge(rectangle{p});
        }

        friend inline rectangle merge(rectangle const &l, rectangle const &r)
        {
            rectangle result{l};
            return l.merge(r);
        }

        friend inline rectangle mrege(rectangle const &l, point_type const &r)
        {
            rectangle result{l};
            return l.merge(r);
        }

        rectangle &intersect(rectangle const &other)
        {
            if (intersects(other))
            {
                auto minx = std::max(x_, other.x_);
                auto maxx = std::min(right(), other.right());
                auto miny = std::max(y_, other.y_);
                auto maxy = std::min(top(), other.top());

                *this = rectangle{minx, miny, maxx - minx, maxy - miny};
            }
            else
            {
                *this = rectangle{};
            }

            return *this;
        }

        friend inline rectangle intersect(rectangle const &l, rectangle const &r)
        {
            rectangle result{l};
            return l.intersect(r);
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
}

#endif
