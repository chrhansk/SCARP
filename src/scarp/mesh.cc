#include "mesh.hh"

namespace scarp
{

idx Mesh::coarseness() const
{
  idx coarseness = 0;

  for(idx i = 0; i < num_cells(); ++i)
  {
    coarseness = std::max(coarseness,
                          cell_size(i));
  }

  return coarseness;
}

} // namespace scarp
