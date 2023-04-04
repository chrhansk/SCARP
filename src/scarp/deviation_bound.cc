#include "deviation_bound.hh"

#include <cassert>
#include <cmath>

namespace scarp
{

DeviationBound::DeviationBound(double value)
  : _value(value)
{
  assert(_value > 0.);
}

DeviationBound sur_bound(idx dimension)
{
  assert(dimension >= 2);

  double max_deviation = 0.;

  for(idx i = 2; i <= dimension; ++i)
  {
    max_deviation += 1. / (double(i));
  }

  return DeviationBound(max_deviation);
}

DeviationBound nfr_bound(idx dimension)
{
  return DeviationBound(1.);
}

DeviationBound sur_vc_bound(idx dimension)
{
  assert(dimension >= 2);
  double dim = (double) dimension;
  double dim_bound = std::floor(dim / 2.);
  return DeviationBound((idx) dim_bound);
}

DeviationBound sur_vc_matching_bound(idx dimension)
{
  return DeviationBound(1.);
}

DeviationBound best_bound(idx dimension,
                          bool vanishing_constraints)
{
  if(vanishing_constraints)
  {
    return sur_vc_bound(dimension);
  }
  else
  {
    return nfr_bound(dimension);
  }
}

} // namespace scarp
