#ifndef CONTROL_WRITER_HH
#define CONTROL_WRITER_HH

#include <iostream>
#include <fstream>

#include "controls.hh"

namespace scarp
{

class ControlWriter
{
public:
  void write(std::ostream& output,
             const Controls& controls);

  void writeAMPL(std::ostream& output,
                 const Controls& controls);
};

}

#endif /* CONTROL_WRITER_HH */
