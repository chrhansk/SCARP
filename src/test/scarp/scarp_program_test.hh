#include "scarp/scarp/scarp_program.hh"

#include "scarp/parameters.hh"

#include "test_fixture.hh"

using namespace scarp;

class SCARPProgramTest : public ProgramTest
{
protected:
  const double scale_factor;

  virtual double execute(const Instance& instance,
                         const CostFunction& costs,
                         bool vanishing_constraints) override;

  virtual double bound_for(const Instance& instance,
                           bool vanishing_constraints) const;

public:
  SCARPProgramTest(double scale_factor=1.0)
    : scale_factor(scale_factor)
  {}
};
