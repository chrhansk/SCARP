#ifndef SCARP_TIMER_HH
#define SCARP_TIMER_HH

#include <algorithm>
#include <numeric>
#include <chrono>

#include "log.hh"

class Timer
{
private:
  std::chrono::time_point<std::chrono::steady_clock> start;

public:
  Timer() :
    start(std::chrono::steady_clock::now())
  {}

  double elapsed()
  {
    auto finish = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
  }
};

#endif /* SCARP_TIMER_HH */
