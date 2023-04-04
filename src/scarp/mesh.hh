#ifndef MESH_HH
#define MESH_HH

#include <cassert>
#include <vector>

#include "scarp/util.hh"

namespace scarp
{

class Mesh
{
public:
  virtual idx cell_size(idx index) const = 0;
  virtual idx num_cells() const = 0;

  // coarseness, i.e., max cell size
  virtual idx coarseness() const;
};

class UniformMesh : public Mesh
{
  const idx n_cells;
public:
  UniformMesh(idx n_cells)
    : n_cells(n_cells)
  {}

  idx cell_size(idx index) const override
  {
    return 1;
  }

  idx coarseness() const override
  {
    return 1;
  }

  idx num_cells() const override
  {
    return n_cells;
  }

};

class AdaptiveMesh : public Mesh
{
  const std::vector<idx> cell_sizes;
public:
  AdaptiveMesh(const std::vector<idx>& cell_sizes)
    : cell_sizes(cell_sizes)
  {
  }

  idx cell_size(idx index) const override
  {
    assert(index < num_cells());
    return cell_sizes[index];
  }

  idx num_cells() const override
  {
    return cell_sizes.size();
  }
};

class PartialMesh : public Mesh
{
private:
  const Mesh& original_mesh;
  idx n_cells;

public:
  PartialMesh(const Mesh& original_mesh,
              idx n_cells)
    : original_mesh(original_mesh),
      n_cells(n_cells)
  {
    assert(n_cells <= original_mesh.num_cells());
  }

  idx cell_size(idx index) const override
  {
    assert(index < n_cells);
    return original_mesh.cell_size(index);
  }

  idx num_cells() const override
  {
    return n_cells;
  }
};

} // namespace scarp

#endif /* MESH_HH */
