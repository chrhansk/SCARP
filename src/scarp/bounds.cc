#include "bounds.hh"

#include <cassert>
#include <cmath>

namespace scarp
{

DeviationBound::DeviationBound(double value)
  : _value(value)
{
  assert(_value > 0.);
}

namespace bounds
{

DeviationBound sur_simple(idx dimension)
{
  assert(dimension >= 2);

  double max_deviation = 0.;

  for(idx i = 2; i <= dimension; ++i)
  {
    max_deviation += 1. / (double(i));
  }

  return DeviationBound(max_deviation);
}

DeviationBound nfr(idx dimension)
{
  return DeviationBound(1.);
}

DeviationBound sur(idx dimension,
                   bool vanishing_constraints)
{
  if(vanishing_constraints)
  {
    return sur_vc(dimension);
  }
  else
  {
    return sur_simple(dimension);
  }
}

DeviationBound sur_vc(idx dimension)
{
  assert(dimension >= 2);
  double dim = (double) dimension;
  double dim_bd = std::floor(dim / 2.);
  return DeviationBound((idx) dim_bd);
}

DeviationBound sur_vc_matching(idx dimension)
{
  return DeviationBound(1.);
}

DeviationBound best(idx dimension,
                          bool vanishing_constraints)
{
  if(vanishing_constraints)
  {
    return sur_vc(dimension);
  }
  else
  {
    return nfr(dimension);
  }
}

} // namespace bounds

} // namespace scarp
