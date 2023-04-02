#ifndef SCARP_LABEL_HH
#define SCARP_LABEL_HH

#include <boost/container_hash/hash.hpp>

#include "label.hh"
#include "util.hh"

class SCARPLabel;

typedef std::shared_ptr<SCARPLabel> SCARPLabelPtr;

class SCARPLabel : public Label
{
private:
  SCARPLabelPtr predecessor;

public:
  SCARPLabel(idx current_control,
             double cost,
             SCARPLabelPtr predecessor,
             idx cell_size = 1)
    : Label(predecessor->get_control_sums(),
            current_control,
            cost),
      predecessor(predecessor)
  {
    control_sums.at(current_control) += cell_size;
  }

  SCARPLabel(idx current_control,
             idx dimension,
             double cost,
             idx cell_size = 1)
    : Label(std::vector<idx>(dimension, 0), current_control, cost)
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

  SCARPLabelPtr get_predecessor() const
  {
    return predecessor;
  }
};

namespace std
{
  template<>
  struct hash<SCARPLabel>
  {
    std::size_t operator()(const SCARPLabel& label) const
    {
      std::size_t seed = 0;

      boost::hash_combine(seed,
                          boost::hash_range(label.get_control_sums().begin(),
                                            label.get_control_sums().end()));

      boost::hash_combine(seed, label.get_current_control());

      return seed;
    }
  };
}

struct SCARPLabelHash
{
  std::size_t operator()(const SCARPLabelPtr& label) const
  {
    assert(label);
    return std::hash<SCARPLabel>()(*label);
  }
};

struct SCARPLabelOrdering
{
  std::size_t operator()(const SCARPLabelPtr& first,
                         const SCARPLabelPtr& second) const
  {
    assert(first);
    assert(second);

    return *first < *second;
  }
};

struct SCARPLabelComparator
{
  bool operator()(const SCARPLabelPtr& first,
                  const SCARPLabelPtr& second) const
  {
    return *first == *second;
  }
};

#endif /* SCARP_LABEL_HH */
