#include <fstream>
#include <iostream>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <gtest/gtest.h>

#include "defs.hh"
#include "log.hh"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace fs = boost::filesystem;

#include "scarp/adaptive_program.hh"
#include "control_reader.hh"
#include "parameters.hh"

#include "csv_reader.hh"
#include "test_fixture.hh"

class AdaptiveProgramTest
{
private:
  double scale_factor;

protected:
  double execute(const FractionalControls& fractional_controls,
                 const std::vector<idx>& cell_volumes,
                 const CostFunction& costs);

public:
  AdaptiveProgramTest(double scale_factor)
    : scale_factor(scale_factor)
  {}

  void execute_all(const fs::path& result_path);
};

void AdaptiveProgramTest::execute_all(const fs::path& result_path)
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

    auto read_result = ControlReader().read_adaptive(instance_input);
    FractionalControls fractional_controls = read_result.fractional_controls;
    std::vector<idx> cell_volumes = read_result.cell_volumes;

    const idx dimension = fractional_controls.dimension();

    std::vector<double> switch_on_costs = default_switch_on_costs(dimension);

    std::vector<double> switch_off_costs = default_switch_off_costs(dimension);

    SwitchCosts switch_costs(switch_on_costs, switch_off_costs);

    double actual = execute(fractional_controls, cell_volumes, switch_costs);

    ASSERT_TRUE(cmp::eq(actual, expected, 1e-3));
  }
}

double AdaptiveProgramTest::execute(const FractionalControls& fractional_controls,
                                    const std::vector<idx>& cell_volumes,
                                    const CostFunction& costs)
{
  const idx dimension = fractional_controls.dimension();

  AdaptiveProgram<idx> program(fractional_controls,
                               costs,
                               cell_volumes,
                               scale_factor);

  auto controls = program.solve();

  return costs.total_cost(controls, fractional_controls);
}

/*
TEST(AdaptiveProgram, solve_noreg_512_triple_10)
{
  log_init();

  AdaptiveProgramTest program_test(1.0);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/adaptive/results.csv"));
}
*/
