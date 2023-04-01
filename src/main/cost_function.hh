#ifndef COST_FUNCTION_HH
#define COST_FUNCTION_HH

#include <cassert>
#include <stdexcept>

#include "controls.hh"
#include "label.hh"

template<class T>
class GenCostFunction
{
public:
  virtual double initial_costs(idx initial_control,
                               const std::vector<double>& fractional_controls) const = 0;

  virtual double final_costs(const GenLabel<T>& label) const = 0;

  // sum includes next_control
  virtual double operator()(const GenLabel<T>& previous_label,
                            idx cell,
                            idx next_control,
                            const std::vector<double>& fractional_control_sums) const = 0;

  double total_cost(const Controls& controls,
                    const Controls& fractional_controls) const;

  virtual ~GenCostFunction() {}
};

template<class T>
double GenCostFunction<T>::total_cost(const Controls& controls,
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

  idx previous_control = initial_control;

  std::vector<T> control_sums(m, 0);

  control_sums.at(initial_control)++;

  add_fractional_controls(0);

  double total_cost = initial_costs(initial_control,
                                    fractional_control_sums);

  for(idx j = 1; j < n; ++j)
  {
    idx next_control = control_at(j);

    add_fractional_controls(j);

    total_cost = (*this)(Label(control_sums, previous_control, total_cost),
                         j,
                         next_control,
                         fractional_control_sums);

    control_sums.at(next_control)++;

    previous_control = next_control;
  }

  return final_costs(Label(control_sums, previous_control, total_cost));
}

typedef GenCostFunction<idx> CostFunction;

template<class T>
class WeightedCostFunction : public GenCostFunction<T>
{
private:
  const GenCostFunction<T>& original_costs;
  const std::vector<T> weights;
public:
  WeightedCostFunction(const GenCostFunction<T>& original_costs,
                       const std::vector<T>& weights)
    : original_costs(original_costs),
      weights(weights)
  {}

  double initial_costs(idx initial_control,
                       const std::vector<double>& fractional_controls) const override;

  double final_costs(const GenLabel<T>& label) const override;

  double operator()(const GenLabel<T>& previous_label,
                    idx cell,
                    idx next_control,
                    const std::vector<double>& fractional_control_sums) const override;
};


template<class T>
double WeightedCostFunction<T>::initial_costs(idx initial_control,
                                              const std::vector<double>& fractional_controls) const
{
  return weights[0] * original_costs.initial_costs(initial_control,
                                                   fractional_controls);
}

template<class T>
double WeightedCostFunction<T>::final_costs(const GenLabel<T>& label) const
{
  return weights.back() * original_costs.final_costs(label);
}

template<class T>
double WeightedCostFunction<T>::operator()(const GenLabel<T>& previous_label,
                                           idx cell,
                                           idx next_control,
                                           const std::vector<double>& fractional_control_sums) const
{
  assert(cell < weights.size());

  return weights[cell] * original_costs(previous_label,
                                        cell,
                                        next_control,
                                        fractional_control_sums);
}

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
                       const std::vector<double>& fractional_controls) const override;

  double final_costs(const Label& label) const override;

  double operator()(const Label& previous_label,
                    idx cell,
                    idx next_control,
                    const std::vector<double>& fractional_control_sums) const override;
};

class SURCosts : public CostFunction
{
private:
  const idx dimension;
public:

  SURCosts(idx dimension)
    : dimension(dimension)
  {}
  
  double initial_costs(idx initial_control,
                       const std::vector<double>& fractional_controls) const override;

  double final_costs(const Label& label) const override;

  double operator()(const Label& previous_label,
                    idx cell,
                    idx next_control,
                    const std::vector<double>& fractional_control_sums) const override;
};

#endif /* COST_FUNCTION_HH */
