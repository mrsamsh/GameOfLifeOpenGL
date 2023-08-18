//
// Utility.hpp
// GEL
//
// Created by Usama Alshughry 25.09.2022.
// Copyright © 2022 Usama Alshughry. All rights reserved
//

#ifndef UTILITY_HPP_
#define UTILITY_HPP_

#define SQR(x) ((x) * (x))
#define ABS(x)           (((x) >= (0)) ? (x) : -(x))
#define MIN(x, y)        (((x) <= (y)) ? (x) :  (y))
#define MAX(x, y)        (((x) >= (y)) ? (x) :  (y))
#define CLAMP(x, l, h) MIN(h, MAX(l, x))
#define SWAP(a, b) \
  do {\
    __typeof__(a) temp = a;\
    a = b;\
    b = temp;\
  } while (0)

#endif // UTILITY_HPP_
