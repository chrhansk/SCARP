#include "test_fixture.hh"

#include <iostream>

#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>

#include <gtest/gtest.h>

#include "parameters.hh"

#include "csv_reader.hh"
#include "control_reader.hh"

#include "cmp.hh"

void ProgramTest::execute_all(const fs::path& result_path)
{
  fs::ifstream input{result_path};

  CSVReader reader(input);

  const int name_field = reader.field("Name");
  const int cost_field = reader.field("SCARP");

  for(const auto& row : reader.get_lines())
  {
    auto instance_name = row.at(name_field);

    double expected = std::stof(row.at(cost_field));

    fs::path instance_path = result_path.parent_path().parent_path() / fs::path(instance_name);

    fs::ifstream instance_input{instance_path};

    auto read_result = ControlReader().read_uniform(instance_input);
    FractionalControls fractional_controls = read_result.fractional_controls;

    const idx dimension = fractional_controls.dimension();

    std::vector<double> switch_on_costs = default_switch_on_costs(dimension);

    std::vector<double> switch_off_costs = default_switch_off_costs(dimension);

    SwitchCosts switch_costs(switch_on_costs, switch_off_costs);

    double actual = execute(fractional_controls, switch_costs);

    ASSERT_TRUE(cmp::eq(actual, expected, 1e-3));
  }
}
