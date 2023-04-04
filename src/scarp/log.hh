#ifndef LOG_HH
#define LOG_HH

#include <boost/log/trivial.hpp>

#define Log BOOST_LOG_TRIVIAL

namespace scarp
{

void log_init(bool quiet=false);

} // namespace scarp

#endif /* LOG_HH */
