cmake_minimum_required(VERSION 3.30)

project(SCARP
  VERSION 0.1.0
  LANGUAGES CXX)

string(TOLOWER ${PROJECT_NAME} project_name)

include(GNUInstallDirs)

set(PROJECT_NAMESPACE "${project_name}::")

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

enable_testing()

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.16.0
)

FetchContent_MakeAvailable(googletest)

add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

find_package(Boost 1.70
  COMPONENTS date_time filesystem log log_setup program_options thread system
  REQUIRED)

add_definitions(-DBOOST_LOG_DYN_LINK)

if(CMAKE_BUILD_TYPE MATCHES "Release")
  add_definitions("-DNDEBUG")
endif()

add_subdirectory(src)

set(CONFIG_FILE "${CMAKE_SOURCE_DIR}/cmake/${project_name}-config.cmake")
set(CONFIG_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/${project_name}-config-version.cmake")
set(CONFIG_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${project_name}")

set(TARGET_FILE "${project_name}-targets.cmake")

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
