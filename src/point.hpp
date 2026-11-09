#ifndef POINT_HPP_
#define POINT_HPP_

class point2
{
public:
    using value_type = float;

    point2() :
        x_{},
        y_{}
    {}

    point2(value_type px, value_type py) :
        x_(px),
        y_(py)
    {}

    point2(point2 const &other) :
        x_{other.x_},
        y_{other.y_}
    {}

    point2 &operator=(point2 const &other)
    {
        x_ = other.x_;
        y_ = other.y_;

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

private:
    value_type x_;
    value_type y_;
};

class point3
{
public:
    using value_type = float;

    point3() :
        x_{},
        y_{},
        z_{}
    {}

    point3(value_type px, value_type py, value_type pz) :
        x_{px},
        y_{py},
        z_{pz}
    {}

    point3(point3 const &other) :
        x_{other.x_},
        y_{other.y_},
        z_{other.z_}
    {}

    point3 &operator=(point3 const &other)
    {
        x_ = other.x_;
        y_ = other.y_;
        z_ = other.z_;

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

    inline value_type z() const noexcept
    {
        return z_;
    }

private:
    value_type x_;
    value_type y_;
    value_type z_;
};

#endif
