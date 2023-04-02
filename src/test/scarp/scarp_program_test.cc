#include "scarp_program_test.hh"

double SCARPProgramTest::execute(const Instance& instance,
                                 const CostFunction& costs,
                                 bool vanishing_constraints)
{
  const FractionalControls& fractional_controls = instance.get_fractional_controls();
  const idx dimension = fractional_controls.dimension();

  SCARPProgram program(instance,
                       costs,
                       scale_factor,
                       vanishing_constraints);

  auto controls = program.solve();

  return costs.total_cost(controls, fractional_controls);
}