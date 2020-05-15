#include "control_writer.hh"

void ControlWriter::write(std::ostream& output,
                          const std::vector<Controls>& controls)
{
  idx dimension = controls.size();
  idx size = controls.begin()->size();

  for(idx k = 0; k < size; ++k)
  {
    for(idx i = 0; i < dimension; ++i)
    {
      output << controls[i][k];

      if(i < dimension - 1)
      {
        output << " ";
      }
    }

    output << std::endl;
  }

}

void ControlWriter::writeAMPL(std::ostream& output,
                              const std::vector<Controls>& controls)
{
  output << "param y := " << std::endl
         << "[*,*]: " << std::endl;

  idx dimension = controls.size();
  idx size = controls.begin()->size();

  for(idx i = 0; i < dimension; ++i)
  {
    output << std::fixed << (double) (i + 1);

    if(i + 1 < dimension)
    {
      output << "  ";
    }
  }

  output << ":=" << std::endl;

  for(idx k = 0; k < size; ++k)
  {
    output << k << " ";

    for(idx i = 0; i < dimension; ++i)
    {
      output << (int) std::round(controls[i][k]);

      if(i + 1 < dimension)
      {
        output << "  ";
      }
    }

    output << std::endl;
  }

  output << ";" << std::endl;

}
