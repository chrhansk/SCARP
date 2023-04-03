#ifndef INSTANCE_READER_HH
#define INSTANCE_READER_HH

#include <iostream>
#include <fstream>

#include "scarp/controls.hh"
#include "scarp/instance.hh"

class InstanceReader
{
private:
  FractionalControls create_controls(const std::vector<std::vector<double>>& control_values);
public:
  Instance read_uniform(std::istream& input);

  Instance read_adaptive(std::istream& input);

  Instance read(std::istream& input, bool adaptive);
};


#endif /* INSTANCE_READER_HH */
