#ifndef SCARP_COST_FUNCTION_HH
#define SCARP_COST_FUNCTION_HH

#include <cassert>

#include "controls.hh"
#include "instance.hh"
#include "label.hh"

namespace scarp
{

class CostFunction
{
protected:
  const Instance& instance;
public:
  CostFunction(const Instance& instance)
    : instance(instance)
  {}

  virtual double initial_costs(idx initial_control,
                               const std::vector<double>& fractional_controls) const = 0;

  virtual double final_costs(const Label& label) const = 0;

  // sum includes next_control
  virtual double operator()(const Label& previous_label,
                            idx next_control,
                            idx next_cell_index,
                            const std::vector<double>& fractional_control_sums) const = 0;

  double total_cost(const Controls& controls,
                    const Controls& fractional_controls) const;

};

class SwitchCosts : public CostFunction
{
private:
  const std::vector<double> switch_on_costs;
  const std::vector<double> switch_off_costs;
public:
  SwitchCosts(const Instance& instance,
              const std::vector<double>& switch_on_costs,
              const std::vector<double>& switch_off_costs)
    : CostFunction(instance),
      switch_on_costs(switch_on_costs),
      switch_off_costs(switch_off_costs)
  {}

  double initial_costs(idx initial_control,
                       const std::vector<double>& fractional_controls) const override;

  double final_costs(const Label& label) const override;

  double operator()(const Label& previous_label,
                    idx next_control,
                    idx next_cell_index,
                    const std::vector<double>& fractional_control_sums) const override;
};

class SURCosts : public CostFunction
{
public:
  SURCosts(const Instance& instance)
    : CostFunction(instance)
  {}

  double initial_costs(idx initial_control,
                       const std::vector<double>& fractional_controls) const override;

  double final_costs(const Label& label) const override;

  double operator()(const Label& previous_label,
                    idx next_control,
                    idx next_cell_index,
                    const std::vector<double>& fractional_control_sums) const override;
};

} // namespace scarp

#endif /* SCARP_COST_FUNCTION_HH */
