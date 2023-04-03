#include "sur.hh"

#include "scarp/log.hh"

#include <cassert>

BinaryControls compute_sur_controls(const Instance& instance,
                                    bool vanishing_constraints,
                                    double eps)
{
  const auto& fractional_controls = instance.get_fractional_controls();
  const Mesh& mesh = instance.get_mesh();

  assert(fractional_controls.are_convex(eps));

  const idx n = fractional_controls.num_cells();
  const idx m = fractional_controls.dimension();

  Log(info) << "Computing SUR controls for size: "
            << n
            << ", dimension: "
            << m
            << ", VC? "
            << std::boolalpha
            << vanishing_constraints;

  std::vector<idx> binary_controls;
  binary_controls.reserve(n);

  std::vector<double> fractional_control_sums(m, 0.);

  std::vector<idx> control_sums(m, 0);

  auto add_fractional_controls = [&](idx j)
    {
      for(idx i = 0; i < m; ++i)
      {
        fractional_control_sums.at(i) += mesh.cell_size(j) * fractional_controls(j, i);
      }
    };

  for(idx j = 0; j < n; ++j)
  {
    idx next_control;
    double next_val = -inf;

    add_fractional_controls(j);

    for(idx i = 0; i < m; ++i)
    {
      double val = fractional_control_sums.at(i) - ((double) control_sums.at(i));

      if(vanishing_constraints)
      {
        if(cmp::zero(fractional_controls(j, i), eps))
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

    binary_controls.push_back(next_control);

    ++control_sums.at(next_control);
  }

  BinaryControls sur_controls(binary_controls,
                              m);

  assert(sur_controls.are_integral(eps));

  assert(sur_controls.are_convex(eps));

  if(vanishing_constraints)
  {
    assert(sur_controls.satisfy_vanishing_constraints(fractional_controls, eps));
  }

  return sur_controls;
}
