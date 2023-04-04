#ifndef SCARP_LOG_HH
#define SCARP_LOG_HH

#include <boost/log/trivial.hpp>

#define Log BOOST_LOG_TRIVIAL

namespace scarp
{

void log_init(bool quiet=false);

} // namespace scarp

#endif /* SCARP_LOG_HH */
