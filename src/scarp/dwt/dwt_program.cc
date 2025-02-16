#include "dwt_program.hh"

#include <boost/container_hash/hash.hpp>
#include <cassert>

#include "scarp/error.hh"
#include "scarp/log.hh"

namespace scarp
{

DWTProgram::DWTProgram(const Controls &fractional_controls,
                       const CostFunction &costs,
                       const std::vector<idx> &minimum_dwt,
                       double max_deviation,
                       bool vanishing_constraints)
    : costs(costs), fractional_controls(fractional_controls),
      size(fractional_controls.num_cells()),
      dimension(fractional_controls.dimension()),
      minimum_dwt(minimum_dwt),
      max_deviation(max_deviation),
      vanishing_constraints(vanishing_constraints),
      fractional_sum(dimension, 0.),
      iteration(0)
{}


std::size_t DWTProgram::ControlSumsHash::operator()(const DWTProgram::ControlSums& control_sums) const
{
  std::size_t seed = 0;

  boost::hash_combine(seed,
                      boost::hash_range(control_sums.begin(), control_sums.end()));

  return seed;
}

DWTProgram::LabelFront&
DWTProgram::current_front()
{
  return (iteration % 2) ? _next_labels : _current_labels;
}

DWTProgram::LabelFront&
DWTProgram::next_front()
{
  return (iteration % 2) ? _current_labels : _next_labels;
}

bool DWTProgram::is_feasible(const DWTLabel& label) const
{
  for(idx i = 0; i < dimension; ++i)
  {
    double deviation = std::fabs(label.get_control_sums().at(i) - fractional_sum.at(i));

    if(deviation > max_deviation)
    {
      return false;
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

  return true;
}

void DWTProgram::create_initial_labels()
{
  iteration = 0;

  current_front().clear();
  next_front().clear();

  idx num_labels = 0;

  for(idx i = 0; i < dimension; ++i)
  {
    current_front().push_back(LabelMap());

    const double initial_cost = costs.initial_costs(i, fractional_sum);

    next_front().push_back(LabelMap());

    idx remaining_dwt = minimum_dwt.at(i);

    auto label = std::make_shared<DWTLabel>(i,
                                            dimension,
                                            initial_cost,
                                            remaining_dwt);

    if(is_feasible(*label))
    {
      next_front().rbegin()->insert(std::make_pair(label->get_control_sums(),
                                                   LabelSet(label)));
      ++num_labels;
    }

    current_front().push_back(LabelMap());
  }

  Log(debug) << "Created " << num_labels << " initial labels";
}

void DWTProgram::expand_labels()
{
  for(idx i = 0; i < dimension; ++i)
  {
    next_front().at(i).clear();
  }

  for(idx i = 0; i < dimension; ++i)
  {
    for(const auto& pair : current_front().at(i))
    {
      const auto& label_set = pair.second;

      for(const auto& label: label_set.get_labels())
      {
        expand_label(label);
      }
    }
  }
}

void DWTProgram::insert_label_into_next_front(DWTLabelPtr label)
{
  if(!is_feasible(*label))
  {
    return;
  }

  idx control = label->get_current_control();
  LabelMap& next_map = next_front().at(control);

  const auto& control_sums = label->get_control_sums();

  auto it = next_map.find(control_sums);

  bool inserted = false;

  if(it == next_map.end())
  {
    next_map.insert(std::make_pair(control_sums, LabelSet(label)));
    inserted = true;
  }
  else
  {
    auto& label_set = it->second;

    inserted = label_set.insert(label);
  }
}

void DWTProgram::expand_label(DWTLabelPtr label)
{
  if(label->get_remaining_dwt() > 0)
  {
    double next_control = label->get_current_control();

    double next_cost = costs(*label,
                             next_control,
                             iteration,
                             fractional_sum);

    idx remaining_dwt = label->get_remaining_dwt() -1;

    auto next_label = std::make_shared<DWTLabel>(next_control,
                                                 next_cost,
                                                 label,
                                                 remaining_dwt);

    insert_label_into_next_front(next_label);

    return;
  }

  assert(label->get_remaining_dwt() == 0);

  for(idx next_control = 0; next_control < dimension; ++next_control)
  {
    idx remaining_dwt = 0;

    if(label->get_current_control() != next_control)
    {
      remaining_dwt = minimum_dwt.at(next_control);
    }

    double next_cost = costs(*label,
                             next_control,
                             iteration,
                             fractional_sum);

    assert(next_cost - label->get_cost() <= 5.0);

    auto next_label = std::make_shared<DWTLabel>(next_control,
                                                 next_cost,
                                                 label,
                                                 remaining_dwt);

    assert(next_label->get_cost() == next_cost);

    insert_label_into_next_front(next_label);
  }
}

BinaryControls
DWTProgram::get_controls(DWTLabelPtr label) const
{
  std::vector<idx> control_values;

  while(label)
  {
    control_values.push_back(label->get_current_control());
    label = label->get_predecessor();
  }

  std::reverse(std::begin(control_values), std::end(control_values));


  return BinaryControls(control_values, dimension);
}

BinaryControls DWTProgram::solve()
{
  for(idx i = 0; i < dimension; ++i)
  {
    fractional_sum.at(i) = fractional_controls(0, i);
  }

  create_initial_labels();

  for(iteration = 1; iteration < size; ++iteration)
  {
    Log(info) << "Performing iteration " << iteration;

    for(idx i = 0; i < dimension; ++i)
    {
      fractional_sum.at(i) += fractional_controls(iteration, i);
    }

    expand_labels();

    if(debugging_enabled())
    {
      idx num_labels = 0;

      for(idx i = 0; i < dimension; ++i)
      {
        num_labels += next_front().at(i).size();
      }

      Log(debug) << "Created " << num_labels << " labels";
    }
  }

  double min_cost = inf;
  DWTLabelPtr min_label;

  for(idx i = 0; i < dimension; ++i)
  {
    for(const auto& pair : current_front().at(i))
    {
      const auto& label_set = pair.second;

      for(const auto& label : label_set.get_labels())
      {
        double total_cost = costs.final_costs(*label);

        if(total_cost < min_cost)
        {
          min_cost = total_cost;
          min_label = label;
        }
      }
    }
  }

  if(!min_label)
  {
    throw InfeasibleException();
  }

  auto min_controls = get_controls(min_label);

  Log(info) << "Min cost: " << min_cost;

  Log(info) << "Costs according to controls: " << costs.total_cost(min_controls,
                                                                   fractional_controls);

  return min_controls;
}

} // namespace scarp
