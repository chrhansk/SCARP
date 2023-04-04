#ifndef DEVIATION_BOUND_HH
#define DEVIATION_BOUND_HH

#include "mesh.hh"
#include "util.hh"

namespace scarp
{

class DeviationBound
{
private:
  const double _value;
public:
  explicit DeviationBound(double value);

  double value() const
  {
    return _value;
  }

  double for_mesh(const Mesh& mesh) const
  {
    return _value * mesh.coarseness();
  }
};

// SUR bound, Thm 6.1 in "Approximation Properties and Tight Bounds
// for Constrained Mixed-Integer Optimal Control"
DeviationBound sur_bound(idx dimension);

// NFR (next-forced rounding) bound, Prop 4.8 in "Relaxations and
// Approximations for Mixed-Integer Optimal Control"
DeviationBound nfr_bound(idx dimension);

// SUR bound with vanishing constraints, Thm 6.1 in "Approximation
// properties of sum- up rounding in the presence of vanishing
// constraints"
DeviationBound sur_vc_bound(idx dimension);

// SUR bound with vanishing constraints derived from matching
// argument, Lemma 4.3 in "Matching Algorithms and Complexity
// Results for Constrained Mixed-Integer Optimal Control with
// Switching Costs"
DeviationBound sur_vc_matching_bound(idx dimension);

// Best bound of the previous ones, (evaluating to 1.)
DeviationBound best_bound(idx dimension,
                          bool vanishing_constraints);

}


#endif /* DEVIATION_BOUND_HH */
