#ifndef SCARP_CONTROL_WRITER_HH
#define SCARP_CONTROL_WRITER_HH

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

#endif /* SCARP_CONTROL_WRITER_HH */
