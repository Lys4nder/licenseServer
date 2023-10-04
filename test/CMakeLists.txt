include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        e40661d89b051e9ef4eb8a2420b74bf78b39ef41 # release-1.10.0
  FIND_PACKAGE_ARGS NAMES GTest
)

# Prevent overriding the parent project's compiler/linker
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
message(STATUS "GoogleTest was downloaded")

# Find Qt6Test
find_package(Qt6Test REQUIRED)

# Check if Qt6Test was found
if (Qt6Test_FOUND)
  message(STATUS "Qt6Test ${Qt6Test_VERSION} was found")
else()
  message(FATAL_ERROR "Qt6Test was not found")
endif()

enable_testing()

add_executable(
  test
  ../test/src/tests.cc
)

target_link_libraries(
  test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test)