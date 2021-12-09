#ifndef COST_FUNCTION_HH
#define COST_FUNCTION_HH

#include <cassert>

#include "controls.hh"
#include "label.hh"

class CostFunction
{
public:
  virtual double initial_costs(idx initial_control,
                               double fractional_control) const = 0;

  virtual double final_costs(const Label& label) const = 0;

  // sum includes next_control
  virtual double operator()(const Label& previous_label,
                            idx next_control,
                            const std::vector<double>& fractional_control_sums) const = 0;

  double total_cost(const std::vector<Controls>& controls,
                    const std::vector<Controls>& fractional_controls) const;

};

class SwitchCosts : public CostFunction
{
private:
  const std::vector<double> switch_on_costs;
  const std::vector<double> switch_off_costs;
public:
  SwitchCosts(const std::vector<double>& switch_on_costs,
              const std::vector<double>& switch_off_costs)
    : switch_on_costs(switch_on_costs),
      switch_off_costs(switch_off_costs)
  {}

  double initial_costs(idx initial_control,
                       double fractional_control) const override
  {
    return switch_on_costs.at(initial_control);
  }

  double final_costs(const Label& label) const override
  {
    return label.get_cost() + switch_off_costs.at(label.get_current_control());
  }

  double operator()(const Label& previous_label,
                    idx next_control,
                    const std::vector<double>& fractional_control_sums) const override
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
};

class SURCosts : public CostFunction
{
  double initial_costs(idx initial_control,
                       double fractional_control) const override
  {
    return std::abs(1. - fractional_control);
  }

  double final_costs(const Label& label) const override
  {
    return label.get_cost();
  }

  double operator()(const Label& previous_label,
                    idx next_control,
                    const std::vector<double>& fractional_control_sums) const override
  {
    const idx num_controls = previous_label.get_control_sums().size();
    assert(fractional_control_sums.size() == num_controls);

    double next_cost = previous_label.get_cost();

    for(idx control = 0; control < num_controls; ++control)
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
};

#endif /* COST_FUNCTION_HH */
