#ifndef SCARP_LABEL_HH
#define SCARP_LABEL_HH

#include <utility>
#include <memory>
#include <vector>

#include "scarp/handle.hh"
#include "scarp/label.hh"
#include "scarp/util.hh"

namespace scarp
{

class SCARPLabel;

typedef std::shared_ptr<SCARPLabel> SCARPLabelPtr;

class SCARPLabel : public Label
{
private:
  Handle predecessor;

public:
  SCARPLabel(idx current_control,
             double cost,
             Handle predecessor,
             const std::vector<idx>& control_sums)
    : Label(control_sums,
            current_control,
            cost),
      predecessor(predecessor)
  {
  }

  SCARPLabel(idx current_control,
             idx dimension,
             double cost,
             idx cell_size = 1)
    : Label(std::vector<idx>(dimension, 0), current_control, cost),
      predecessor(Handle::invalid())
  {
    control_sums.at(current_control) += cell_size;
  }

  void replace(const SCARPLabel& other)
  {
    control_sums = other.get_control_sums();
    current_control = other.get_current_control();
    cost = other.get_cost();
    predecessor = other.get_predecessor();
  }

  bool operator==(const SCARPLabel& other) const
  {
    return (control_sums == other.control_sums) &&
      (current_control == other.current_control);
  }

  bool operator<(const SCARPLabel& other) const
  {
    for(idx i = 0; i < control_sums.size(); ++i)
    {
      if(control_sums[i] != other.control_sums[i])
      {
        return control_sums[i] < other.control_sums[i];
      }
    }

    return current_control < other.current_control;
  }

  Handle get_predecessor() const
  {
    return predecessor;
  }
};

struct SCARPLabelHash
{
  std::size_t operator()(const SCARPLabel& label) const;
};

struct SCARPLabelPtrHash
{
  std::size_t operator()(const SCARPLabelPtr& label) const;
};

struct SCARPLabelPtrOrdering
{
  std::size_t operator()(const SCARPLabelPtr& first,
                         const SCARPLabelPtr& second) const;
};

struct SCARPLabelComparator
{
  bool operator()(const SCARPLabelPtr& first,
                  const SCARPLabelPtr& second) const;
};

} // namespace scarp

#endif /* SCARP_LABEL_HH */
