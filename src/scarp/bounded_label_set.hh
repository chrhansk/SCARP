#ifndef BOUNDED_LABEL_SET_HH
#define BOUNDED_LABEL_SET_HH

#include <map>
#include <memory>

#include "util.hh"

template <class Label, class Costs, class Lengths>
class BoundedLabelSet
{
public:

  typedef std::map<idx, std::shared_ptr<Label>> LabelSet;

  class Labels
  {
  public:
    typedef typename LabelSet::const_iterator SetIterator;

    struct Iterator
    {
      SetIterator iter;

      std::shared_ptr<Label> operator*() const
      {
        return iter->second;
      }

      const Iterator& operator++()
      {
        ++iter;
        return *this;
      }

      const bool operator!=(const Iterator& other) const
      {
        return iter != other.iter;
      }
    };

    SetIterator begin_it, end_it;

    Labels(SetIterator begin_it, SetIterator end_it)
      : begin_it(begin_it),
        end_it(end_it)
    {}

    Iterator begin()
    {
      return Iterator{begin_it};
    }

    Iterator end()
    {
      return Iterator{end_it};
    }
  };

private:
  LabelSet labels;
  Costs costs;
  Lengths lengths;

  bool dominates(const Label& first, const Label& second) const;

public:
  BoundedLabelSet(std::shared_ptr<Label> label,
                  const Costs& costs = Costs(),
                  const Lengths& lengths = Lengths())
    : costs(costs),
      lengths(lengths)
  {
    insert(label);
  }

  BoundedLabelSet(const Costs& costs = Costs(),
                  const Lengths& lengths = Lengths())
    : costs(costs),
      lengths(lengths)
  {}

  bool insert(std::shared_ptr<Label> label);

  bool is_dominated(std::shared_ptr<Label> label);

  idx size() const
  {
    return labels.size();
  }

  Labels get_labels() const
  {
    return Labels(std::begin(labels), std::end(labels));
  }
};

template <class Label, class Costs, class Lengths>
bool BoundedLabelSet<Label, Costs, Lengths>::dominates(const Label& first,
                                                       const Label& second) const
{
  return (costs(first) <= costs(second)) && (lengths(first) <= lengths(second));
}

template <class Label, class Costs, class Lengths>
bool BoundedLabelSet<Label, Costs, Lengths>::insert(std::shared_ptr<Label> label)
{
  assert(label);

  idx length = lengths(*label);

  if(labels.empty())
  {
    labels.insert(std::make_pair(length, label));

    return true;
  }

  if(is_dominated(label))
  {
    return false;
  }

  auto upper_bound = labels.lower_bound(length);

  while(upper_bound != std::end(labels))
  {
    std::shared_ptr<Label> bound_label = upper_bound->second;

    if(dominates(*label, *bound_label))
    {
      upper_bound = labels.erase(upper_bound);
    }
    else
    {
      break;
    }
  }

  labels.insert(std::make_pair(length, label));

  return true;
}


template <class Label, class Costs, class Lengths>
bool
BoundedLabelSet<Label, Costs, Lengths>::is_dominated(std::shared_ptr<Label> label)
{
  assert(label);

  idx length = lengths(*label);

  auto lower_bound = labels.lower_bound(length);

  if(lower_bound == std::end(labels))
  {
    --lower_bound;
  }

  assert(lower_bound != std::end(labels));

  while((lower_bound->first > length) && (lower_bound != std::begin(labels)))
  {
    --lower_bound;
  }

  std::shared_ptr<Label> bound_label = lower_bound->second;

  if(dominates(*bound_label, *label))
  {
    return true;
  }

  return false;
}


#endif /* BOUNDED_LABEL_SET_HH */
