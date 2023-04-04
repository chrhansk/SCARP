#ifndef PARAMETERS_HH
#define PARAMETERS_HH

#include <vector>
#include "util.hh"

namespace scarp
{

std::vector<double> default_switch_on_costs(idx dimension);

std::vector<double> default_switch_off_costs(idx dimension);

} // namespace scarp

#endif /* PARAMETERS_HH */
