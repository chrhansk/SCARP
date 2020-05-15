add_library(perf_common
  EXCLUDE_FROM_ALL
  benchmark.cc)

target_link_libraries(perf_common
  common)

add_custom_target(build_benchmarks)

add_custom_target(run_benchmarks
  DEPENDS build_benchmarks)

add_custom_target(collect)

add_dependencies(collect build_benchmarks)

set(DATASET_DIR "${CMAKE_SOURCE_DIR}/dataset/lotka_volterra")

set(OUTPUT_DIR "${DATASET_DIR}/solutions")

file(MAKE_DIRECTORY ${OUTPUT_DIR})

function(add_benchmark)
  cmake_parse_arguments(PARSE_ARGV 0 ARGS "" "TARGET" "SOURCES")

  add_executable(${ARGS_TARGET}
    EXCLUDE_FROM_ALL
    ${ARGS_SOURCES})

  target_include_directories(${ARGS_TARGET}
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}")

  add_dependencies(build_benchmarks ${ARGS_TARGET})

  target_link_libraries(${ARGS_TARGET} perf_common)
endfunction()

function(add_collect_target)
  set(oneValueArgs INSTANCE SCALE DRIVER)
  cmake_parse_arguments(PARSE_ARGV 0 ARGS "" "${oneValueArgs}" "")

  set(INSTANCE_DIR "${DATASET_DIR}/${ARGS_INSTANCE}")

  set(TARGET_OUTPUT_DIR "${OUTPUT_DIR}/${ARGS_DRIVER}/${ARGS_SCALE}")

  file(MAKE_DIRECTORY ${TARGET_OUTPUT_DIR})

  set(COLLECT_TARGET_NAME "collect_${ARGS_DRIVER}_${ARGS_INSTANCE}_${ARGS_SCALE}")

  set(COLLECT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${COLLECT_TARGET_NAME}.csv")

  add_custom_target("${COLLECT_TARGET_NAME}"
    COMMAND "${ARGS_DRIVER}" --scale="${ARGS_SCALE}" "${INSTANCE_DIR}" "${TARGET_OUTPUT_DIR}" > "${COLLECT_FILE}")

  add_dependencies("${COLLECT_TARGET_NAME}" "${ARGS_DRIVER}")

  add_dependencies(collect "${COLLECT_TARGET_NAME}")

endfunction()

add_benchmark(
  TARGET scarp_benchmark
  SOURCES scarp_benchmark.cc)

add_benchmark(
  TARGET sur_benchmark
  SOURCES sur_benchmark.cc)

add_benchmark(
  TARGET scarp_zero_benchmark
  SOURCES scarp_zero_benchmark.cc)

set(INSTANCES
  "512"
  "512_triple"
  "1024"
  "1024_triple")

set(SCALES
  "1.0"
  "1.5"
  "2.0")

foreach(INSTANCE ${INSTANCES})
  add_custom_target("collect_${INSTANCE}")

  foreach(SCALE ${SCALES})

    add_collect_target(
      INSTANCE ${INSTANCE}
      SCALE ${SCALE}
      DRIVER scarp_benchmark)

    # add_collect_target(
    #   INSTANCE ${INSTANCE}
    #   SCALE ${SCALE}
    #   DRIVER scarp_zero_benchmark)

    add_collect_target(
      INSTANCE ${INSTANCE}
      SCALE ${SCALE}
      DRIVER sur_benchmark)


  endforeach()

endforeach()
