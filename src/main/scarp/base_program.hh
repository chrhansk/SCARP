#ifndef BASE_PROGRAM_HH
#define BASE_PROGRAM_HH


#include <algorithm>
#include <set>
#include <unordered_set>

#include "log.hh"
#include "scarp_label.hh"

#include "controls.hh"
#include "parameters.hh"

#include "cost_function.hh"

template<class T>
class BaseProgram
{
public:
  typedef std::unordered_set<GenSCARPLabelPtr<T>,
                             GenSCARPLabelHash<T>,
                             GenSCARPLabelComparator<T>> LabelSet;

  //typedef std::set<LabelPtr, LabelOrdering> LabelSet;
  typedef std::vector<LabelSet> LabelFront;

private:
  LabelFront _current_labels, _next_labels;
protected:

  idx num_labels, num_expansions;

  const GenCostFunction<T>& costs;
  const Controls& fractional_controls;

  const bool vanishing_constraints;

  const idx size, dimension;

  const double max_deviation;

  std::vector<double> fractional_sum;

  idx iteration;

  BinaryControls get_controls(GenSCARPLabelPtr<T> label) const;

  LabelFront& current_front()
  {
    return (iteration % 2) ? _next_labels : _current_labels;
  }

  LabelFront& next_front()
  {
    return (iteration % 2) ? _current_labels : _next_labels;
  }

  virtual void expand_label(GenSCARPLabelPtr<T> label) = 0;

  virtual void add_fractional_controls(idx k) = 0;

  virtual void create_initial_labels() = 0;

  void expand_all();

  void expand_labels();

  bool is_feasible(const GenSCARPLabel<T>& label) const;

  bool insert_label(GenSCARPLabelPtr<T> label,
                    LabelSet& label_set) const;

  bool is_feasible(const BinaryControls& actual_controls) const;
public:
  BaseProgram(const Controls& fractional_controls,
              const GenCostFunction<T>& costs,
              double scale_factor = 1.,
              bool vanishing_constraints = false)
    : num_labels(0),
      num_expansions(0),
      costs(costs),
      fractional_controls(fractional_controls),
      vanishing_constraints(vanishing_constraints),
      size(fractional_controls.num_cells()),
      dimension(fractional_controls.dimension()),
      max_deviation(scale_factor*max_control_deviation(dimension)),
      fractional_sum(dimension, 0.),
      iteration(0)
  {}

  BinaryControls solve();

  std::vector<BinaryControls> solve_all();
};

template<class T>
void BaseProgram<T>::expand_all()
{
  Log(info) << "Computing SCARP controls for size: "
            << size
            << ", dimension: "
            << dimension
            << ", VC? "
            << std::boolalpha
            << vanishing_constraints;

  add_fractional_controls(0);

  create_initial_labels();

  for(iteration = 1; iteration < size; ++iteration)
  {
    Log(debug) << "Performing iteration " << iteration;

    add_fractional_controls(iteration);
    expand_labels();
  }
}

template<class T>
BinaryControls BaseProgram<T>::solve()
{
  expand_all();

  double min_costs = inf;
  SCARPLabelPtr min_label;

  for(idx i = 0; i < dimension; ++i)
  {
    for(const auto& label : current_front().at(i))
    {
      double total_cost = costs.final_costs(*label);

      if(total_cost < min_costs)
      {
        min_costs = total_cost;
        min_label = label;
      }
    }
  }

  if(!min_label)
  {
    throw std::logic_error("Instance is infeasible");
  }

  auto min_controls = get_controls(min_label);

  assert(dimension == min_controls.dimension());
  assert(size == min_controls.num_cells());

  assert(is_feasible(min_controls));

  Log(info) << "Created " << num_labels
            << " labels during "
            << num_expansions
            << " expansions";

  Log(info) << "Optimal control costs: " << min_costs;

  if(debugging_enabled())
  {
    if(vanishing_constraints)
    {
      assert(min_controls.satisfy_vanishing_constraints(fractional_controls));
    }

    const double total_costs = costs.total_cost(min_controls, fractional_controls);

    Log(debug) << "Costs according to controls: " << total_costs;

    assert(cmp::eq(total_costs, min_costs));
  }

  return min_controls;
}

template<class T>
std::vector<BinaryControls> BaseProgram<T>::solve_all()
{
  expand_all();

  double min_costs = inf;

  for(idx i = 0; i < dimension; ++i)
  {
    for(const auto& label : current_front().at(i))
    {
      double total_cost = costs.final_costs(*label);

      if(total_cost < min_costs)
      {
        min_costs = total_cost;
      }
    }
  }

  std::vector<BinaryControls> all_controls;

  for (idx i = 0; i < dimension; ++i)
  {
    for (const auto &label : current_front().at(i))
    {
      double total_cost = costs.final_costs(*label);

      if(!cmp::eq(total_cost, min_costs))
      {
        continue;
      }

      auto min_controls = get_controls(label);

      if(debugging_enabled())
      {
        if(vanishing_constraints)
        {
          assert(min_controls.satisfy_vanishing_constraints(fractional_controls));
        }
      }


      assert(dimension == min_controls.dimension());
      assert(size == min_controls.num_cells());

      assert(is_feasible(min_controls));

      all_controls.push_back(min_controls);
    }
  }

  Log(info) << "Found "
            << all_controls.size()
            << " optimal controls";

  Log(info) << "Created " << num_labels
            << " labels during "
            << num_expansions
            << " expansions";

  return all_controls;
}

template<class T>
BinaryControls BaseProgram<T>::get_controls(GenSCARPLabelPtr<T> label) const
{
  std::vector<idx> control_values;
  control_values.reserve(size);

  while(label)
  {
    control_values.push_back(label->get_current_control());
    label = label->get_predecessor();
  }

  std::reverse(control_values.begin(), control_values.end());

  return BinaryControls(control_values,
                        dimension);
}

template<class T>
void BaseProgram<T>::expand_labels()
{
  for(idx i = 0; i < dimension; ++i)
  {
    next_front().at(i).clear();
  }

  for(idx i = 0; i < dimension; ++i)
  {
    for(const auto& label : current_front().at(i))
    {
      expand_label(label);
    }
  }

  if(debugging_enabled())
  {
    idx curr_num_labels = 0;

    for(idx i = 0; i < dimension; ++i)
    {
      curr_num_labels += next_front().at(i).size();
    }

    Log(debug) << "Number of active labels: " << curr_num_labels;
  }
}

template<class T>
bool BaseProgram<T>::is_feasible(const GenSCARPLabel<T>& label) const
{
  if(debugging_enabled())
  {
    double fraction_total = 0.;

    for(const auto& value : fractional_sum)
    {
      fraction_total += value;
    }

    double label_total = 0.;

    for(const auto& value : label.get_control_sums())
    {
      label_total += value;
    }

    assert(cmp::eq(fraction_total, label_total, 1e-3));

    if(label.get_predecessor())
    {
      double pred_total = 0.;

      for(const auto& value : label.get_predecessor()->get_control_sums())
      {
        pred_total += value;
      }

      assert(label_total - pred_total == 1);
    }

  }

  if(vanishing_constraints)
  {
    const idx i = label.get_current_control();

    double control_value = fractional_controls(iteration, i);

    if(cmp::zero(control_value))
    {
      return false;
    }
  }

  for(idx i = 0; i < dimension; ++i)
  {
    double deviation = std::fabs(label.get_control_sums().at(i) - fractional_sum.at(i));

    if(deviation > max_deviation)
    {
      return false;
    }
  }

  return true;
}

template<class T>
bool BaseProgram<T>::is_feasible(const BinaryControls& actual_controls) const
{
  if (!(actual_controls.are_integral() && actual_controls.are_convex()))
  {
    return false;
  }

  const idx curr_num_cells = actual_controls.num_cells();

  FractionalControls partial_controls(curr_num_cells,
                                      dimension);

  for(idx k = 0; k < curr_num_cells; ++k)
  {
    for(idx i = 0; i < dimension; ++i)
    {
      partial_controls(k, i) = fractional_controls(k, i);
    }
  }

  if(vanishing_constraints)
  {
    assert(actual_controls.satisfy_vanishing_constraints(partial_controls));
  }

  return cmp::le(actual_controls.distance(partial_controls), max_deviation);
}

template<class T>
bool BaseProgram<T>::insert_label(GenSCARPLabelPtr<T> label,
                                  LabelSet& label_set) const
{
  auto it = label_set.find(label);

  if(it == label_set.end())
  {
    label_set.insert(label);

    return true;
  }
  else
  {
    if((*it)->get_cost() > label->get_cost())
    {
      (*it)->replace(*label);
    }
  }

  return false;
}

#endif /* BASE_PROGRAM_HH */
