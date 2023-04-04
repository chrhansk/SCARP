#ifndef SCARP_HANDLE_HH
#define SCARP_HANDLE_HH

#include "scarp/util.hh"

namespace scarp
{

class Handle
{
private:
  idx _value;

public:
  explicit Handle(idx value)
    : _value(value)
  {}

  idx value() const
  {
    return _value;
  }

  operator bool()
  {
    return value() != Handle::invalid().value();
  }

  static Handle invalid()
  {
    return Handle((idx) -1);
  }
};

} // namespace scarp

#endif /* SCARP_HANDLE_HH */
