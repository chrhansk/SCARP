#ifndef SUR_HH
#define SUR_HH

#include "controls.hh"

std::vector<Controls> compute_sur_controls(const std::vector<Controls>& fractional_controls,
                                           bool vanishing_constraints = false,
                                           double eps = cmp::eps);

#endif /* SUR_HH */
