#include "scarp/scarp_program_test.hh"

#include "scarp/bounds.hh"
#include "scarp/cost_function.hh"

double SCARPProgramTest::execute(const Instance& instance,
                                 const CostFunction& costs,
                                 bool vanishing_constraints)
{
  const FractionalControls& fractional_controls = instance.get_fractional_controls();


  const double max_deviation = bound_for(instance, vanishing_constraints);

  SCARPProgram program(instance,
                       costs,
                       max_deviation,
                       vanishing_constraints);

  auto controls = program.solve();

  const double distance = controls.distance(fractional_controls,
                                            instance.get_mesh());

  SURCosts sur_costs(instance);

  const double sur_cost = sur_costs.total_cost(controls, fractional_controls);

  assert(cmp::eq(distance, sur_cost));

  return costs.total_cost(controls, fractional_controls);
}

double SCARPProgramTest::bound_for(const Instance& instance,
                                   bool vanishing_constraints) const
{
  const idx dimension = instance.dimension();

  return bounds::sur(dimension, vanishing_constraints).for_mesh(instance.get_mesh(),
                                                                scale_factor);
}
