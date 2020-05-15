#ifndef CONTROL_READER_HH
#define CONTROL_READER_HH

#include <iostream>
#include <fstream>

#include "controls.hh"

class ControlReader
{
public:
  std::vector<Controls> read(std::istream& input);
};


#endif /* CONTROL_READER_HH */
