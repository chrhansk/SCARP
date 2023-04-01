#ifndef CSV_READER_HH
#define CSV_READER_HH

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

class CSVReader
{
private:
  std::unordered_map<std::string, int> read_header(const std::string& line);
  std::vector<std::string> read_line(const std::string& line);

  std::unordered_map<std::string, int> header;
  std::vector<std::vector<std::string>> lines;
public:
  CSVReader(std::istream& input);

  int field(const std::string& name)
  {
    return header[name];
  }

  const std::vector<std::vector<std::string>>& get_lines() const
  {
    return lines;
  }
};


#endif /* CSV_READER_HH */
