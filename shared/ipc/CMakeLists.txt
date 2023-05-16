cmake_minimum_required(VERSION 3.20)

project(jvm-toolbox-ipc)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

find_package(Flatbuffers CONFIG REQUIRED)


# Include directories
include_directories(include)

# Add source files
add_executable(jvm-toolbox-ipc src/main.cpp src/shared_memory_manager.cpp src/consumer.cpp)
add_library(jvm-toolbox-ipc-lib src/shared_memory_manager.cpp src/consumer.cpp)
target_link_libraries(jvm-toolbox-ipc PRIVATE flatbuffers::flatbuffers)

find_package(GTest CONFIG REQUIRED)

# Add test files
add_executable(jvm-toolbox-ipc-test test/main.cpp test/shared_memory_manager_test.cpp test/consumer_producer_tests.cpp)
target_link_libraries(jvm-toolbox-ipc-test PRIVATE flatbuffers::flatbuffers GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main jvm-toolbox-ipc-lib)