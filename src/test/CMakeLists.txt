add_custom_target(build_tests)

set(TEST_COMMON_SRC
  test_fixture.cc
  scarp/scarp_program_test.cc)

add_library(test_common ${TEST_COMMON_SRC})

target_include_directories(test_common
    PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR})

target_link_libraries(test_common
  PUBLIC
  scarp
  GTest::GTest)

function(add_unit_test TEST_NAME)
  get_filename_component(BASE_NAME ${TEST_NAME} NAME)
  add_executable(${BASE_NAME} ${TEST_NAME}.cc)

  target_link_libraries(${BASE_NAME}
    test_common)

  add_dependencies(build_tests ${BASE_NAME})

  add_test(NAME ${BASE_NAME} COMMAND ${BASE_NAME})
endfunction()


include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_unit_test(scarp/uniform_mesh_test)
add_unit_test(scarp/adaptive_mesh_test)
add_unit_test(scarp/adaptive_mesh_vc_test)
