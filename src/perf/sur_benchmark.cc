#include "benchmark.hh"

#include "sur/sur.hh"

class SURBenchmark : public Benchmark
{
public:
  virtual std::vector<Controls> execute(const std::vector<Controls>& fractional_controls,
                                        double scale_factor) override;

};

std::vector<Controls>
SURBenchmark::execute(const std::vector<Controls>& fractional_controls,
                        double scale_factor)
{
  return compute_sur_controls(fractional_controls);
}


int main(int argc, char** argv)
{
  SURBenchmark().execute_all(argc, argv);

  return 0;
}
