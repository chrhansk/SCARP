#include "control_reader.hh"

#include <boost/algorithm/string.hpp>
#include <string>

#include "log.hh"

class ReadError : public std::exception
{
	const char* what () const throw()
  {
    return "Read error";
  }
};

std::vector<std::string> get_tokens(const std::string& line)
{
  std::vector<std::string> tokens;

  boost::algorithm::split(tokens, line, boost::is_any_of(" "));

  std::vector<std::string> filtered_tokens;

  for(const std::string& token : tokens)
  {
    if(token != "")
    {
      filtered_tokens.push_back(token);
    }
  }

  return filtered_tokens;
}

UniformReadResult ControlReader::read_uniform(std::istream& input)
{
  std::vector<std::vector<double>> controls;

  std::string line;

  bool first_line = true;

  if(!input)
  {
    throw std::invalid_argument("Can't read from input");
  }

  while(std::getline(input, line))
  {
    std::vector<std::string> tokens = get_tokens(line);

    if(tokens.empty())
    {
      throw ReadError();
    }

    if(first_line)
    {
      for(const auto& token : tokens)
      {
        controls.push_back(std::vector<double>{std::stod(token)});
      }

      first_line = false;
    }
    else
    {
      if(tokens.size() != controls.size())
      {
        throw ReadError();
      }

      for(idx i = 0; i < controls.size(); ++i)
      {
        controls[i].push_back(std::stod(tokens[i]));
      }
    }
  }

  const idx dimension = controls.size();
  const idx num_cells = controls[0].size();

  Log(info) << "Read in controls with dimension " << dimension
            << ", size " << num_cells;

  FractionalControls fractional_controls(num_cells, dimension);

  for(idx k = 0; k < num_cells; ++k)
  {
    for(idx i = 0; i < dimension; ++i)
    {
      fractional_controls(k, i) = controls.at(i).at(k);
    }
  }

  return UniformReadResult{fractional_controls};
}

AdaptiveReadResult ControlReader::read_adaptive(std::istream &input)
{
  std::vector<idx> cell_volumes;
  std::vector<std::vector<double>> controls;

  std::string line;

  bool first_line = true;

  if(!input)
  {
    throw std::invalid_argument("Can't read from input");
  }

  while(std::getline(input, line))
  {
    std::vector<std::string> tokens;
    boost::algorithm::split(tokens, line, boost::is_any_of(":"));

    if(tokens.size() != 2)
    {
      throw ReadError();
    }

    const idx cell_volume = std::stoi(tokens[0]);

    cell_volumes.push_back(cell_volume);

    tokens = get_tokens(tokens[1]);

    if(first_line)
    {
      for(const auto& token : tokens)
      {
        controls.push_back(std::vector<double>{std::stod(token)});
      }

      first_line = false;
    }
    else
    {
      if(tokens.size() != controls.size())
      {
        throw ReadError();
      }

      for(idx i = 0; i < controls.size(); ++i)
      {
        controls[i].push_back(std::stod(tokens[i]));
      }
    }
  }

  const idx dimension = controls.size();
  const idx num_cells = controls[0].size();

  Log(info) << "Read in controls with dimension " << dimension
            << ", size " << num_cells;

  FractionalControls fractional_controls(num_cells, dimension);

  for(idx k = 0; k < num_cells; ++k)
  {
    for(idx i = 0; i < dimension; ++i)
    {
      fractional_controls(k, i) = controls.at(i).at(k);
    }
  }

  return AdaptiveReadResult{fractional_controls, cell_volumes};
}
