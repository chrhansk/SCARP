#ifndef SCARP_INSTANCE_READER_HH
#define SCARP_INSTANCE_READER_HH

#include <iostream>

#include "scarp/controls.hh"
#include "scarp/instance.hh"

namespace scarp
{

class InstanceReader
{
private:
  FractionalControls create_controls(const std::vector<std::vector<double>>& control_values);
public:
  Instance read_uniform(std::istream& input);

  Instance read_adaptive(std::istream& input);

  Instance read(std::istream& input, bool adaptive);
};

} // namespace scarp

#endif /* SCARP_INSTANCE_READER_HH */
