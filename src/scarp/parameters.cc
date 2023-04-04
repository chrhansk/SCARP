#include "parameters.hh"

#include <cassert>

namespace scarp
{

std::vector<double> default_switch_on_costs(idx dimension)
{
  std::vector<double> switch_on_costs{2., 1.};

  if(dimension <= 2)
  {
    switch_on_costs.resize(dimension, 0.);
  }
  else
  {
    for(idx j = 2; j < dimension; ++j)
    {
      switch_on_costs.push_back(0.);
    }
  }

  return switch_on_costs;
}

std::vector<double> default_switch_off_costs(idx dimension)
{
  if(dimension == 0)
  {
    return std::vector<double>{};
  }

  assert(dimension > 0);

  std::vector<double> switch_off_costs(dimension - 1, 0.1);

  switch_off_costs.push_back(0.);

  return switch_off_costs;
}

} // namespace scarp
