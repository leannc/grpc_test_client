cmake_minimum_required(VERSION 3.21)
project(grpc_test_client)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} 
  src/main.cpp
  src/TestClient.h
  src/TestClient.cpp
  src/ShapeReader.h
  src/ShapeReader.cpp
  src/MachineStatusReader.h
  src/MachineStatusReader.cpp

  ${CMAKE_SOURCE_DIR}/../grpc_test_server/src/proto/gen/test.pb.cc
  ${CMAKE_SOURCE_DIR}/../grpc_test_server/src/proto/gen/test.grpc.pb.cc
 )

file(GLOB_RECURSE grpc_libs "${GRPC_LIBS_DIR}/*.lib")

target_link_libraries(${PROJECT_NAME}
   ${grpc_libs}
)

target_include_directories(${PROJECT_NAME}
   PRIVATE
  ${GRPC_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/../grpc_test_server/src/proto/gen/
)

install(TARGETS  ${PROJECT_NAME}
   RUNTIME DESTINATION ${install_DIR}
)