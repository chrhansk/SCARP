#ifndef CONTROLS_HH
#define CONTROLS_HH

#include <vector>

#include "cmp.hh"
#include "util.hh"

typedef std::vector<double> Controls;

double control_distance(const std::vector<Controls>& first,
                        const std::vector<Controls>& second);

double control_costs(const std::vector<Controls>& controls,
                     const std::vector<double>& switch_on_costs,
                     const std::vector<double>& switch_off_costs);


double max_control_deviation(idx dimension);

bool controls_are_integral(const std::vector<Controls>& controls,
                           double eps = cmp::eps);

bool controls_are_convex(const std::vector<Controls>& controls,
                         double eps = cmp::eps);

bool controls_satisfy_vanishing_constraints(const std::vector<Controls>& controls,
                                            const std::vector<Controls>& fractional_controls,
                                            double eps = cmp::eps);

#endif /* CONTROLS_HH */
