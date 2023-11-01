cmake_minimum_required(VERSION 3.8)
project(ServerApp VERSION 0.5 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify paths for dependencies
list(APPEND CMAKE_PREFIX_PATH "/root/.local")

# Sets to autogenerate the MOC files
set(CMAKE_AUTOMOC ON)

find_package(Threads REQUIRED)

# Find Qt6 components
find_package(Qt6 COMPONENTS Gui REQUIRED)

# Find Protobuf
find_package(Protobuf CONFIG REQUIRED)

# Find gRPC
find_package(gRPC CONFIG REQUIRED)

# Find OpenCV
find_package(OpenCV REQUIRED)


# Check if Qt6 was found
if (Qt6_FOUND)
  message(STATUS "Qt6 was found")
else()
  message(FATAL_ERROR "Qt6 was not found")
endif()


# Check if OpenCV was found
if (OpenCV_FOUND)
  message(STATUS "OpenCV ${OpenCV_VERSION} was found")
else()
  message(FATAL_ERROR "OpenCV was not found")
endif()

include_directories(${OpenCV_INCLUDE_DIRS}
)

# Check if protobuf was found
if (Protobuf_FOUND)
  message(STATUS "Protobuf ${Protobuf_VERSION} was found")
else()
  message(FATAL_ERROR "Protobuf was not found")
endif()

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
  find_program(_PROTOBUF_PROTOC protoc)
else()
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

# Check if gRPC was found
if (gRPC_FOUND)
  message(STATUS "gRPC ${gRPC_VERSION} was found")
else()
  message(FATAL_ERROR "gRPC was not found")
endif()

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
  find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

# Gets the protos file path
get_filename_component(PROTO_FILE "./proto/proto.proto" ABSOLUTE)
get_filename_component(PROTO_FILE_PATH "${PROTO_FILE}" PATH)

# Sets the location for generated files
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/generated/)
set(PROTO_SOURCES "${CMAKE_BINARY_DIR}/generated/proto.pb.cc")
set(PROTO_HEADERS "${CMAKE_BINARY_DIR}/generated/proto.pb.h")
set(GRPC_SOURCES "${CMAKE_BINARY_DIR}/generated/proto.grpc.pb.cc")
set(GRPC_HEADERS "${CMAKE_BINARY_DIR}/generated/proto.grpc.pb.h")

set_property(SOURCE ${GRPC_SOURCES} ${PROTO_SOURCES} ${GRPC_HEADERS} ${PROTO_HEADERS} PROPERTY SKIP_AUTOMOC ON)

# Generate *.pb.* and *.qrpc.* files from .proto file
add_custom_command (
  OUTPUT "${PROTO_SOURCES}" "${PROTO_HEADERS}" "${GRPC_SOURCES}" "${GRPC_HEADERS}"
  COMMAND ${_PROTOBUF_PROTOC}
  ARGS --grpc_out "${CMAKE_BINARY_DIR}/generated/"
       --cpp_out "${CMAKE_BINARY_DIR}/generated/"
       -I "${PROTO_FILE_PATH}"
       --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
       "${PROTO_FILE}"
  DEPENDS "${PROTO_FILE}"
)
# Sets a library with generated files
add_library(GrpcProtoLib
  ${PROTO_SOURCES}
  ${PROTO_HEADERS}
  ${GRPC_SOURCES}
  ${GRPC_HEADERS}
)

# Links the library against the necessary dependencies
target_link_libraries(GrpcProtoLib
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF} 
)

# Sets the project sources file
set(PROJECT_SOURCES
  main.cc
  src/ImageProcessing.cc
  src/ImageConvertor.cc
)

# Sets the project headers file
set(PROJECT_HEADERS
    include/ImageProcessing.h
    include/ImageConvertor.h
)

# Sets the executable
add_executable(${CMAKE_PROJECT_NAME}
  ${PROJECT_SOURCES}
  ${PROJECT_HEADERS}
)

# Links the executable against the necessary dependencies
target_link_libraries(${CMAKE_PROJECT_NAME}
  GrpcProtoLib
  Qt6::Gui
  ${OpenCV_LIBS}
)

# Sets a library with project file for testing
add_library(${CMAKE_PROJECT_NAME}_lib
  ${PROJECT_SOURCES}
  ${PROJECT_HEADERS}
)

# Links the library against the necessary dependencies
target_link_libraries(${CMAKE_PROJECT_NAME}_lib
  GrpcProtoLib
  Qt6::Gui
  ${OpenCV_LIBS}
)

# Adds the test directory
# add_subdirectory(test)

