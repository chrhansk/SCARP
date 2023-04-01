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
                         const CostFunction& costs) = 0;

public:
  void execute_all(const fs::path& result_file);
};


#endif /* TEST_FIXTURE_HH */
