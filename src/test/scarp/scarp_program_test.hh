#include "scarp/scarp_program.hh"

#include "parameters.hh"

#include "test_fixture.hh"

class SCARPProgramTest : public ProgramTest
{
private:
  const double scale_factor;

protected:
  virtual double execute(const Instance& instance,
                         const CostFunction& costs) override;

public:
  SCARPProgramTest(double scale_factor=1.0)
    : scale_factor(scale_factor)
  {}
};
