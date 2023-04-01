#include "uniform_program.hh"

#include "cmp.hh"
#include "log.hh"

UniformProgram::UniformProgram(const Controls& fractional_controls,
                               const CostFunction& costs,
                               double scale_factor,
                               bool vanishing_constraints)
  : BaseProgram(fractional_controls, costs, scale_factor, vanishing_constraints)
{
  assert(fractional_controls.are_convex());

  Log(debug) << "Max deviation: " << max_deviation;
}

void UniformProgram::create_initial_labels()
{
  iteration = 0;
  num_labels = 0;
  num_expansions = 0;

  current_front().clear();
  next_front().clear();

  idx num_initial_labels = 0;

  for(idx i = 0; i < dimension; ++i)
  {
    const double initial_cost = costs.initial_costs(i, fractional_sum);

    current_front().push_back(LabelSet());

    auto label = std::make_shared<SCARPLabel>(i,
                                              dimension,
                                              initial_cost);

    if(is_feasible(*label))
    {
      next_front().push_back(LabelSet{label});
      ++num_initial_labels;
    }
    else
    {
      next_front().push_back(LabelSet());
    }
  }

  Log(debug) << "Created " << num_initial_labels << " initial labels";

  num_labels = num_initial_labels;
}

void UniformProgram::expand_label(SCARPLabelPtr label)
{
  assert(is_feasible(get_controls(label)));

  for(idx next_control = 0; next_control < dimension; ++next_control)
  {
    double next_cost = costs(*label,
                             iteration,
                             next_control,
                             fractional_sum);

    auto next_label = std::make_shared<SCARPLabel>(next_control,
                                                   next_cost,
                                                   label);

    if(!is_feasible(*next_label))
    {
      continue;
    }

    ++num_expansions;

    assert(is_feasible(get_controls(next_label)));

    num_labels += insert_label(next_label, next_front().at(next_control));
  }
}

void UniformProgram::add_fractional_controls(idx k)
{
  assert(k >= 0);
  assert(k < size);

  for(idx i = 0; i < dimension; ++i)
  {
    fractional_sum.at(i) += fractional_controls(k, i);
  }
}
