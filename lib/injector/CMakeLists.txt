cmake_minimum_required(VERSION 3.20)
project(injector)

set(CMAKE_CXX_STANDARD 23)

add_executable(injector ./src/main.cpp ./src/injector.cpp)

target_link_libraries( injector PRIVATE jvm-toolbox-ipc-lib )