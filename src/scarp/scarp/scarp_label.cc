#include "scarp_label.hh"

#include <boost/container_hash/hash.hpp>

namespace scarp
{

std::size_t SCARPLabelHash::operator()(const SCARPLabel& label) const
{
  std::size_t seed = 0;

  boost::hash_combine(seed,
                      boost::hash_range(label.get_control_sums().begin(),
                                        label.get_control_sums().end()));

  boost::hash_combine(seed, label.get_current_control());

  return seed;
}

std::size_t SCARPLabelPtrHash::operator()(const SCARPLabelPtr& label) const
{
  assert(label);
  return SCARPLabelHash()(*label);
}

std::size_t SCARPLabelPtrOrdering::operator()(const SCARPLabelPtr& first,
                                              const SCARPLabelPtr& second) const
{
  assert(first);
  assert(second);

  return *first < *second;
}

bool SCARPLabelComparator::operator()(const SCARPLabelPtr& first,
                                      const SCARPLabelPtr& second) const
{
  return *first == *second;
}

} // namespace scarp
