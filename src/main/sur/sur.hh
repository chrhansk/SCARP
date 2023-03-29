#ifndef SUR_HH
#define SUR_HH

#include "controls.hh"

BinaryControls compute_sur_controls(const Controls& fractional_controls,
                                    bool vanishing_constraints = false,
                                    double eps = cmp::eps);

#endif /* SUR_HH */
