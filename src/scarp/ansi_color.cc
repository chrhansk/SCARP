#include "ansi_color.hh"

#include <unistd.h>   // For isatty

namespace scarp
{

  namespace ansi
  {
    const bool can_colorize(isatty(STDOUT_FILENO));
  }

} // namespace scarp
