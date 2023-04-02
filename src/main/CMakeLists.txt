set(COMMON_SRC
  ansi_color.cc
  cmp.cc
  instance_reader.cc
  control_writer.cc
  controls.cc
  cost_function.cc
  log.cc
  parameters.cc
  scarp/scarp_program.cc
  dwt/dwt_program.cc
  sur/sur.cc
  util.cc)

add_library(common ${COMMON_SRC})

set(LIBS
  ${Boost_LIBRARIES})

target_link_libraries(common ${LIBS})

configure_file(defs.hh.in ${CMAKE_BINARY_DIR}/defs.hh)

add_executable(scarp_solver scarp_solver.cc)
target_link_libraries(scarp_solver common)

add_executable(dwt_solver dwt_solver.cc)
target_link_libraries(dwt_solver common)

add_executable(sur_solver sur_solver.cc)
target_link_libraries(sur_solver common)

add_executable(solution_tester solution_tester.cc)
target_link_libraries(solution_tester common)
