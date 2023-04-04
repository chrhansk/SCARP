#include "scarp/scarp_program_test.hh"

#include "scarp/cost_function.hh"

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

  const double distance = controls.distance(fractional_controls,
                                            instance.get_mesh());

  SURCosts sur_costs(instance);

  const double sur_cost = sur_costs.total_cost(controls, fractional_controls);

  assert(cmp::eq(distance, sur_cost));

  return costs.total_cost(controls, fractional_controls);
}
