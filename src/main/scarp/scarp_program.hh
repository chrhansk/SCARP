#ifndef SCARP_PROGRAM_HH
#define SCARP_PROGRAM_HH

#include <memory>

#include <set>
#include <unordered_set>

#include "scarp_label.hh"

#include "controls.hh"
#include "parameters.hh"

#include "cost_function.hh"

class SCARPProgram
{
public:
  typedef std::unordered_set<SCARPLabelPtr,
                             SCARPLabelHash,
                             SCARPLabelComparator> LabelSet;

  //typedef std::set<LabelPtr, LabelOrdering> LabelSet;
  typedef std::vector<LabelSet> LabelFront;
private:
  const CostFunction& costs;
  LabelFront _current_labels, _next_labels;

  idx num_labels, num_expansions;

  const std::vector<Controls>& fractional_controls;

  bool vanishing_constraints;

  const idx size, dimension;

  double max_deviation;

  std::vector<double> fractional_sum;

  idx iteration;

  void create_initial_labels();

  void expand_labels();

  bool is_feasible(const SCARPLabel& label) const;

  bool is_feasible(const std::vector<Controls>& actual_controls) const;

  void expand_label(SCARPLabelPtr label);

  std::vector<Controls> get_controls(SCARPLabelPtr label) const;

  LabelFront& current_front();

  LabelFront& next_front();

  void expand_all();

public:
  SCARPProgram(const std::vector<Controls>& controls,
               const CostFunction& costs,
               double scale_factor = 1.,
               bool vanishing_constraints = false);

  std::vector<Controls> solve();

  std::vector<std::vector<Controls>> solve_all();
};


#endif /* SCARP_PROGRAM_HH */
