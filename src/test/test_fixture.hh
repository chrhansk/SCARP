#ifndef TEST_FIXTURE_HH
#define TEST_FIXTURE_HH

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

#include "instance.hh"

#include "cost_function.hh"

class ProgramTest
{
protected:
  virtual double execute(const Instance& instance,
                         const CostFunction& costs,
                         bool vanishing_constraints) = 0;

public:
  void execute_all(const fs::path& result_file,
                   bool adaptive=false,
                   bool vanishing_constraints=false);
};


#endif /* TEST_FIXTURE_HH */
