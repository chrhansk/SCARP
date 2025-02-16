#ifndef SCARP_LABEL_HH
#define SCARP_LABEL_HH

#include <cassert>
#include <vector>

#include "scarp/util.hh"

namespace scarp
{

class Label
{
protected:
  std::vector<idx> control_sums;
  idx current_control;
  double cost;

public:
  Label(const std::vector<idx>& control_sums,
        idx current_control,
        double cost)
    : control_sums(control_sums),
      current_control(current_control),
      cost(cost)
  {
    assert(current_control < dimension());
  }

  const std::vector<idx>& get_control_sums() const
  {
    return control_sums;
  }

  const idx dimension() const
  {
    return control_sums.size();
  }

  idx get_current_control() const
  {
    return current_control;
  }

  double get_cost() const
  {
    return cost;
  }
};

} // namespace scarp

#endif /* SCARP_LABEL_HH */
