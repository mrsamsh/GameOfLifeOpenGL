//
// Log.hpp
// GEL
//
// Created by Usama Alshughry 25.09.2022.
// Copyright © 2022 Usama Alshughry. All rights reserved
//

#ifndef LOG_HPP_
#define LOG_HPP_

#include <ctime>
#include <iostream>

static char _Time[11];

inline static char *getTime() {
  std::time_t t = std::time(nullptr);
  std::strftime(_Time, sizeof(_Time), "%T", std::localtime(&t));
  return _Time;
}

#define Error(message, ...)                                                    \
  do {                                                                         \
    fprintf(stderr, "[%s] (%s:%d) - \033[0;31mERROR: ", getTime(), __FILE__,   \
            __LINE__);                                                         \
    fprintf(stderr, message "\033[0m\n", ##__VA_ARGS__);                       \
  } while (0)

#define ErrorReturn(retval, ...)                                               \
  do {                                                                         \
    Error(__VA_ARGS__);                                                        \
    return retval;                                                             \
  } while (0)

#define SELECT_ELEVENTH(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, ...) a11
#define COUNT(...)                                                             \
  SELECT_ELEVENTH(dummy, ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define CONCAT2(a, b) a##b
#define CONCAT(a, b) CONCAT2(a, b)

#define PRINT_10(x, ...) x << ' ' << PRINT_9(__VA_ARGS__)
#define PRINT_9(x, ...) x << ' ' << PRINT_8(__VA_ARGS__)
#define PRINT_8(x, ...) x << ' ' << PRINT_7(__VA_ARGS__)
#define PRINT_7(x, ...) x << ' ' << PRINT_6(__VA_ARGS__)
#define PRINT_6(x, ...) x << ' ' << PRINT_5(__VA_ARGS__)
#define PRINT_5(x, ...) x << ' ' << PRINT_4(__VA_ARGS__)
#define PRINT_4(x, ...) x << ' ' << PRINT_3(__VA_ARGS__)
#define PRINT_3(x, ...) x << ' ' << PRINT_2(__VA_ARGS__)
#define PRINT_2(x, ...) x << ' ' << PRINT_1(__VA_ARGS__)
#define PRINT_1(x) x << ' ' << PRINT_0()
#define PRINT_0() std::endl

#define Print(...)                                                             \
  std::cout << '[' << getTime() << "] "                                        \
            << CONCAT(PRINT_, COUNT(__VA_ARGS__))(__VA_ARGS__)

#define Printf(message, ...)                                                   \
  printf("[%s] " message "\n", getTime(), ##__VA_ARGS__)

#define Assert(x, message, ...)                                                \
  do {                                                                         \
    if (!(x)) {                                                                \
      Error(message, ##__VA_ARGS__);                                           \
      exit(-1);                                                                \
    }                                                                          \
  } while (0)

#define AssertReturn(x, retval, message, ...)                                  \
  do {                                                                         \
    if (!(x)) {                                                                \
      ErrorReturn(retval, message, ##__VA_ARGS__);                             \
    }                                                                          \
  } while (0)

#endif // LOG_HPP_
