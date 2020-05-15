#include "sur.hh"

#include "log.hh"

#include <cassert>

std::vector<Controls> compute_sur_controls(const std::vector<Controls>& fractional_controls,
                                           bool vanishing_constraints,
                                           double eps)
{
  assert(controls_are_convex(fractional_controls, eps));

  const idx size = fractional_controls.front().size();
  const idx dimension = fractional_controls.size();

  Log(info) << "Computing SUR controls for size: "
            << size
            << ", dimension: "
            << dimension
            << ", VC? "
            << std::boolalpha
            << vanishing_constraints;

  std::vector<Controls> sur_controls(dimension, Controls(size, 0.));

  std::vector<double> fractional_control_sums(dimension, 0.);

  std::vector<idx> control_sums(dimension, 0);

  auto add_fractional_controls = [&](idx j)
    {
      for(idx i = 0; i < dimension; ++i)
      {
        fractional_control_sums.at(i) += fractional_controls.at(i).at(j);
      }
    };

  for(idx j = 0; j < size; ++j)
  {
    idx next_control;
    double next_val = -inf;

    add_fractional_controls(j);

    for(idx i = 0; i < dimension; ++i)
    {
      double val = fractional_control_sums.at(i) - ((double) control_sums.at(i));

      if(vanishing_constraints)
      {
        if(cmp::zero(fractional_controls.at(i).at(j), eps))
        {
          continue;
        }
      }

      if(val > next_val)
      {
        next_val = val;
        next_control = i;
      }
    }

    sur_controls.at(next_control).at(j) = 1;

    ++control_sums.at(next_control);
  }

  assert(controls_are_integral(sur_controls, eps));

  assert(controls_are_convex(sur_controls, eps));

  if(vanishing_constraints)
  {
    assert(controls_satisfy_vanishing_constraints(sur_controls, fractional_controls, eps));
  }

  return sur_controls;
}
