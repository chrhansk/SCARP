#include "controls.hh"

#include <cassert>
#include <cmath>

#include "scarp/mesh.hh"
#include "scarp/util.hh"

namespace scarp
{

bool Controls::are_convex(double eps) const
{
  for(idx i = 0; i < num_cells(); ++i)
  {
    double control_sum = 0.;

    for(idx j = 0; j < dimension(); ++j)
    {
      control_sum += (*this)(i, j);
    }

    if(!cmp::eq(control_sum, 1., eps))
    {
      return false;
    }
  }

  return  true;
}

bool Controls::are_integral(double eps) const
{
  const idx n = num_cells();
  const idx m = dimension();

  for(idx k = 0; k < n; ++k)
  {
    for(idx i = 0; i < m; ++i)
    {
      if(!cmp::integral((*this)(k, i), eps))
      {
        return false;
      }
    }
  }

  return true;
}

double Controls::distance(const Controls& other,
                          const Mesh& mesh) const
{
  assert(num_cells() == mesh.num_cells());
  assert(num_cells() == other.num_cells());
  assert(dimension() == other.dimension());

  double max_deviation = -inf;

  const idx n = num_cells();
  const idx m = dimension();

  std::vector<double> sum(m, 0.);
  std::vector<double> other_sum(m, 0.);

  for(idx j = 0; j < n; ++j)
  {
    const idx cell_size = mesh.cell_size(j);

    for(idx i = 0; i < m; ++i)
    {
      sum.at(i) += cell_size * (*this)(j, i);
      other_sum.at(i) += cell_size * other(j, i);
    }

    for(idx i = 0; i < m; ++i)
    {
      double deviation = std::fabs(sum.at(i) - other_sum.at(i));

      if(deviation > max_deviation)
      {
        max_deviation = deviation;
      }
    }
  }

  return max_deviation;
}

bool Controls::satisfy_vanishing_constraints(const Controls& fractional_controls,
                                             double eps) const
{
  const idx n = num_cells();
  const idx m = dimension();

  for(idx k = 0; k < n; ++k)
  {
    for(idx i = 0; i < m; ++i)
    {
      if(cmp::zero(fractional_controls(k, i), eps) &&
         !cmp::zero((*this)(k, i), eps))
      {
        return false;
      }
    }
  }

  return true;
}

PartialControls Controls::partial_controls(idx n_cells) const
{
  return PartialControls(*this, n_cells);
}


FractionalControls::FractionalControls(idx num_cells, idx dimension)
  : fractional_controls(num_cells, dimension)
{

}

double FractionalControls::operator()(idx i, idx j) const
{
  return fractional_controls(i, j);
}

double& FractionalControls::operator()(idx i, idx j)
{
  return fractional_controls(i, j);
}

const idx FractionalControls::num_cells() const
{
  return fractional_controls.num_rows();
}

const idx FractionalControls::dimension() const
{
  return fractional_controls.num_cols();
}

BinaryControls::BinaryControls(std::vector<idx>& binary_controls,
                               idx dimension)
  : binary_controls(binary_controls),
    dim(dimension)
{
}

double BinaryControls::operator()(idx i, idx j) const
{
  return binary_controls[i] == j;
}

const idx BinaryControls::num_cells() const
{
  return binary_controls.size();
}

const idx BinaryControls::dimension() const
{
  return dim;
}

} // namespace scarp
