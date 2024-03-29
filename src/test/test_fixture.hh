#ifndef SCARP_TEST_FIXTURE_HH
#define SCARP_TEST_FIXTURE_HH

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

#include "scarp/instance.hh"

#include "scarp/cost_function.hh"

using namespace scarp;

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

  virtual ~ProgramTest() {}
};


#endif /* SCARP_TEST_FIXTURE_HH */
