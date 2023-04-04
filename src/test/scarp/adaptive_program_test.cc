#include "adaptive_program_test.hh"
#include "scarp/mesh.hh"
#include "scarp/bounds.hh"

double AdaptiveProgramTest::bound_for(const Instance& instance,
                                      bool vanishing_constraints) const
{
  const idx dimension = instance.dimension();

  // Note: This bound would not ordinarily ensure feasibility.
  // But: the adaptive test instances were generated in such
  // a way that the SUR bound ensures feasibility. The
  // optimal values are also with respect to this bound
  UniformMesh uniform_mesh(instance.get_mesh().num_cells());

  return bounds::sur_simple(dimension).for_mesh(uniform_mesh,
                                                scale_factor);
}
