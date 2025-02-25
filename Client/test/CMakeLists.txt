include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/3fbe4db9a39291ae8d7a9c5f1d75896bb4c5a18f.zip
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
