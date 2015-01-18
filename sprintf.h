#ifndef SPRINTF_H
#define SPRINTF_H

#include <string>
#include <sstream>

#if __cplusplus < 201100L
#error "Requires C++11 features"
#else

namespace utils {

namespace _internal {

void sprintf(::std::stringstream &ret, const char *fmt)
{
  while (*fmt) {
    if (*fmt == '%') {
      fmt++;
      if (*fmt == '%') {
        fmt++;
        ret << '%';
      } else {
        ret.str("");
        return;
      }
    } else {
      ret << *fmt++;
    }
  }
}

template<typename T, typename... Args>
void sprintf(::std::stringstream &ret, const char *fmt, const T &param, Args... args)
{
  while (*fmt) {
    if (*fmt == '%') {
      fmt++;
      switch (*fmt) {
        case '%': {
          ret << '%'; fmt++;
        }
        case 'v': {
          ret << param;
          fmt++;
          _internal::sprintf(ret, fmt, args...);
          return;
        }
        default: {
          ret.str("");
          return;
        }
      }
    } else {
      ret << *fmt++;
    }
  }
}

} // _internal

template<typename... Args>
::std::string sprintf(const char *fmt, const Args... params)
{
  ::std::stringstream ret;
  _internal::sprintf(ret, fmt, params...);
  return ret.str();
}

} // utils

#endif // __cplusplus < 201100L

#endif // SPRINTF_H