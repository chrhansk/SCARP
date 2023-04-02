#include "cost_function.hh"

#include <stdexcept>

double CostFunction::total_cost(const Controls& controls,
                                const Controls& fractional_controls) const
{
  const idx n = instance.num_cells();
  const idx m = instance.dimension();
  const Mesh& mesh = instance.get_mesh();

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
        fractional_control_sums.at(i) += mesh.cell_size(j) * fractional_controls(j, i);
      }
    };

  idx initial_control = control_at(0);

  add_fractional_controls(0);

  double total_cost = initial_costs(initial_control,
                                    fractional_control_sums);

  idx previous_control = initial_control;

  std::vector<idx> control_sums(m, 0);

  control_sums.at(initial_control) += mesh.cell_size(0);

  for(idx j = 1; j < n; ++j)
  {
    idx next_control = control_at(j);

    add_fractional_controls(j);

    total_cost = (*this)(Label(control_sums, previous_control, total_cost),
                         next_control,
                         j,
                         fractional_control_sums);

    control_sums.at(next_control) += mesh.cell_size(j);

    previous_control = next_control;
  }

  return final_costs(Label(control_sums, previous_control, total_cost));
}

double SwitchCosts::initial_costs(idx initial_control,
                                  const std::vector<double>& fractional_controls) const
{
  assert(fractional_controls.size() == instance.dimension());
  assert(initial_control < instance.dimension());

  return switch_on_costs.at(initial_control);
}

double SwitchCosts::final_costs(const Label& label) const
{
  assert(label.dimension() == instance.dimension());

  return label.get_cost() + switch_off_costs.at(label.get_current_control());
}

double SwitchCosts::operator()(const Label& previous_label,
                               idx next_control,
                               idx next_cell_index,
                               const std::vector<double>& fractional_control_sums) const
{
  assert(previous_label.dimension() == instance.dimension());
  assert(fractional_control_sums.size() == instance.dimension());
  assert(next_cell_index < instance.num_cells());
  assert(next_control < instance.dimension());

  const idx previous_control = previous_label.get_current_control();

  const double previous_cost = previous_label.get_cost();

  if(previous_control == next_control)
  {
    return previous_cost;
  }

  return previous_cost
    + switch_off_costs.at(previous_control)
    + switch_on_costs.at(next_control);
}

double SURCosts::initial_costs(idx initial_control,
                               const std::vector<double>& fractional_controls) const
{
  assert(fractional_controls.size() == instance.dimension());
  assert(initial_control < instance.dimension());

  const idx dimension = fractional_controls.size();
  const idx cell_size = instance.get_mesh().cell_size(0);

  double max_cost = std::abs(cell_size - fractional_controls.at(initial_control));

  for(idx control = 0; control < dimension; ++control)
  {
    if(control == initial_control)
    {
      continue;
    }

    const double curr_cost = fractional_controls.at(control);

    max_cost = std::max(max_cost, curr_cost);
  }

  return max_cost;
}

double SURCosts::final_costs(const Label& label) const
{
  assert(label.dimension() == instance.dimension());

  return label.get_cost();
}

double SURCosts::operator()(const Label& previous_label,
                            idx next_control,
                            idx next_cell_index,
                            const std::vector<double>& fractional_control_sums) const
{
  assert(previous_label.dimension() == instance.dimension());
  assert(fractional_control_sums.size() == instance.dimension());
  assert(next_cell_index < instance.num_cells());
  assert(next_control < instance.dimension());

  const idx dimension = fractional_control_sums.size();
  const idx cell_size = instance.get_mesh().cell_size(next_cell_index);

  double next_cost = previous_label.get_cost();

  for(idx control = 0; control < dimension; ++control)
  {
    double next_control_sum = previous_label.get_control_sums().at(control);

    if(control == next_control)
    {
      next_control_sum += cell_size;
    }

    const double fractional_control_sum = fractional_control_sums.at(control);

    const double current_control_cost = std::abs(next_control_sum - fractional_control_sum);

    next_cost = std::max(current_control_cost, next_cost);
  }

  return next_cost;
}
