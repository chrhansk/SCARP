#include "benchmark.hh"

#include "scarp/bounds.hh"
#include "scarp/scarp/scarp_program.hh"

class SCARPBenchmark : public Benchmark
{
public:
  virtual BinaryControls execute(const Instance& instance,
                                 const CostFunction& costs,
                                 bool vanishing_constraints) override;

  double bound_for(const Instance& instance,
                   bool vanishing_constraints) const;
};

BinaryControls
SCARPBenchmark::execute(const Instance& instance,
                        const CostFunction& costs,
                        bool vanishing_constraints)
{
  const double max_deviation = bound_for(instance, vanishing_constraints);

  SCARPProgram program(instance,
                       costs,
                       max_deviation);

  return program.solve();
}

double SCARPBenchmark::bound_for(const Instance& instance,
                                 bool vanishing_constraints) const
{
  const idx dimension = instance.dimension();

  return bounds::sur(dimension, vanishing_constraints).for_mesh(instance.get_mesh());
}

int main(int argc, char** argv)
{
  SCARPBenchmark().execute_all(argc, argv);

  return 0;
}
