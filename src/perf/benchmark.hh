#ifndef SCARP_BENCHMARK_HH
#define SCARP_BENCHMARK_HH

#include "scarp/instance.hh"
#include "scarp/cost_function.hh"

using namespace scarp;

class Benchmark
{
public:
  void execute_all(int argc, char** argv);

  virtual BinaryControls execute(const Instance& instance,
                                 const CostFunction& costs,
                                 bool vanishing_constraints=false) = 0;

};


#endif /* SCARP_BENCHMARK_HH */
