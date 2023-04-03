#ifndef DWT_LABEL_HH
#define DWT_LABEL_HH

#include <memory>

#include "scarp/label.hh"
#include "scarp/util.hh"

class DWTLabel;

typedef std::shared_ptr<DWTLabel> DWTLabelPtr;

class DWTLabel : public Label
{
private:
  DWTLabelPtr predecessor;
  idx remaining_dwt;

public:
  DWTLabel(idx current_control,
           double cost_increase,
           DWTLabelPtr predecessor,
           idx remaining_dwt)
    : Label(predecessor->get_control_sums(),
            current_control,
            predecessor->get_cost() + cost_increase),
      predecessor(predecessor),
      remaining_dwt(remaining_dwt)
  {
    control_sums.at(current_control)++;
  }

  DWTLabel(idx current_control,
           idx dimensions,
           double cost,
           idx remaining_dwt)
    : Label(std::vector<idx>(dimensions, 0), current_control, cost),
      remaining_dwt(remaining_dwt)
  {
    control_sums.at(current_control)++;
  }

  void replace(const DWTLabel& other)
  {
    control_sums = other.get_control_sums();
    current_control = other.get_current_control();
    cost = other.get_cost();
    predecessor = other.get_predecessor();
    remaining_dwt = other.get_remaining_dwt();
  }

  bool operator==(const DWTLabel& other) const
  {
    return control_sums == other.control_sums;
  }

  const std::vector<idx>& get_control_sums() const
  {
    return control_sums;
  }

  idx get_remaining_dwt() const
  {
    return remaining_dwt;
  }

  idx get_current_control() const
  {
    return current_control;
  }

  bool operator<(const DWTLabel& other) const
  {
    for(idx i = 0; i < control_sums.size(); ++i)
    {
      if(control_sums[i] != other.control_sums[i])
      {
        return control_sums[i] < other.control_sums[i];
      }
    }

    return false;
  }

  double get_cost() const
  {
    return cost;
  }

  DWTLabelPtr get_predecessor() const
  {
    return predecessor;
  }

};

#endif /* DWT_LABEL_HH */
