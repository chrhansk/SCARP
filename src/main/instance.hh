#ifndef INSTANCE_HH
#define INSTANCE_HH

#include <cassert>
#include <memory>

#include "controls.hh"
#include "mesh.hh"

class Instance
{
private:
  std::unique_ptr<Mesh> mesh;
  FractionalControls fractional_controls;

public:
  Instance(std::unique_ptr<Mesh>&& _mesh,
           FractionalControls&& fractional_controls)
    : mesh(std::move(_mesh)),
      fractional_controls(fractional_controls)
  {
    assert(mesh);
    assert(mesh->num_cells() == fractional_controls.num_cells());
  }

  const idx num_cells() const
  {
    return fractional_controls.num_cells();
  }

  const idx dimension() const
  {
    return fractional_controls.dimension();
  }

  const Mesh& get_mesh() const
  {
    return *mesh;
  }

  const FractionalControls& get_fractional_controls() const
  {
    return fractional_controls;
  }

  Mesh& get_mesh()
  {
    return *mesh;
  }

  FractionalControls& get_fractional_controls()
  {
    return fractional_controls;
  }

};

#endif /* INSTANCE_HH */
