#include <fstream>
#include <iostream>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <gtest/gtest.h>

#include "defs.hh"
#include "log.hh"

#include "scarp/uniform_program.hh"

#include "parameters.hh"

#include "test_fixture.hh"

class UniformProgramTest : public ProgramTest
{
private:
  double scale_factor;

protected:
  virtual double execute(const FractionalControls& fractional_controls,
                         const CostFunction& costs) override;

public:
  UniformProgramTest(double scale_factor)
    : scale_factor(scale_factor)
  {}
};

double UniformProgramTest::execute(const FractionalControls& fractional_controls,
                                   const CostFunction& costs)
{
  const idx dimension = fractional_controls.dimension();

  UniformProgram program(fractional_controls,
                         costs,
                         scale_factor);

  auto controls = program.solve();

  return costs.total_cost(controls, fractional_controls);
}


TEST(UniformProgram, solve_noreg_512_triple_10)
{
  log_init();

  UniformProgramTest program_test(1.0);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/noreg/512_triple/results_1.0.csv"));
}

TEST(UniformProgram, solve_noreg_512_triple_15)
{
  log_init();

  UniformProgramTest program_test(1.5);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/noreg/512_triple/results_1.5.csv"));
}

TEST(UniformProgram, solve_noreg_512_triple_20)
{
  log_init();

  UniformProgramTest program_test(2.0);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/noreg/512_triple/results_2.0.csv"));
}


TEST(UniformProgram, solve_l1reg_512_triple_10)
{
  log_init();

  UniformProgramTest program_test(1.0);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/l1reg/512_triple/results_1.0.csv"));
}

TEST(UniformProgram, solve_l1reg_512_triple_15)
{
  log_init();

  UniformProgramTest program_test(1.5);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/l1reg/512_triple/results_1.5.csv"));
}

TEST(UniformProgram, solve_l1reg_512_triple_20)
{
  log_init();

  UniformProgramTest program_test(2.0);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/l1reg/512_triple/results_2.0.csv"));
}
