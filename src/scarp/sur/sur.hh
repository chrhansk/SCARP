#ifndef SCARP_SUR_HH
#define SCARP_SUR_HH

#include "scarp/instance.hh"

namespace scarp
{

BinaryControls compute_sur_controls(const Instance& instance,
                                    bool vanishing_constraints = false,
                                    double eps = cmp::eps);

} // namespace scarp

#endif /* SCARP_SUR_HH */
