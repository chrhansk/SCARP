#include "control_reader.hh"

#include <boost/algorithm/string.hpp>

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

std::vector<Controls> ControlReader::read(std::istream& input)
{
  std::vector<Controls> controls;

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
        controls.push_back(Controls{std::stod(token)});
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

  Log(info) << "Read in controls with dimension " << controls.size()
            << ", size " << controls[0].size();

  return controls;
}
