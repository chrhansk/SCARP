#include "benchmark.hh"

#include "scarp/scarp_program.hh"

class SCARPBenchmark : public Benchmark
{
public:
  virtual std::vector<Controls> execute(const std::vector<Controls>& fractional_controls,
                                        double scale_factor) override;

};

std::vector<Controls>
SCARPBenchmark::execute(const std::vector<Controls>& fractional_controls,
                        double scale_factor)
{
  idx dimension = fractional_controls.size();

  assert(dimension >= 2);

  std::vector<double> switch_on_costs = default_switch_on_costs(dimension);
  std::vector<double> switch_off_costs = default_switch_off_costs(dimension);

  SwitchCosts switch_costs(switch_on_costs, switch_off_costs);

  SCARPProgram program(fractional_controls,
                       switch_costs,
                       scale_factor);

  return program.solve();
}


int main(int argc, char** argv)
{
  SCARPBenchmark().execute_all(argc, argv);

  return 0;
}
