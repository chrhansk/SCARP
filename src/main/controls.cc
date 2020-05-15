#include "controls.hh"

#include <cmath>

double control_distance(const std::vector<Controls>& first_controls,
                        const std::vector<Controls>& second_controls)
{
  const idx size = first_controls.front().size();
  const idx dimension = first_controls.size();

  std::vector<double> first_sum(dimension, 0.);
  std::vector<double> second_sum(dimension, 0.);

  double max_deviation = -inf;

  for(idx k = 0; k < size; ++k)
  {
    for(idx i = 0; i < dimension; ++i)
    {
      first_sum.at(i) += first_controls[i][k];
      second_sum.at(i) += second_controls[i][k];
    }

    for(idx i = 0; i < dimension; ++i)
    {
      double deviation = std::fabs(first_sum.at(i) - second_sum.at(i));

      if(deviation > max_deviation)
      {
        max_deviation = deviation;
      }
    }
  }

  return max_deviation;
}

double control_costs(const std::vector<Controls>& controls,
                     const std::vector<double>& switch_on_costs,
                     const std::vector<double>& switch_off_costs)
{
  const idx size = controls.front().size();
  const idx dimension = controls.size();

  double costs = 0.;
  idx current_control = 0;

  for(idx i = 0; i < dimension; ++i)
  {
    if(controls[i][0] > 0)
    {
      current_control = i;
      costs += switch_on_costs.at(i);
      break;
    }
  }

  for(idx k = 0; k < size; ++k)
  {
    for(idx i = 0; i < dimension; ++i)
    {
      if(controls[i][k] > 0 && i != current_control)
      {
        costs += switch_on_costs.at(i) + switch_off_costs.at(current_control);

        current_control = i;

        break;
      }
    }
  }

  costs += switch_off_costs.at(current_control);

  return costs;
}

double max_control_deviation(idx dimension)
{
  double max_deviation = 0.;

  for(idx i = 2; i <= dimension; ++i)
  {
    max_deviation += 1. / (double(i));
  }

  return max_deviation;
}

bool controls_are_integral(const std::vector<Controls>& controls,
                           double eps)
{
  const idx size = controls.front().size();
  const idx dimension = controls.size();

  for(idx k = 0; k < size; ++k)
  {
    for(idx i = 0; i < dimension; ++i)
    {
      if(!cmp::integral(controls[i][k], eps))
      {
        return false;
      }
    }
  }

  return true;
}

bool controls_are_convex(const std::vector<Controls>& controls,
                         double eps)
{
  const idx size = controls.front().size();
  const idx dimension = controls.size();

  for(idx k = 0; k < size; ++k)
  {
    double control_sum = 0.;

    for(idx i = 0; i < dimension; ++i)
    {
      control_sum += controls[i][k];
    }

    if(!cmp::eq(control_sum, 1., eps))
    {
      return false;
    }
  }

  return true;
}

bool controls_satisfy_vanishing_constraints(const std::vector<Controls>& controls,
                                            const std::vector<Controls>& fractional_controls,
                                            double eps)
{
  const idx size = controls.front().size();
  const idx dimension = controls.size();

  for(idx k = 0; k < size; ++k)
  {
    for(idx i = 0; i < dimension; ++i)
    {
      if(cmp::zero(fractional_controls[i][k], eps) &&
         !cmp::zero(controls[i][k], eps))
      {
        return false;
      }
    }
  }

  return true;
}
