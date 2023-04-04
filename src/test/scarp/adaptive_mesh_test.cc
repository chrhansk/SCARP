#include "scarp/adaptive_program_test.hh"

#include <gtest/gtest.h>

#include "defs.hh"
#include "scarp/instance.hh"
#include "scarp/log.hh"

TEST(AdaptiveMesh, solve_adaptive)
{
  log_init();

  AdaptiveProgramTest program_test;
  program_test.execute_all(fs::path(TESTS_DIRECTORY "/adaptive/results.csv"),
                           true); // vanishing constraints
}
