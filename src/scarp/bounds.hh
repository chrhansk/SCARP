#ifndef SCARP_BOUNDS_HH
#define SCARP_BOUNDS_HH

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

  double for_mesh(const Mesh& mesh, double scale_factor=1.) const
  {
    return _value * scale_factor * mesh.coarseness();
  }
};

namespace bounds
{

// SUR bound, Thm 6.1 in "Approximation Properties and Tight Bounds
// for Constrained Mixed-Integer Optimal Control"
DeviationBound sur_simple(idx dimension);

// NFR (next-forced rounding) bound, Prop 4.8 in "Relaxations and
// Approximations for Mixed-Integer Optimal Control"
DeviationBound nfr(idx dimension);

// SUR bound with vanishing constraints, Thm 6.1 in "Approximation
// properties of sum- up rounding in the presence of vanishing
// constraints"
DeviationBound sur_vc(idx dimension);

// either sur_simple or sur_vc
DeviationBound sur(idx dimension,
                   bool vanishing_constraints);

// SUR bound with vanishing constraints derived from matching
// argument, Lemma 4.3 in "Matching Algorithms and Complexity
// Results for Constrained Mixed-Integer Optimal Control with
// Switching Costs"
DeviationBound sur_vc_matching(idx dimension);

// Best bound of the previous ones, (evaluating to 1.)
DeviationBound best(idx dimension,
                    bool vanishing_constraints);

} // namespace bounds

} // namespace scarp


#endif /* SCARP_BOUNDS_HH */
