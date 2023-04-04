#ifndef SCARP_PARAMETERS_HH
#define SCARP_PARAMETERS_HH

#include <vector>
#include "util.hh"

namespace scarp
{

std::vector<double> default_switch_on_costs(idx dimension);

std::vector<double> default_switch_off_costs(idx dimension);

} // namespace scarp

#endif /* SCARP_PARAMETERS_HH */
