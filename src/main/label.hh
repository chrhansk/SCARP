#ifndef LABEL_HH
#define LABEL_HH

#include "util.hh"

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
  {}

  const std::vector<idx>& get_control_sums() const
  {
    return control_sums;
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

#endif /* LABEL_HH */
