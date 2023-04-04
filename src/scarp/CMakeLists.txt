set(COMMON_SRC
  ansi_color.cc
  cmp.cc
  instance_reader.cc
  control_writer.cc
  controls.cc
  cost_function.cc
  log.cc
  mesh.cc
  parameters.cc
  scarp/scarp_label.cc
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

target_link_libraries(scarp
  PRIVATE
  ${Boost_LIBRARIES})

target_include_directories(scarp
  PUBLIC
  "$<BUILD_INTERFACE:${SCARP_DIR}>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
  ${BOOST_INCLUDE_DIRS}
)

install(TARGETS scarp
  EXPORT ${PROJECT_NAMESPACE}SCARP
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT "libraries")

foreach(file ${HEADERS})
  get_filename_component(dir ${file} DIRECTORY)
  install(FILES ${file}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/scarp/${dir}
    COMPONENT "headers")
endforeach()

set_target_properties(scarp
  PROPERTIES
  VERSION ${PROJECT_VERSION})

configure_file(defs.hh.in ${CMAKE_BINARY_DIR}/defs.hh)

add_executable(scarp_solver scarp_solver.cc)
target_link_libraries(scarp_solver ${Boost_LIBRARIES} scarp)

add_executable(dwt_solver dwt_solver.cc)
target_link_libraries(dwt_solver ${Boost_LIBRARIES} scarp)

add_executable(sur_solver sur_solver.cc)
target_link_libraries(sur_solver ${Boost_LIBRARIES} scarp)

add_executable(solution_tester solution_tester.cc)
target_link_libraries(solution_tester ${Boost_LIBRARIES} scarp)
