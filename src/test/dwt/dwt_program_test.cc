#include <boost/algorithm/string.hpp>
#include <gtest/gtest.h>

#include "defs.hh"

#include "scarp/bounds.hh"
#include "scarp/log.hh"
#include "scarp/dwt/dwt_program.hh"

#include "test_fixture.hh"

class DWTProgramTest : public ProgramTest
{
private:
  double scale_factor;

protected:
  virtual double execute(const Instance& instance,
                         const CostFunction& costs,
                         bool vanishing_constraints) override;

  virtual double bound_for(const Instance& instance,
                           bool vanishing_constraints) const;

public:
  DWTProgramTest(double scale_factor)
    : scale_factor(scale_factor)
  {}
};

double DWTProgramTest::execute(const Instance& instance,
                               const CostFunction& costs,
                               bool vanishing_constraints)
{
  const FractionalControls& fractional_controls = instance.get_fractional_controls();
  const double max_deviation = bound_for(instance, vanishing_constraints);

  const idx dimension = fractional_controls.dimension();

  std::vector<idx> dwelling_times(dimension, 0);

  DWTProgram program(fractional_controls,
                     costs,
                     dwelling_times,
                     max_deviation,
                     vanishing_constraints);

  auto controls = program.solve();

  return costs.total_cost(controls, fractional_controls);
}

double DWTProgramTest::bound_for(const Instance& instance,
                                 bool vanishing_constraints) const
{
  const idx dimension = instance.dimension();

  return bounds::sur(dimension, vanishing_constraints).for_mesh(instance.get_mesh(),
                                                                scale_factor);
}


TEST(DWTProgram, solve_noreg_512_triple_10)
{
  log_init();

  DWTProgramTest program_test(1.0);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/noreg/512_triple/results_1.0.csv"));
}

TEST(DWTProgram, solve_noreg_512_triple_15)
{
  log_init();

  DWTProgramTest program_test(1.5);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/noreg/512_triple/results_1.5.csv"));
}

TEST(DWTProgram, solve_noreg_512_triple_20)
{
  log_init();

  DWTProgramTest program_test(2.0);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/noreg/512_triple/results_2.0.csv"));
}


TEST(DWTProgram, solve_l1reg_512_triple_10)
{
  log_init();

  DWTProgramTest program_test(1.0);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/l1reg/512_triple/results_1.0.csv"));
}

TEST(DWTProgram, solve_l1reg_512_triple_15)
{
  log_init();

  DWTProgramTest program_test(1.5);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/l1reg/512_triple/results_1.5.csv"));
}

TEST(DWTProgram, solve_l1reg_512_triple_20)
{
  log_init();

  DWTProgramTest program_test(2.0);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/l1reg/512_triple/results_2.0.csv"));
}
