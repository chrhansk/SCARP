cmake_minimum_required(VERSION 3.1)

project(SCARP
  VERSION 0.1.0
  LANGUAGES CXX)

include(GNUInstallDirs)

set(PROJECT_NAMESPACE "${PROJECT_NAME}::")

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

enable_testing()

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

find_package(Boost 1.54
  COMPONENTS date_time filesystem log log_setup program_options thread system
  REQUIRED)

add_definitions(-DBOOST_LOG_DYN_LINK)

if(CMAKE_BUILD_TYPE MATCHES "Release")
  add_definitions("-DNDEBUG")
endif()

add_subdirectory(src)

set(CONFIG_FILE "${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake")
set(CONFIG_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake")
set(CONFIG_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/sleqp")

set(TARGET_FILE "${PROJECT_NAME}-targets.cmake")

install(EXPORT ${PROJECT_NAMESPACE}SCARP
  FILE ${TARGET_FILE}
  NAMESPACE
  ${PROJECT_NAMESPACE}
  DESTINATION
  ${CONFIG_DESTINATION}
  COMPONENT "libraries"
)

write_basic_package_version_file(
  ${CONFIG_VERSION_FILE}
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
  ${CONFIG_FILE}
  ${CONFIG_VERSION_FILE}
  DESTINATION
  ${CONFIG_DESTINATION}
  COMPONENT "libraries"
)
