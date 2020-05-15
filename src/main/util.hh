#ifndef UTIL_HH
#define UTIL_HH

#include <vector>

typedef unsigned int idx;

extern double inf;

constexpr bool debugging_enabled()
{
#if defined(NDEBUG)
  return false;
#else
  return true;
#endif
}

#endif /* UTIL_HH */
