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

set(HEADERS
  dwt/dwt_label.hh
  dwt/dwt_program.hh
  scarp/scarp_label.hh
  scarp/scarp_program.hh
  sur/sur.hh
  ansi_color.hh
  bounded_label_set.hh
  cmp.hh
  control_writer.hh
  controls.hh
  cost_function.hh
  handle.hh
  instance.hh
  instance_reader.hh
  label.hh
  log.hh
  matrix.hh
  mesh.hh
  parameters.hh
  scarp.hh
  util.hh
)

add_library(scarp SHARED ${COMMON_SRC})

target_include_directories(scarp
  PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

install(TARGETS scarp
  EXPORT SCARP::SCARP
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${HEADERS}
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/scarp"
  COMPONENT "headers")

set_target_properties(scarp
  PROPERTIES
  VERSION ${PROJECT_VERSION})

set(LIBS
  ${Boost_LIBRARIES})

target_link_libraries(scarp ${LIBS})

configure_file(defs.hh.in ${CMAKE_BINARY_DIR}/defs.hh)

add_executable(scarp_solver scarp_solver.cc)
target_link_libraries(scarp_solver scarp)

add_executable(dwt_solver dwt_solver.cc)
target_link_libraries(dwt_solver scarp)

add_executable(sur_solver sur_solver.cc)
target_link_libraries(sur_solver scarp)

add_executable(solution_tester solution_tester.cc)
target_link_libraries(solution_tester scarp)
