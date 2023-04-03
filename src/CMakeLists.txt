#include_directories(${CMAKE_SOURCE_DIR}/src/main)

set(SCARP_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(scarp)
add_subdirectory(test)
add_subdirectory(perf)
