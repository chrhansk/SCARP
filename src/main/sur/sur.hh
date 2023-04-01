#ifndef SUR_HH
#define SUR_HH

#include "instance.hh"

BinaryControls compute_sur_controls(const Instance& instance,
                                    bool vanishing_constraints = false,
                                    double eps = cmp::eps);

#endif /* SUR_HH */
