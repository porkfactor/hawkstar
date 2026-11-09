#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <cstdint>
#include <cstddef>
#include <cmath>
#include <type_traits>

namespace detail
{
    template<typename _Type, std::enable_if_t<std::is_floating_point<_Type>::value, bool> = 0>
    static inline _Type sqrt(_Type const &v)
    {
        return std::sqrt(v);
    }

    template<typename _Type, std::enable_if_t<std::is_integral<_Type>::value, bool> = 0>
    static inline _Type sqrt(_Type const &v)
    {
        return static_cast<_Type>(std::sqrt(static_cast<double>(v)));
    }
}

class vector2
{
public:
    using value_type = float;

    vector2() :
        x_{},
        y_{}
    {}

    vector2(float px, float py) :
        x_(px),
        y_(py)
    {}

    vector2(vector2 const &r) :
        x_(r.x_),
        y_(r.y_)
    {}

    inline value_type x() const
    {
        return x_;
    }

    inline value_type y() const
    {
        return y_;
    }

    bool operator==(vector2 const &r) const
    {
        return (x_ == r.x_) && (y_ == r.y_);
    }

    bool operator!=(vector2 const &r) const
    {
        return (x_ != r.x_) || (y_ != r.y_);
    }
    
    vector2 &operator=(vector2 const &r)
    {
        x_ = r.x_;
        y_ = r.y_;
        return *this;
    }

    vector2 &operator+=(vector2 const &r)
    {
        x_ += r.x_;
        y_ += r.y_;
        return *this;
    }

    vector2 &operator-=(vector2 const &r)
    {
        x_ -= r.x_;
        y_ -= r.y_;
        return *this;
    }

    value_type magnitude(void) const
    {
        return std::sqrt((x_ * x_) + (y_ * y_));
    }

    vector2 normalize(void) const
    {
        return vector2(*this) / magnitude();
    }

    vector2 normal(void) const
    {
        return vector2(-y_, x_);
    }

    vector2 floor(void) const
    {
        return vector2(std::floor(x_), std::floor(y_));
    }

    template<typename _Type, std::enable_if_t<std::is_integral<_Type>::value, bool> = 0>
    vector2 &operator*=(_Type r)
    {
        x_ = x_ * static_cast<value_type>(r);
        y_ = y_ * static_cast<value_type>(r);
        return *this;
    };

    template<typename _Type, std::enable_if_t<std::is_floating_point<_Type>::value, bool> = 0>
    vector2 &operator*=(_Type r)
    {
        x_ = x_ * static_cast<value_type>(r);
        y_ = y_ * static_cast<value_type>(r);
        
        return *this;
    };

    template<typename _Type, std::enable_if_t<std::is_integral<_Type>::value, bool> = 0>
    vector2 &operator/=(_Type const &r)
    {
        x_ = x_ / static_cast<value_type>(r);
        y_ = y_ / static_cast<value_type>(r);
        return *this;
    };

    template<typename _Type, std::enable_if_t<std::is_floating_point<_Type>::value, bool> = 0>
    vector2 &operator/=(_Type const &r)
    {
        x_ = x_ / static_cast<value_type>(r);
        y_ = y_ / static_cast<value_type>(r);
        return *this;
    };

    inline friend vector2 operator+(vector2 const &l, vector2 const &r)
    {
        vector2 result{l};
        return result += r;
    }

    inline friend vector2 operator-(vector2 const &l, vector2 const &r)
    {
        vector2 result{l};
        return result -= r;
    }

    template<typename _Type>
    inline friend vector2 operator*(vector2 const &l, _Type const &r)
    {
        vector2 result{l};
        return result *= r;
    }

    template<typename _Type>
    inline friend vector2 operator/(vector2 const &l, _Type const &r)
    {
        vector2 result{l};
        return result /= r;
    }

private:
    value_type x_;
    value_type y_;
};

class vector3
{
public:
    using value_type = float;

    vector3() :
        x_{},
        y_{},
        z_{}
    {}

    vector3(float px, float py, float pz) :
        x_(px),
        y_(py),
        z_(pz)
    {}

    vector3(vector3 const &r) :
        x_(r.x_),
        y_(r.y_),
        z_(r.z_)
    {}

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

    bool operator==(vector3 const &r) const
    {
        return (x_ == r.x_) && (y_ == r.y_) && (z_ == r.z_);
    }

    bool operator!=(vector3 const &r) const
    {
        return (x_ != r.x_) || (y_ != r.y_) || (z_ != r.z_);
    }
    
    vector3 &operator=(vector3 const &r)
    {
        x_ = r.x_;
        y_ = r.y_;
        z_ = r.z_;
        return *this;
    }

    vector3 &operator+=(vector3 const &r)
    {
        x_ += r.x_;
        y_ += r.y_;
        z_ += r.z_;
        return *this;
    }

    vector3 &operator-=(vector3 const &r)
    {
        x_ -= r.x_;
        y_ -= r.y_;
        z_ -= r.z_;
        return *this;
    }

    value_type magnitude(void) const
    {
        return std::sqrt((x_ * x_) + (y_ * y_) + (z_ * z_));
    }

    vector3 normalize(void) const
    {
        return vector3(*this) / magnitude();
    }

    vector3 normal(void) const
    {
        return vector3(-y_, x_, z_);
    }

    vector3 floor(void) const
    {
        return vector3(std::floor(x_), std::floor(y_), std::floor(z_));
    }

    template<typename _Type, std::enable_if_t<std::is_integral<_Type>::value, bool> = 0>
    vector2 &operator*=(_Type r)
    {
        x_ = x_ * static_cast<value_type>(r);
        y_ = y_ * static_cast<value_type>(r);
        z_ = z_ * static_cast<value_type>(r);
        return *this;
    };

    template<typename _Type, std::enable_if_t<std::is_floating_point<_Type>::value, bool> = 0>
    vector3 &operator*=(_Type r)
    {
        x_ = x_ * static_cast<value_type>(r);
        y_ = y_ * static_cast<value_type>(r);
        z_ = z_ * static_cast<value_type>(r);
        return *this;
    };

    template<typename _Type, std::enable_if_t<std::is_integral<_Type>::value, bool> = 0>
    vector3 &operator/=(_Type const &r)
    {
        x_ = x_ / static_cast<value_type>(r);
        y_ = y_ / static_cast<value_type>(r);
        z_ = z_ / static_cast<value_type>(r);
        return *this;
    };

    template<typename _Type, std::enable_if_t<std::is_floating_point<_Type>::value, bool> = 0>
    vector3 &operator/=(_Type const &r)
    {
        x_ = x_ / static_cast<value_type>(r);
        y_ = y_ / static_cast<value_type>(r);
        z_ = z_ / static_cast<value_type>(r);
        return *this;
    };

    inline friend vector3 operator+(vector3 const &l, vector3 const &r)
    {
        vector3 result{l};
        return result += r;
    }

    inline friend vector3 operator-(vector3 const &l, vector3 const &r)
    {
        vector3 result{l};
        return result -= r;
    }

    template<typename _Type>
    inline friend vector3 operator*(vector3 const &l, _Type const &r)
    {
        vector3 result{l};
        return result *= r;
    }

    template<typename _Type>
    inline friend vector3 operator/(vector3 const &l, _Type const &r)
    {
        vector3 result{l};
        return result /= r;
    }

private:
    value_type x_;
    value_type y_;
    value_type z_;
};

#endif
