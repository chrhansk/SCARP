#include "cost_function.hh"

#include <stdexcept>

double CostFunction::total_cost(const std::vector<Controls>& controls,
                                const std::vector<Controls>& fractional_controls) const
{
  const idx size = fractional_controls.front().size();
  const idx dimension = fractional_controls.size();

  std::vector<double> fractional_control_sums(dimension, 0.);

  auto control_at = [&](idx j) -> idx
    {
      for(idx i = 0; i < dimension; ++i)
      {
        if(controls.at(i).at(j) == 1)
        {
          return i;
        }
      }

      throw std::invalid_argument("Controls are not integral");

      return 0;
    };

  auto add_fractional_controls = [&](idx j)
    {
      for(idx i = 0; i < dimension; ++i)
      {
        fractional_control_sums.at(i) += fractional_controls.at(i).at(j);
      }
    };

  idx initial_control = control_at(0);

  double total_cost = initial_costs(initial_control,
                                    fractional_controls.at(initial_control).at(0));

  idx previous_control = initial_control;

  std::vector<idx> control_sums(dimension, 0);

  control_sums.at(initial_control)++;

  add_fractional_controls(0);

  for(idx j = 1; j < size; ++j)
  {
    idx next_control = control_at(j);

    add_fractional_controls(j);

    total_cost = (*this)(Label(control_sums, previous_control, total_cost),
                         next_control,
                         fractional_control_sums.at(next_control));

    control_sums.at(next_control)++;

    previous_control = next_control;
  }

  return final_costs(Label(control_sums, previous_control, total_cost));
}
