#ifndef CONTROLS_HH
#define CONTROLS_HH

#include <cassert>
#include <vector>

#include "matrix.hh"
#include "cmp.hh"
#include "util.hh"

// typedef std::vector<double> Controls;

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

  // simple distance on uniform grid
  double distance(const Controls& other) const;

  // adaptive distance
  template<class T>
  double distance(const Controls& other,
                  const std::vector<T>& cell_volumes) const;

};

template<class T>
double Controls::distance(const Controls& other,
                          const std::vector<T>& cell_volumes) const
{
  assert(num_cells() == other.num_cells());
  assert(dimension() == other.dimension());
  assert(num_cells() == cell_volumes.size());

  double max_deviation = -inf;

  const idx n = num_cells();
  const idx m = dimension();

  std::vector<double> sum(m, 0.);
  std::vector<double> other_sum(m, 0.);

  for(idx j = 0; j < n; ++j)
  {
    const T cell_volume = cell_volumes[j];

    for(idx i = 0; i < m; ++i)
    {
      sum.at(i) += cell_volume * (*this)(j, i);
      other_sum.at(i) += cell_volume * other(j, i);
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

double max_control_deviation(idx dimension);

#endif /* CONTROLS_HH */
