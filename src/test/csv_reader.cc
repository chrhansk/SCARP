#include "csv_reader.hh"

#include <boost/algorithm/string.hpp>

CSVReader::CSVReader(std::istream &input)
{
  std::string line;
  std::getline(input, line);

  header = read_header(line);

  while(std::getline(input, line))
  {
    lines.push_back(read_line(line));
  }
}

std::unordered_map<std::string, int>
CSVReader::read_header(const std::string &line)
{
  std::unordered_map<std::string, int> fields;

  std::vector<std::string> entries;

  boost::split(entries, line, boost::is_any_of(" "));

  int i = 0;

  for(auto& entry : entries)
  {
    fields.insert({entry, i++});
  }

  return fields;
}

std::vector<std::string>
CSVReader::read_line(const std::string& line)
{
  std::vector<std::string> entries;

  boost::split(entries, line, boost::is_any_of(" "));

  for(auto& entry : entries)
  {
    boost::algorithm::trim(entry);
  }

  return entries;
}
