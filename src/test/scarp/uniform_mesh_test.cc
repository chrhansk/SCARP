#include "scarp/scarp_program_test.hh"

#include <gtest/gtest.h>

#include "defs.hh"
#include "instance.hh"
#include "log.hh"

TEST(SCARPProgram, solve_noreg_512_triple_10)
{
  log_init();

  SCARPProgramTest program_test(1.0);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/noreg/512_triple/results_1.0.csv"));
}

TEST(SCARPProgram, solve_noreg_512_triple_15)
{
  log_init();

  SCARPProgramTest program_test(1.5);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/noreg/512_triple/results_1.5.csv"));
}

TEST(SCARPProgram, solve_noreg_512_triple_20)
{
  log_init();

  SCARPProgramTest program_test(2.0);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/noreg/512_triple/results_2.0.csv"));
}


TEST(SCARPProgram, solve_l1reg_512_triple_10)
{
  log_init();

  SCARPProgramTest program_test(1.0);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/l1reg/512_triple/results_1.0.csv"));
}

TEST(SCARPProgram, solve_l1reg_512_triple_15)
{
  log_init();

  SCARPProgramTest program_test(1.5);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/l1reg/512_triple/results_1.5.csv"));
}

TEST(SCARPProgram, solve_l1reg_512_triple_20)
{
  log_init();

  SCARPProgramTest program_test(2.0);
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/l1reg/512_triple/results_2.0.csv"));
}
