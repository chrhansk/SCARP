#include "cost_function.hh"

double SwitchCosts::initial_costs(idx initial_control,
                                  const std::vector<double>& fractional_controls) const
{
  return switch_on_costs.at(initial_control);
}

double SwitchCosts::final_costs(const Label& label) const
{
  return label.get_cost() + switch_off_costs.at(label.get_current_control());
}

double SwitchCosts::operator()(const Label& previous_label,
                               idx cell,
                               idx next_control,
                               const std::vector<double>& fractional_control_sums) const
{
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
  assert(fractional_controls.size() == dimension);

  double max_cost = std::abs(1. - fractional_controls.at(initial_control));

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
  return label.get_cost();
}

double SURCosts::operator()(const Label& previous_label,
                            idx cell,
                            idx next_control,
                            const std::vector<double>& fractional_control_sums) const
{
  const auto& control_sums = previous_label.get_control_sums();

  assert(dimension == control_sums.size());
  assert(dimension == fractional_control_sums.size());

  double next_cost = previous_label.get_cost();

  for(idx control = 0; control < dimension; ++control)
  {
    double next_control_sum = previous_label.get_control_sums().at(control);

    if(control == next_control)
    {
      ++next_control_sum;
    }

    const double fractional_control_sum = fractional_control_sums.at(control);

    const double current_control_cost = std::abs(next_control_sum - fractional_control_sum);

    next_cost = std::max(current_control_cost, next_cost);
  }

  return next_cost;
}
