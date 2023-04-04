#include "control_writer.hh"

namespace scarp
{

void ControlWriter::write(std::ostream& output,
                          const Controls& controls)
{
  const idx n = controls.num_cells();
  const idx m = controls.dimension();

  for(idx k = 0; k < n; ++k)
  {
    for(idx i = 0; i < m; ++i)
    {
      output << controls(k, i);

      if(i + 1 < m)
      {
        output << " ";
      }
    }

    output << std::endl;
  }

}

void ControlWriter::writeAMPL(std::ostream& output,
                              const Controls& controls)
{
  output << "param y := " << std::endl
         << "[*,*]: " << std::endl;

  const idx n = controls.num_cells();
  const idx m = controls.dimension();

  for(idx i = 0; i < m; ++i)
  {
    output << std::fixed << (double) (i + 1);

    if(i + 1 < m)
    {
      output << "  ";
    }
  }

  output << ":=" << std::endl;

  for(idx k = 0; k < n; ++k)
  {
    output << k << " ";

    for(idx i = 0; i < m; ++i)
    {
      output << (int) std::round(controls(k, i));

      if(i + 1 < m)
      {
        output << "  ";
      }
    }

    output << std::endl;
  }

  output << ";" << std::endl;

}

} // namespace scarp
