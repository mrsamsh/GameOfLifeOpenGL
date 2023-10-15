//
// Utility.hpp
// GEL
//
// Created by Usama Alshughry 25.09.2022.
// Copyright © 2022 Usama Alshughry. All rights reserved
//

#ifndef UTILITY_HPP_
#define UTILITY_HPP_

template <typename T>
constexpr inline T SQR(const T x)
{
  return x * x;
}

template <typename T>
constexpr inline T ABS(const T x)
{
  return x >= 0 ? x : -x;
}

template <typename T>
constexpr inline T MIN(const T x, const T y)
{
  return x <= y ? x : y;
}

template <typename T>
constexpr inline T MAX(const T x, const T y)
{
  return x >= y ? x : y;
}

template <typename T>
constexpr inline T CLAMP(const T v, const T low, const T high)
{
  return MIN(high, MAX(low, v));
}

template <typename T>
constexpr inline void SWAP(T& a, T& b)
{
  T temp = a;
  a = b;
  b = temp;
}

template <typename T>
constexpr inline T SNAP(const T x, const int g)
{
  return (static_cast<int>(x) / g) * g;
}

#endif // UTILITY_HPP_
