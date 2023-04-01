#include "cost_function.hh"

#include <stdexcept>

double CostFunction::total_cost(const Controls& controls,
                                const Controls& fractional_controls) const
{
  const idx n = controls.num_cells();
  const idx m = controls.dimension();

  std::vector<double> fractional_control_sums(m, 0.);

  auto control_at = [&](idx j) -> idx
    {
      for(idx i = 0; i < m; ++i)
      {
        if(controls(j, i) == 1.)
        {
          return i;
        }
      }

      throw std::invalid_argument("Controls are not integral");

      return 0;
    };

  auto add_fractional_controls = [&](idx j)
    {
      for(idx i = 0; i < m; ++i)
      {
        fractional_control_sums.at(i) += fractional_controls(j, i);
      }
    };

  idx initial_control = control_at(0);

  double total_cost = initial_costs(initial_control,
                                    fractional_controls(0, initial_control));

  idx previous_control = initial_control;

  std::vector<idx> control_sums(m, 0);

  control_sums.at(initial_control)++;

  add_fractional_controls(0);

  for(idx j = 1; j < n; ++j)
  {
    idx next_control = control_at(j);

    add_fractional_controls(j);

    total_cost = (*this)(Label(control_sums, previous_control, total_cost),
                         next_control,
                         j,
                         fractional_control_sums);

    control_sums.at(next_control)++;

    previous_control = next_control;
  }

  return final_costs(Label(control_sums, previous_control, total_cost));
}
