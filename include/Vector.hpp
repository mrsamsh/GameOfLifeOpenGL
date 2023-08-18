//
// Vector.hpp
// GEL
//
// Created by Usama Alshughry 25.09.2022.
// Copyright © 2022 Usama Alshughry. All rights reserved
//

#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <cmath>
#include <ostream>

#include "Utility.hpp"

#define TINY_BIT 1.e-6

namespace ge
{

template<typename T>
class vec2T
{
public:
  vec2T() : x(0), y(0) { }
  vec2T(const T& a, const T& b) : x{a}, y{b} { }
  vec2T(const vec2T& other) : x{other.x}, y{other.y} { }
  template<typename U>
  vec2T(const vec2T<U>& other) : x(other.x), y(other.y) { }

  vec2T& operator+=(const vec2T& r)
  {
    x += r.x;
    y += r.y;
    return *this;
  }

  vec2T operator+(const vec2T& r) const
  {
    return vec2T(*this).operator+=(r);
  }

  vec2T& operator-=(const vec2T& r)
  {
    x -= r.x;
    y -= r.y;
    return *this;
  }

  vec2T operator-(const vec2T& r) const
  {
    return vec2T(*this).operator-=(r);
  }

  vec2T& operator*=(T scalar)
  {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  vec2T operator*(T scalar)
  {
    return vec2T(*this).operator*=(scalar);
  }

  vec2T& operator/=(T divisor)
  {
    T scalar = 1.f / divisor;
    x *= scalar;
    y *= scalar;
    return *this;
  }

  vec2T operator/(T scalar)
  {
    return vec2T(*this).operator/=(scalar);
  }

  T mag2() const
  {
    return SQR(x) + SQR(y);
  }

  T mag() const
  {
    return sqrt(SQR(x) + SQR(y));
  }

  vec2T normalized() const
  {
    if (isZero()) return vec2T();

    vec2T result(*this);
    return result /= result.mag();
  }

  vec2T& normalize()
  {
    if (isZero())
    {
      zero();
      return *this;
    }
    return *this /= mag();
  }

  operator T* ()
  {
    return elements;
  }

  operator const T* () const
  {
    return elements;
  }

private:
  bool isZero() const
  {
    return ABS(x) < TINY_BIT && ABS(y) < TINY_BIT;
  }

  void zero()
  {
    x = 0;
    y = 0;
  }

public:
  union
  {
    struct { T x, y; };
    struct { T w, h; };
    struct { T s, t; };
    T elements[2];
  };
};

template<typename T>
inline T dotProduct2(const vec2T<T>& l, const vec2T<T>& r)
{
  return l.x * r.x + l.y * r.y;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& st, const vec2T<T>& v)
{
  char log[30];
  snprintf(log, 30, "{ %+-8.3f, %+-8.3f }", v.x, v.y);
  st << log;
  return st;
}

template<>
inline std::ostream& operator<<(std::ostream& st, const vec2T<int>& v)
{
  char log[30];
  snprintf(log, 30, "{ %+-5d, %+-5d }", v.x, v.y);
  st << log;
  return st;
}

template<typename T>
class vec3T
{
public:
  vec3T() : x(0), y(0), z(0) { }
  vec3T(const T& a, const T& b, const T& c) : x{a}, y{b}, z{c} { }
  vec3T(const vec3T& other) : x{other.x}, y{other.y}, z{other.z} { }
  vec3T(const vec2T<T>& other, const T& c) : x{other.x}, y{other.y}, z(c) { }
  template<typename U>
  vec3T(const vec3T<U>& other) : x(other.x), y(other.y), z(other.z) { }

  vec3T& operator+=(const vec3T& r)
  {
    x += r.x;
    y += r.y;
    z += r.z;
    return *this;
  }

  vec3T operator+(const vec3T& r) const
  {
    return vec3T(*this).operator+=(r);
  }

  vec3T& operator-=(const vec3T& r)
  {
    x -= r.x;
    y -= r.y;
    z -= r.z;
    return *this;
  }

  vec3T operator-(const vec3T& r) const
  {
    return vec3T(*this).operator-=(r);
  }

  vec3T& operator*=(T scalar)
  {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  vec3T operator*(T scalar)
  {
    return vec3T(*this).operator*=(scalar);
  }

  vec3T& operator/=(T divisor)
  {
    T scalar = 1.f / divisor;
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  vec3T operator/(T scalar)
  {
    return vec3T(*this).operator/=(scalar);
  }

  T mag3() const
  {
    return SQR(x) + SQR(y) + SQR(z);
  }

  T mag() const
  {
    return sqrt(SQR(x) + SQR(y) + SQR(z));
  }

  vec3T normalized() const
  {
    if (isZero()) return vec3T();

    vec3T result(*this);
    return result /= result.mag();
  }

  vec3T& normalize()
  {
    if (isZero())
    {
      zero();
      return *this;
    }
    return *this /= mag();
  }

  operator T* ()
  {
    return elements;
  }

  operator const T* () const
  {
    return elements;
  }

private:
  bool isZero() const
  {
    return ABS(x) < TINY_BIT && ABS(y) < TINY_BIT && ABS(z) < TINY_BIT;
  }

  void zero()
  {
    x = 0;
    y = 0;
    z = 0;
  }

public:
  union
  {
    struct { T x, y, z; };
    struct { T w, h, d; };
    struct { T s, t, u; };
    T elements[3];
  };
};

template<typename T>
inline T dotProduct3(const vec3T<T>& l, const vec3T<T>& r)
{
  return l.x * r.x + l.y * r.y + l.z * r.z;
}

template<typename T>
inline vec3T<T> crossProduct3(const vec3T<T>& l, const vec3T<T>& r)
{
  return vec3T<T>(
      l.y * r.z - l.z * r.y,
      l.z * r.x - l.x * r.z,
      l.x * r.y - l.y * r.x
  );
}

template<typename T>
inline std::ostream& operator<<(std::ostream& st, const vec3T<T>& v)
{
  char log[41];
  snprintf(log, 41, "{ %+-8.3f, %+-8.3f, %+-8.3f }", v.x, v.y, v.z);
  st << log;
  return st;
}

template<>
inline std::ostream& operator<<(std::ostream& st, const vec3T<int>& v)
{
  char log[41];
  snprintf(log, 41, "{ %+-5d, %+-5d, %+-5d }", v.x, v.y, v.z);
  st << log;
  return st;
}

template<typename T>
class vec4T
{
public:
  vec4T() : x(0), y(0), z(0), w(0) { }
  vec4T(const T& a, const T& b, const T& c, const T& d)
  : x{a}, y{b}, z{c}, w{d} { }
  vec4T(const vec4T& other)
  : x{other.x}, y{other.y}, z{other.z}, w{other.w} { }
  vec4T(const vec2T<T>& other, const T&c, const T& d)
  : x{other.x}, y{other.y}, z{c}, w{d} { }
  vec4T(const vec3T<T>& other, const T& d)
  : x{other.x}, y{other.y}, z{other.z}, w{d} { }
  template<typename U>
  vec4T(const vec4T<U>& other)
  : x(other.x), y(other.y), z(other.z), w(other.w) { }

  vec4T& operator+=(const vec4T& r)
  {
    x += r.x;
    y += r.y;
    z += r.z;
    w += r.w;
    return *this;
  }

  vec4T operator+(const vec4T& r) const
  {
    return vec4T(*this).operator+=(r);
  }

  vec4T& operator-=(const vec4T& r)
  {
    x -= r.x;
    y -= r.y;
    z -= r.z;
    w -= r.w;
    return *this;
  }

  vec4T operator-(const vec4T& r) const
  {
    return vec4T(*this).operator-=(r);
  }

  vec4T& operator*=(T scalar)
  {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
  }

  vec4T operator*(T scalar)
  {
    return vec4T(*this).operator*=(scalar);
  }

  vec4T& operator/=(T divisor)
  {
    T scalar = 1.f / divisor;
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
  }

  vec4T operator/(T scalar)
  {
    return vec4T(*this).operator/=(scalar);
  }

  T mag3() const
  {
    return SQR(x) + SQR(y) + SQR(z) + SQR(w);
  }

  T mag() const
  {
    return sqrt(SQR(x) + SQR(y) + SQR(z) + SQR(w));
  }

  vec4T normalized() const
  {
    if (isZero()) return vec4T();

    vec4T result(*this);
    return result /= result.mag();
  }

  vec4T& normalize()
  {
    if (isZero())
    {
      zero();
      return *this;
    }
    return *this /= mag();
  }

  operator T* ()
  {
    return elements;
  }

  operator const T* () const
  {
    return elements;
  }

private:
  bool isZero() const
  {
    return ABS(x) < TINY_BIT && ABS(y) < TINY_BIT && ABS(z) < TINY_BIT &&
      ABS(w) < TINY_BIT;
  }

  void zero()
  {
    x = 0;
    y = 0;
    z = 0;
    w = 0;
  }

public:
  union
  {
    struct { T x, y, z, w; };
    T elements[4];
  };
};

template<typename T>
inline T dotProduct4(const vec4T<T>& l, const vec4T<T>& r)
{
  return l.x * r.x + l.y * r.y + l.z * r.z + l.w * r.w;
}

template<typename T>
inline vec4T<T> crossProduct4(const vec4T<T>& l, const vec4T<T>& r)
{
  return vec3T<T>(
      l.y * r.z - l.z * r.y,
      l.z * r.x - l.x * r.z,
      l.x * r.y - l.y * r.x,
      0
  );
}

template<typename T>
inline std::ostream& operator<<(std::ostream& st, const vec4T<T>& v)
{
  char log[52];
  snprintf(log, 52, "{ %+-8.3f, %+-8.3f, %+-8.3f, %+-8.3f }", v.x, v.y, v.z, v.w);
  st << log;
  return st;
}

template<>
inline std::ostream& operator<<(std::ostream& st, const vec4T<int>& v)
{
  char log[52];
  snprintf(log, 52, "{ %+-5d, %+-5d, %+-5d, %+-5d }", v.x, v.y, v.z, v.w);
  st << log;
  return st;
}

using vec2 = vec2T<float>;
using ivec2 = vec2T<int>;
using vec3 = vec3T<float>;
using ivec3 = vec3T<int>;
using vec4 = vec4T<float>;
using ivec4 = vec4T<int>;

} // namespace ge

#undef TINY_BIT

#endif // VECTOR_HPP_
