#include "benchmark.hh"

#include "scarp/bounds.hh"
#include "scarp/cost_function.hh"
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
  idx dimension = instance.dimension();

  assert(dimension >= 2);

  std::vector<double> switch_on_costs(dimension, 0.);
  std::vector<double> switch_off_costs(dimension, 0.);

  SwitchCosts zero_costs(instance, switch_on_costs, switch_off_costs);

  const double max_deviation = bound_for(instance, vanishing_constraints);

  SCARPProgram program(instance,
                       zero_costs,
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
