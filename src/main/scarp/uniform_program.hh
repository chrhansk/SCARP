#ifndef UNIFORM_PROGRAM_HH
#define UNIFORM_PROGRAM_HH

#include "base_program.hh"

#include "controls.hh"
#include "parameters.hh"

#include "cost_function.hh"

class UniformProgram : public BaseProgram<idx>
{
private:
  void create_initial_labels() override;

  void expand_label(SCARPLabelPtr label) override;

  void add_fractional_controls(idx k) override;

public:
  UniformProgram(const Controls& fractional_controls,
                 const CostFunction& costs,
                 double scale_factor = 1.,
                 bool vanishing_constraints = false);
};


#endif /* UNIFORM_PROGRAM_HH */
