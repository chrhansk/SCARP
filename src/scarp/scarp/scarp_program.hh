#ifndef SCARP_SCARP_PROGRAM_HH
#define SCARP_SCARP_PROGRAM_HH

#include <memory>

#include <set>
#include <unordered_set>
#include <vector>

#include "scarp/scarp/scarp_label.hh"

#include "scarp/controls.hh"
#include "scarp/instance.hh"
#include "scarp/parameters.hh"

#include "scarp/cost_function.hh"

namespace scarp
{

class SCARPProgram
{
public:
  typedef std::unordered_set<SCARPLabelPtr,
                             SCARPLabelPtrHash,
                             SCARPLabelComparator> LabelSet;

  //typedef std::set<LabelPtr, LabelOrdering> LabelSet;
  typedef std::vector<LabelSet> LabelFront;
private:
  const CostFunction& costs;
  std::vector<Handle> predecessors;
  std::vector<idx> controls;

  LabelFront _current_labels, _next_labels;

  idx num_labels, num_expansions;

  const Instance& instance;
  const Controls& fractional_controls;

  const bool vanishing_constraints;

  const idx size, dimension;

  const double max_deviation;

  std::vector<double> fractional_sum;

  idx iteration;

  void create_initial_labels();

  void expand_labels();

  bool is_feasible(const SCARPLabel& label) const;

  bool check_feasible(const BinaryControls& actual_controls) const;

  void expand_label(SCARPLabelPtr label);

  BinaryControls get_controls(SCARPLabelPtr label) const;

  LabelFront& current_front();

  LabelFront& next_front();

  void expand_all();

public:
  SCARPProgram(const Instance& instance,
               const CostFunction& costs,
               double max_deviation,
               bool vanishing_constraints = false);

  BinaryControls solve();

  std::vector<BinaryControls> solve_all();
};

} // namespace scarp

#endif /* SCARP_SCARP_PROGRAM_HH */
