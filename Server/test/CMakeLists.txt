# test/CMakeLists.txt
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

# Prevent overriding the parent project's compiler/linker
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
message(STATUS "GoogleTest was downloaded")

enable_testing()

find_package(Qt6 COMPONENTS Core Gui REQUIRED)
find_package(OpenCV REQUIRED)
find_package(gRPC REQUIRED)

add_executable(
  RunTests
  main.cc
  ImageProcessingTest.cc
  ImageConvertorTest.cc
  ../src/ImageProcessing.cc
  ../src/ImageConvertor.cc
)

target_link_libraries(
  RunTests
  Qt6::Core
  Qt6::Gui
  ${OpenCV_LIBS}
  Matplot++::matplot
  gRPC::grpc++
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(RunTests)
