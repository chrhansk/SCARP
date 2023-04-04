#ifndef ADAPTIVE_PROGRAM_TEST_HH
#define ADAPTIVE_PROGRAM_TEST_HH

#include "scarp_program_test.hh"

class AdaptiveProgramTest : public SCARPProgramTest
{
public:
  virtual double bound_for(const Instance& instance,
                           bool vanishing_constraints) const override;
};


#endif /* ADAPTIVE_PROGRAM_TEST_HH */
