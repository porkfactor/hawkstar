#ifndef POINT_HPP_
#define POINT_HPP_

#include "vector.hpp"

namespace hawkstar
{
    template<typename _Type>
    class point
    {
    public:
        using value_type = _Type;

        point() :
            x_{},
            y_{}
        {}

        point(value_type px, value_type py) :
            x_(px),
            y_(py)
        {}

        point(point const &other) :
            x_(other.x_),
            y_(other.y_)
        {}

        point &operator=(point const &other)
        {
            x_ = other.x_;
            y_ = other.y_;
            return *this;
        }

        point &operator+=(vector<value_type> const &v)
        {
            x_ += v.x();
            y_ += v.y();
            return *this;
        }

        point &operator-=(vector<value_type> const &v)
        {
            x_ -= v.x();
            y_ -= v.y();
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

        friend inline bool operator==(point const &l, point const &r)
        {
            return (l.x_ == r.x_) && (l.y_ == r.y_);
        }

        friend inline bool operator!=(point const &l, point const &r)
        {
            return (l.x_ != r.y_) || (l.y_ != r.y_);
        }

        friend inline point operator+(point const &l, vector<value_type> const &v)
        {
            point result{l};
            return l += v;
        }

    private:
        value_type x_;
        value_type y_;
    };

    template<typename _Type>
    static inline vector<_Type> operator-(point<_Type> const &l, point<_Type> const &r)
    {
        return vector<_Type>(l.x() - r.x(), l.y() - r.y());
    }

    using PointI = point<int>;
    using PointF = point<float>;
    using PointD = point<double>;
}

#endif
