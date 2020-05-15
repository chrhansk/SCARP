#ifndef CONTROL_WRITER_HH
#define CONTROL_WRITER_HH

#include <iostream>
#include <fstream>

#include "controls.hh"

class ControlWriter
{
public:
  void write(std::ostream& output,
             const std::vector<Controls>& controls);

  void writeAMPL(std::ostream& output,
                 const std::vector<Controls>& controls);
};


#endif /* CONTROL_WRITER_HH */
