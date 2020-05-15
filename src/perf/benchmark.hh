#ifndef BENCHMARK_HH
#define BENCHMARK_HH

#include "controls.hh"

class Benchmark
{
public:
  void execute_all(int argc, char** argv);

  virtual std::vector<Controls> execute(const std::vector<Controls>& fractional_controls,
                                        double scale_factor) = 0;

};


#endif /* BENCHMARK_HH */
