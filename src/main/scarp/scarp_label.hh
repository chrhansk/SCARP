#ifndef SCARP_LABEL_HH
#define SCARP_LABEL_HH

#include <boost/container_hash/hash.hpp>

#include "label.hh"
#include "util.hh"

template <class T>
class GenSCARPLabel;


template <class T> using GenSCARPLabelPtr = std::shared_ptr<GenSCARPLabel<T>>;

template<class T>
class GenSCARPLabel : public GenLabel<T>
{
private:
  GenSCARPLabelPtr<T> predecessor;

  using GenLabel<T>::control_sums;
  using GenLabel<T>::current_control;
  using GenLabel<T>::cost;

public:
  GenSCARPLabel(idx current_control,
                double cost,
                GenSCARPLabelPtr<T> predecessor,
                T curr_volume = T(1))
    : Label(predecessor->get_control_sums(),
            current_control,
            cost),
      predecessor(predecessor)
  {
    control_sums.at(current_control) += curr_volume;
  }

  GenSCARPLabel(idx curr_control,
                idx dimension,
                double cost,
                T curr_volume = T(1))
    : Label(std::vector<T>(dimension, (T) 0), curr_control, cost)
  {
    control_sums.at(curr_control) += curr_volume;
  }

  void replace(const GenSCARPLabel<T>& other)
  {
    control_sums = other.get_control_sums();
    current_control = other.get_current_control();
    cost = other.get_cost();
    predecessor = other.get_predecessor();
  }

  GenSCARPLabelPtr<T> get_predecessor() const
  {
    return predecessor;
  }
};

namespace std
{
  template<class T>
  struct hash<GenSCARPLabel<T>>
  {
    std::size_t operator()(const GenSCARPLabel<T>& label) const
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


template<class T>
struct GenSCARPLabelHash
{
  std::size_t operator()(const GenSCARPLabelPtr<T>& label) const
  {
    assert(label);
    return std::hash<GenSCARPLabel<T>>()(*label);
  }
};

template<class T>
struct GenSCARPLabelOrdering
{
  std::size_t operator()(const GenSCARPLabelPtr<T>& first,
                         const GenSCARPLabelPtr<T>& second) const
  {
    assert(first);
    assert(second);

    const auto& my_sums = first->get_control_sums();
    const auto& other_sums = second->get_control_sums();

    assert(my_sums.size() == other_sums.size());

    const idx size = my_sums.size();

    for(idx i = 0; i < size; ++i)
    {
      if(my_sums[i] != other_sums[i])
      {
        return my_sums[i] < other_sums[i];
      }
    }

    return false;
  }
};

template<class T>
struct GenSCARPLabelComparator
{
  bool operator()(const GenSCARPLabelPtr<T>& first,
                  const GenSCARPLabelPtr<T>& second) const
  {
    assert(first);
    assert(second);

    return first->get_control_sums() == second->get_control_sums();
  }
};

typedef GenSCARPLabel<idx> SCARPLabel;
typedef GenSCARPLabelPtr<idx> SCARPLabelPtr;
typedef GenSCARPLabelHash<idx> SCARPLabelHash;
typedef GenSCARPLabelComparator<idx> SCARPLabelComparator;

#endif /* SCARP_LABEL_HH */
