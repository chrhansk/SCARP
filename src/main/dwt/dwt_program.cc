#include "dwt_program.hh"

#include "log.hh"

DWTProgram::DWTProgram(const std::vector<Controls>& fractional_controls,
                       const std::vector<double>& switch_on_costs,
                       const std::vector<double>& switch_off_costs,
                       const std::vector<idx>& minimum_dwt,
                       double scale_factor)
  : fractional_controls(fractional_controls),
    switch_on_costs(switch_on_costs),
    switch_off_costs(switch_off_costs),
    size(fractional_controls.begin()->size()),
    dimension(fractional_controls.size()),
    minimum_dwt(minimum_dwt),
    max_deviation(scale_factor*max_control_deviation(dimension)),
    fractional_sum(dimension, 0.),
    iteration(0)
{

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
    double cost = switch_on_costs.at(i);

    next_front().push_back(LabelMap());

    idx remaining_dwt = minimum_dwt.at(i);

    auto label = std::make_shared<DWTLabel>(i,
                                            dimension,
                                            cost,
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

void DWTProgram::expand_label(DWTLabelPtr label)
{
  for(idx next_control = 0; next_control < dimension; ++next_control)
  {
    double cost_increase = 0.;
    idx remaining_dwt = label->get_remaining_dwt();

    if(label->get_current_control() != next_control)
    {
      if(label->get_remaining_dwt() > 0)
      {
        continue;
      }

      remaining_dwt = minimum_dwt.at(next_control);

      cost_increase = switch_on_costs.at(next_control) +
        switch_off_costs.at(label->get_current_control());
    }
    else
    {
      if(remaining_dwt > 0)
      {
        --remaining_dwt;
      }
    }

    auto next_label = std::make_shared<DWTLabel>(next_control,
                                                 cost_increase,
                                                 label,
                                                 remaining_dwt);


    if(!is_feasible(*next_label))
    {
      continue;
    }

    auto& next_map = next_front().at(next_control);

    const auto& control_sums = next_label->get_control_sums();

    auto it = next_map.find(control_sums);

    if(it == next_map.end())
    {
      next_map.insert(std::make_pair(control_sums, LabelSet(next_label)));
    }
    else
    {
      auto& label_set = it->second;

      label_set.insert(next_label);
    }
  }
}

std::vector<Controls>
DWTProgram::get_controls(DWTLabelPtr label) const
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

std::vector<Controls> DWTProgram::solve()
{
  for(idx i = 0; i < dimension; ++i)
  {
    fractional_sum.at(i) = fractional_controls[i][0];
  }

  create_initial_labels();

  for(iteration = 1; iteration < size; ++iteration)
  {
    Log(info) << "Performing iteration " << iteration;

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

  double min_cost = inf;
  DWTLabelPtr min_label;

  for(idx i = 0; i < dimension; ++i)
  {
    for(const auto& pair : current_front().at(i))
    {
      const auto& label_set = pair.second;

      for(const auto& label : label_set.get_labels())
      {

        double total_cost = label->get_cost() +
          switch_off_costs.at(label->get_current_control());

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

  Log(info) << "Costs according to controls: " << control_costs(min_controls,
                                                                switch_on_costs,
                                                                switch_off_costs);

  return min_controls;
}
