#include "scarp/scarp/scarp_program.hh"

#include "scarp/parameters.hh"

#include "test_fixture.hh"

using namespace scarp;

class SCARPProgramTest : public ProgramTest
{
private:
  const double scale_factor;

protected:
  virtual double execute(const Instance& instance,
                         const CostFunction& costs,
                         bool vanishing_constraints) override;

public:
  SCARPProgramTest(double scale_factor=1.0)
    : scale_factor(scale_factor)
  {}
};
