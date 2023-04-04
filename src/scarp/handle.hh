#ifndef HANDLE_HH
#define HANDLE_HH

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

#endif /* HANDLE_HH */
