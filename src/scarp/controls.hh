#ifndef SCARP_CONTROLS_HH
#define SCARP_CONTROLS_HH

#include <vector>

#include "scarp/matrix.hh"
#include "scarp/cmp.hh"
#include "scarp/util.hh"

namespace scarp
{

class Mesh;
class FractionalControls;
class PartialControls;

class Controls
{
public:

  // i: cell index
  // j: control index
  virtual double operator()(idx i, idx j) const = 0;

  virtual const idx num_cells() const = 0;
  virtual const idx dimension() const = 0;

  bool are_convex(double eps = cmp::eps) const;
  bool are_integral(double eps = cmp::eps) const;

  bool satisfy_vanishing_constraints(const Controls& fractional_controls,
                                     double eps = cmp::eps) const;

  double distance(const Controls& other,
                  const Mesh& mesh) const;

  PartialControls partial_controls(idx n_cells) const;
};

class PartialControls : public Controls
{
private:
  const Controls& controls;
  const idx n_cells;
public:
  PartialControls(const Controls& controls, idx n_cells)
    : controls(controls),
      n_cells(n_cells)
  {
    assert(n_cells <= controls.num_cells());
  }

  double operator()(idx i, idx j) const override
  {
    assert(i < n_cells);
    return controls(i, j);
  }

  const idx num_cells() const override
  {
    return n_cells;
  }

  const idx dimension() const override
  {
    return controls.dimension();
  }
};

class FractionalControls : public Controls
{
private:
  Matrix<double> fractional_controls;
public:
  FractionalControls(idx num_cells, idx dimension);

  double operator()(idx i, idx j) const override;
  double& operator()(idx i, idx j);

  const Matrix<double>& values() const
  {
    return fractional_controls;
  }

  const idx num_cells() const override;
  const idx dimension() const override;
};

class BinaryControls : public Controls
{
private:
  std::vector<idx> binary_controls;
  idx dim;

public:
  // constructor
  BinaryControls(std::vector<idx>& control_values,
                 idx dimension);

  std::vector<idx>& values()
  {
    return binary_controls;
  }

  const std::vector<idx>& values() const
  {
    return binary_controls;
  }

  double operator()(idx i, idx j) const override;

  const idx num_cells() const override;
  const idx dimension() const override;
};

} // namespace scarp

#endif /* SCARP_CONTROLS_HH */
