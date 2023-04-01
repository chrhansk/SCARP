#ifndef INSTANCE_READER_HH
#define INSTANCE_READER_HH

#include <iostream>
#include <fstream>

#include "instance.hh"

class InstanceReader
{
public:
  Instance read_uniform(std::istream& input);
};


#endif /* INSTANCE_READER_HH */
