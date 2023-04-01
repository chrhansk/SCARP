#ifndef CONTROL_READER_HH
#define CONTROL_READER_HH

#include <iostream>
#include <fstream>

#include "controls.hh"

struct UniformReadResult
{
  FractionalControls fractional_controls;
};

struct AdaptiveReadResult
{
  FractionalControls fractional_controls;
  std::vector<idx> cell_volumes;
};

class ControlReader
{
public:
  UniformReadResult read_uniform(std::istream& input);

  AdaptiveReadResult read_adaptive(std::istream& input);
};


#endif /* CONTROL_READER_HH */
