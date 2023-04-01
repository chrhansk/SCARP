#ifndef ADAPTIVE_PROGRAM_HH
#define ADAPTIVE_PROGRAM_HH

#include "base_program.hh"

#include "controls.hh"
#include "parameters.hh"

#include "cost_function.hh"

#include "cmp.hh"
#include "log.hh"

template<class T>
class AdaptiveProgram : public BaseProgram<T>
{
private:

  using BaseProgram<T>::current_front;
  using BaseProgram<T>::next_front;
  using BaseProgram<T>::insert_label;

  using BaseProgram<T>::get_controls;
  using BaseProgram<T>::is_feasible;

  using BaseProgram<T>::num_labels;
  using BaseProgram<T>::num_expansions;

  using BaseProgram<T>::fractional_controls;
  using BaseProgram<T>::costs;

  using BaseProgram<T>::vanishing_constraints;
  using BaseProgram<T>::size;
  using BaseProgram<T>::dimension;

  using BaseProgram<T>::max_deviation;

  using BaseProgram<T>::fractional_sum;

  using BaseProgram<T>::iteration;

  using BaseProgram<T>::LabelSet;

  const std::vector<T> cell_volumes;

  void create_initial_labels() override;

  void expand_label(GenSCARPLabelPtr<T> label) override;

  void add_fractional_controls(idx k) override;

public:
  AdaptiveProgram(const Controls& fractional_controls,
                  const CostFunction& costs,
                  const std::vector<T>& cell_volumes,
                  double scale_factor = 1.,
                  bool vanishing_constraints = false);
};

template<class T>
AdaptiveProgram<T>::AdaptiveProgram(const Controls& fractional_controls,
                                    const CostFunction& costs,
                                    const std::vector<T>& cell_volumes,
                                    double scale_factor,
                                    bool vanishing_constraints)
  : BaseProgram<T>(fractional_controls, costs, scale_factor, vanishing_constraints),
    cell_volumes(cell_volumes)
{
  assert(fractional_controls.are_convex());
  assert(cell_volumes.size() == size);

  Log(debug) << "Max deviation: " << max_deviation;
}


template<class T>
void AdaptiveProgram<T>::create_initial_labels()
{
  iteration = 0;
  num_labels = 0;
  num_expansions = 0;

  current_front().clear();
  next_front().clear();

  idx num_initial_labels = 0;

  const T curr_volume = cell_volumes[0];

  for(idx i = 0; i < dimension; ++i)
  {
    const double initial_cost = costs.initial_costs(i, fractional_sum);

    auto label = std::make_shared<SCARPLabel>(i,
                                              dimension,
                                              initial_cost,
                                              curr_volume);

    if(is_feasible(*label))
    {
      next_front().push_back(typename BaseProgram<T>::LabelSet{label});
      ++num_initial_labels;
    }
    else
    {
      next_front().push_back(typename BaseProgram<T>::LabelSet());
    }

    current_front().push_back(typename BaseProgram<T>::LabelSet());
  }

  Log(debug) << "Created " << num_initial_labels << " initial labels";

  num_labels = num_initial_labels;

}

template<class T>
void AdaptiveProgram<T>::add_fractional_controls(idx k)
{
  for(idx i = 0; i < dimension; ++i)
  {
    fractional_sum.at(i) += cell_volumes[k] * fractional_controls(k, i);
  }
}

template<class T>
void AdaptiveProgram<T>::expand_label(GenSCARPLabelPtr<T> label)
{
  assert(is_feasible(get_controls(label)));

  const T next_volume = cell_volumes[iteration];

  for(idx next_control = 0; next_control < dimension; ++next_control)
  {
    double next_cost = costs(*label,
                             iteration,
                             next_control,
                             fractional_sum);

    auto next_label = std::make_shared<SCARPLabel>(next_control,
                                                   next_cost,
                                                   label,
                                                   next_volume);

    if(!is_feasible(*next_label))
    {
      continue;
    }

    ++num_expansions;

    assert(is_feasible(get_controls(next_label)));

    num_labels += insert_label(next_label, next_front().at(next_control));
  }
}

#endif /* ADAPTIVE_PROGRAM_HH */
