#include "benchmark.hh"

#include "scarp/sur/sur.hh"

class SURBenchmark : public Benchmark
{
public:
  virtual BinaryControls execute(const Instance& instance,
                                 const CostFunction& costs,
                                 bool vanishing_constraints) override;

};

BinaryControls
SURBenchmark::execute(const Instance& instance,
                      const CostFunction& costs,
                      bool vanishing_constraints)
{
  return compute_sur_controls(instance, vanishing_constraints);
}


int main(int argc, char** argv)
{
  SURBenchmark().execute_all(argc, argv);

  return 0;
}
