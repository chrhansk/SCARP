#include "test_fixture.hh"

#include <iostream>

#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>

#include <gtest/gtest.h>

#include "parameters.hh"

#include "instance_reader.hh"

#include "cmp.hh"

void ProgramTest::execute_all(const fs::path& result_path, bool adaptive, bool vanishing_constraints)
{
  auto split_string = [](const std::string& line)
    -> std::vector<std::string>
    {
      std::vector<std::string> entries;

      boost::split(entries, line, boost::is_any_of(" "));

      for(auto& entry : entries)
      {
        boost::algorithm::trim(entry);
      }

      return entries;
    };

  auto read_header = [&](const std::string& line)
    -> std::map<std::string, int>
    {
      std::map<std::string, int> fields;

      std::vector<std::string> entries = split_string(line);

      int i = 0;

      for(auto& entry : entries)
      {
        fields.insert({entry, i++});
      }

      return fields;
    };

  fs::ifstream input{result_path};

  std::string line;

  std::getline(input, line);

  auto fields = read_header(line);

  const int name_field = fields.at("Name");
  const int cost_field = fields.at("SCARP");

  while(std::getline(input, line))
  {
    auto row = split_string(line);

    auto instance_name = row.at(name_field);

    double expected = std::stof(row.at(cost_field));

    fs::path instance_path = result_path.parent_path().parent_path() / fs::path(instance_name);

    fs::ifstream instance_input{instance_path};

    auto instance = InstanceReader().read(instance_input, adaptive);
    const FractionalControls& fractional_controls = instance.get_fractional_controls();

    const idx dimension = fractional_controls.dimension();

    std::vector<double> switch_on_costs = default_switch_on_costs(dimension);

    std::vector<double> switch_off_costs = default_switch_off_costs(dimension);

    SwitchCosts switch_costs(instance,
                             switch_on_costs,
                             switch_off_costs);

    double actual = execute(instance, switch_costs, vanishing_constraints);

    ASSERT_TRUE(cmp::eq(actual, expected, 1e-3));
  }
}
