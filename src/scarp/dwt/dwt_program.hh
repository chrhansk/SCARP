#ifndef DWT_PROGRAM_HH
#define DWT_PROGRAM_HH

#include <unordered_map>

#include <boost/container_hash/hash.hpp>

#include "scarp/bounded_label_set.hh"
#include "scarp/cost_function.hh"
#include "scarp/controls.hh"
#include "scarp/dwt/dwt_label.hh"

class InfeasibleException : public std::exception{};

class DWTProgram
{
private:

  struct DWTCosts
  {
    double operator()(const DWTLabel& label) const
    {
      return label.get_cost();
    }
  };

  struct DWTLengths
  {
    idx operator()(const DWTLabel& label) const
    {
      return label.get_remaining_dwt();
    }
  };

  typedef BoundedLabelSet<DWTLabel,
                          DWTCosts,
                          DWTLengths> LabelSet;

  typedef std::vector<idx> ControlSums;

  struct ControlSumsHash
  {
    std::size_t operator()(const ControlSums& control_sums) const
    {
      std::size_t seed = 0;

      boost::hash_combine(seed,
                          boost::hash_range(control_sums.begin(), control_sums.end()));

      return seed;
    }
  };

  typedef std::unordered_map<ControlSums, LabelSet, ControlSumsHash> LabelMap;

  typedef std::vector<LabelMap> LabelFront;

  const CostFunction& costs;

  LabelFront _current_labels, _next_labels;

  const Controls& fractional_controls;

  const idx size, dimension;

  std::vector<idx> minimum_dwt;

  double max_deviation;

  std::vector<double> fractional_sum;

  idx iteration;

  void create_initial_labels();

  void expand_labels();

  void expand_label(DWTLabelPtr label);

  LabelFront& current_front();

  LabelFront& next_front();

  bool is_feasible(const DWTLabel& label) const;

  BinaryControls get_controls(DWTLabelPtr label) const;

public:
  DWTProgram(const Controls& fractional_controls,
             const CostFunction& costs,
             const std::vector<idx>& minimum_dwt,
             double scale_factor=1.);

  BinaryControls solve();
};


#endif /* DWT_PROGRAM_HH */
