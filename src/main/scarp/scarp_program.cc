#include "scarp_program.hh"

#include <algorithm>
#include <sstream>

#include "cmp.hh"
#include "log.hh"


SCARPProgram::SCARPProgram(const std::vector<Controls>& fractional_controls,
                           const CostFunction& costs,
                           double scale_factor,
                           bool vanishing_constraints)
  : costs(costs),
    num_labels(0),
    num_expansions(0),
    fractional_controls(fractional_controls),
    vanishing_constraints(vanishing_constraints),
    size(fractional_controls.begin()->size()),
    dimension(fractional_controls.size()),
    max_deviation(scale_factor*max_control_deviation(dimension)),
    fractional_sum(dimension, 0.),
    iteration(0)
{
  assert(controls_are_convex(fractional_controls, 1e-3));

  Log(debug) << "Max deviation: " << max_deviation;
}

SCARPProgram::LabelFront&
SCARPProgram::current_front()
{
  return (iteration % 2) ? _next_labels : _current_labels;
}

SCARPProgram::LabelFront&
SCARPProgram::next_front()
{
  return (iteration % 2) ? _current_labels : _next_labels;
}

void SCARPProgram::create_initial_labels()
{
  iteration = 0;

  current_front().clear();
  next_front().clear();

  idx num_initial_labels = 0;

  for(idx i = 0; i < dimension; ++i)
  {
    const double initial_cost =
      costs.initial_costs(i, fractional_controls.at(i).at(0));

    next_front().push_back(LabelSet());

    auto label = std::make_shared<SCARPLabel>(i,
                                              dimension,
                                              initial_cost);

    if(is_feasible(*label))
    {
      next_front().rbegin()->insert(label);
      ++num_initial_labels;
    }

    current_front().push_back(LabelSet());
  }

  Log(debug) << "Created " << num_initial_labels << " initial labels";

  num_labels = num_initial_labels;

}

void SCARPProgram::expand_label(SCARPLabelPtr label)
{
  assert(is_feasible(get_controls(label)));

  for(idx next_control = 0; next_control < dimension; ++next_control)
  {
    double next_cost = costs(*label,
                             next_control,
                             fractional_sum.at(next_control));

    auto next_label = std::make_shared<SCARPLabel>(next_control,
                                                   next_cost,
                                                   label);

    if(!is_feasible(*next_label))
    {
      continue;
    }

    ++num_expansions;

    assert(is_feasible(get_controls(next_label)));

    auto it = next_front().at(next_control).find(next_label);

    if(it == next_front().at(next_control).end())
    {
      next_front().at(next_control).insert(next_label);

      ++num_labels;
    }
    else
    {
      if((*it)->get_cost() > next_label->get_cost())
      {
        (*it)->replace(*next_label);
      }
    }
  }
}

std::vector<Controls>
SCARPProgram::get_controls(SCARPLabelPtr label) const
{
  std::vector<Controls> controls(dimension, Controls{});

  std::vector<idx> control_values;

  while(label)
  {
    control_values.push_back(label->get_current_control());
    label = label->get_predecessor();
  }

  std::reverse(std::begin(control_values), std::end(control_values));

  for(const auto& value : control_values)
  {
    assert(value >= 0);
    assert(value < dimension);

    for(idx i = 0; i < dimension; ++i)
    {
      controls[i].push_back(value == i);
    }
  }

  return controls;
}

void SCARPProgram::expand_labels()
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
}

bool SCARPProgram::is_feasible(const SCARPLabel& label) const
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

    double control_value = fractional_controls.at(i).at(iteration);

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

bool SCARPProgram::is_feasible(const std::vector<Controls>& actual_controls) const
{
  return cmp::le(control_distance(actual_controls, fractional_controls), max_deviation) &&
    controls_are_integral(actual_controls) &&
    controls_are_convex(actual_controls);
}

void SCARPProgram::expand_all()
{
  Log(info) << "Computing SCARP controls for size: "
            << size
            << ", dimension: "
            << dimension
            << ", VC? "
            << std::boolalpha
            << vanishing_constraints;


  for(idx i = 0; i < dimension; ++i)
  {
    fractional_sum.at(i) = fractional_controls[i][0];
  }

  create_initial_labels();

  for(iteration = 1; iteration < size; ++iteration)
  {
    Log(debug) << "Performing iteration " << iteration;

    for(idx i = 0; i < dimension; ++i)
    {
      fractional_sum.at(i) += fractional_controls[i][iteration];
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
}

std::vector<Controls> SCARPProgram::solve()
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

  auto min_controls = get_controls(min_label);

  assert(dimension == min_controls.size());
  assert(size == min_controls.front().size());

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
      assert(controls_satisfy_vanishing_constraints(min_controls, fractional_controls));
    }

    const double total_costs = costs.total_cost(min_controls, fractional_controls);

    Log(debug) << "Costs according to controls: " << total_costs;

    assert(cmp::eq(total_costs, min_costs));
  }

  return min_controls;
}

std::vector<std::vector<Controls>> SCARPProgram::solve_all()
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

  std::vector<std::vector<Controls>> all_controls;

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
          assert(controls_satisfy_vanishing_constraints(min_controls, fractional_controls));
        }
      }


      assert(dimension == min_controls.size());
      assert(size == min_controls.front().size());

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
