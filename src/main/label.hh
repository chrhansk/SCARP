#ifndef LABEL_HH
#define LABEL_HH

#include "util.hh"

template <class T>
class GenLabel
{
protected:
  std::vector<T> control_sums;
  T current_control;
  double cost;

public:
  GenLabel(const std::vector<T>& control_sums,
           T current_control,
           double cost)
    : control_sums(control_sums),
      current_control(current_control),
      cost(cost)
  {}

  const std::vector<T>& get_control_sums() const
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

typedef GenLabel<idx> Label;

#endif /* LABEL_HH */
